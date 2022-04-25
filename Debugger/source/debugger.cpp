#include <main.h>

#include <cassert>
#include <stdio.h>

DWORD WINAPI debugger_wait_routine(LPVOID lpThreadParameter)
{
	c_debugger* debugger = static_cast<c_debugger*>(lpThreadParameter);

	while (true)
	{
		if ((GetAsyncKeyState(VK_NEXT) & 0x8001) == 0x8001)
			break;
		Sleep(1);
	}

	debugger->detach();

	return 0;
}

void dump_to_console(const char* line_prefix, unsigned char* buffer, unsigned long long buffer_length, unsigned long max_length, unsigned long line_width = 16)
{
	printf("%ssize: %llu\n", line_prefix, buffer_length);
	printf("%soffset(h) ", line_prefix);
	unsigned long line_offset = 0;
	while (line_offset < line_width)
	{
		if (line_offset > 0 && line_offset % (line_width / 2) == 0)
			printf(" ");

		printf("%02X ", line_offset++);
	}

	unsigned long buffer_offset = 0;
	while (buffer_offset < min(buffer_length, max_length))
	{
		if (buffer_offset > 0 && buffer_offset % (line_width / 2) == 0 && buffer_offset % line_width != 0)
			printf(" ");

		if (buffer_offset % line_width == 0)
			printf("\n%s%08X: ", line_prefix, buffer_offset);

		printf("%02X ", buffer[buffer_offset++]);
	}
	printf("\n");
}

void add_break_on_winmain(c_debugger*, LPMODULEINFO);

c_debugger::c_debugger(c_process* process) :
	m_process(*process),
	m_process_is_attached(false),
	m_debug_event({ 0 }),
	m_continue_status(DBG_CONTINUE),
	m_thread_handle(NULL),
	m_print_debug_strings(true)
{
	ZeroMemory(&m_breakpoints, sizeof(m_breakpoints));
	ZeroMemory(&m_module_info_callbacks, sizeof(m_module_info_callbacks));
}

c_debugger::~c_debugger()
{
	delete& m_process;
}

void c_debugger::attach()
{
	DebugActiveProcess(m_process.get_process_id());
	m_process_is_attached = true;

	CreateThread(NULL, 0, debugger_wait_routine, this, NULL, NULL);
}

void c_debugger::detach()
{
	DebugActiveProcessStop(m_process.get_process_id());
	m_process_is_attached = false;
}

void c_debugger::add_breakpoint(SIZE_T offset, bool print_registers, const char* opcode_name, const wchar_t* name, void(*callback)(c_debugger&, c_registers&))
{
	if (!opcode_name)
		return;

	if (m_breakpoints.count < m_breakpoints.get_max_count())
	{
		s_breakpoint& breakpoint = m_breakpoints[m_breakpoints.count++];

		printf("Adding breakpoint at 0x%016zX for %ls\n", offset, name);

		breakpoint.module_offset = offset;
		breakpoint.print_registers = print_registers;
		breakpoint.opcode_name = opcode_name;
		wcsncpy_s(breakpoint.name, name, NUMBER_OF(breakpoint.name));
		breakpoint.callback = callback;
	}
	else
	{
		printf("Unable to add breakpoint at 0x%016zX for %ls\n", offset, name);
	}
}

void c_debugger::add_module_info_callback(const char* callback_name, void(*callback)(c_debugger&, LPMODULEINFO))
{
	if (m_module_info_callbacks.count < m_module_info_callbacks.get_max_count())
	{
		printf("Adding `MODULEINFO` callback for %s\n", callback_name);
		m_module_info_callbacks[m_module_info_callbacks.count++] = callback;
	}
	else
	{
		printf("Unable to add `MODULEINFO` callback for %s\n", callback_name);
	}
}

c_process& c_debugger::get_process()
{
	return m_process;
}

void c_debugger::run_debugger(bool print_debug_strings)
{
	m_print_debug_strings = print_debug_strings;

	CONTEXT context = { 0 };

	bool first_run = true;

	HMODULE modules[1] = { 0 };
	MODULEINFO module_info = { 0 };

	SIZE_T bytes_written = 0;
	SIZE_T bytes_read = 0;

	SIZE_T offset = 0;

	SIZE_T call_location = 0;
	SIZE_T call_location_bytes_read = 0;
	SIZE_T last_call_location = 0;

	BYTE instructions[READ_PAGE_SIZE] = { 0 };

	DWORD breakpoints_set = 0;

	WCHAR module_name[MAX_PATH] = { 0 };

	while (m_process_is_attached)
	{
		memset(modules, 0, sizeof(modules));
		memset(instructions, 0, sizeof(instructions));
		memset(module_name, 0, sizeof(module_name));

		m_continue_status = DBG_CONTINUE;

		if (!WaitForDebugEvent(&m_debug_event, INFINITE))
			return;

		switch (m_debug_event.dwDebugEventCode)
		{
		case EXCEPTION_DEBUG_EVENT:
		{
			switch (m_debug_event.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case EXCEPTION_BREAKPOINT:
			{
				if (first_run)
				{
					first_run = false;

					m_thread_handle = OpenThread(THREAD_ALL_ACCESS, true, m_debug_event.dwThreadId);

					EnumProcessModules(m_process.get_process_handle(), modules, sizeof(modules), (LPDWORD)&bytes_read);
					if (*modules)
						GetModuleInformation(m_process.get_process_handle(), *modules, &module_info, sizeof(module_info));

					for (SIZE_T i = 0; i < m_module_info_callbacks.count; i++)
					{
						decltype(*m_module_info_callbacks.data) callback = m_module_info_callbacks[i];
						callback(*this, &module_info);
					}

					for (SIZE_T i = 0; i < m_breakpoints.count; i++)
					{
						s_breakpoint& breakpoint = m_breakpoints[i];
						LPVOID call_breakpoint_address = (LPVOID)((SIZE_T)module_info.lpBaseOfDll + breakpoint.module_offset);

						read_debuggee_memory(call_breakpoint_address, &instructions, READ_PAGE_SIZE, &bytes_read);
						for (SIZE_T c = 0; c < bytes_read; c++)
						{
							if (!instruction_startswith(instructions + c, instructions + (bytes_read - c), breakpoint.opcode_name))
								continue;

							breakpoint.break_on = instructions[c];

							if (breakpoint.break_on == k_call_instruction)
							{
								offset = (SIZE_T)call_breakpoint_address + c;
								read_debuggee_memory((LPVOID)(offset + 1), &call_location, 4, &call_location_bytes_read);
								call_location += offset + 5;
								if (call_location < (SIZE_T)module_info.lpBaseOfDll || call_location >(SIZE_T)module_info.lpBaseOfDll + module_info.SizeOfImage)
									continue;
							}
							else
							{
								offset = (SIZE_T)call_breakpoint_address + c;
							}

							if (breakpoints_set < m_breakpoints.get_max_count())
							{
								write_debuggee_memory((LPVOID)offset, &k_break_instruction, sizeof(k_break_instruction), &bytes_written);
								FlushInstructionCache(m_process.get_process_handle(), (LPVOID)offset, 1);

								breakpoints_set++;
								break;
							}
						}
					}

					m_process.resume_thread();
				}
				else
				{
					m_thread_handle = OpenThread(THREAD_ALL_ACCESS, true, m_debug_event.dwThreadId);
					if (m_thread_handle != NULL)
					{
						context.ContextFlags = CONTEXT_ALL;
						GetThreadContext(m_thread_handle, &context);

						context.Xip--;
						context.EFlags |= 0x100;

						SetThreadContext(m_thread_handle, &context);
						CloseHandle(m_thread_handle);

						BYTE instruction = 0;
						for (SIZE_T i = 0; i < m_breakpoints.count; i++)
						{
							s_breakpoint& breakpoint = m_breakpoints[i];
							if ((context.Xip - (SIZE_T)module_info.lpBaseOfDll) == breakpoint.module_offset)
							{
								instruction = breakpoint.break_on;
								break;
							}
						}

						if (instruction)
						{
							write_debuggee_memory((LPVOID)context.Xip, &instruction, sizeof(instruction), &bytes_written);
							FlushInstructionCache(m_process.get_process_handle(), (LPVOID)context.Xip, 1);
						}

						last_call_location = context.Xip;
					}
				}

				m_continue_status = DBG_CONTINUE;
				break;
			}
			case EXCEPTION_SINGLE_STEP:
			{
				m_thread_handle = OpenThread(THREAD_ALL_ACCESS, true, m_debug_event.dwThreadId);
				if (m_thread_handle != NULL)
				{
					context.ContextFlags = CONTEXT_ALL;
					GetThreadContext(m_thread_handle, &context);
					SuspendThread(m_thread_handle);

					c_registers registers(&module_info, context);

					if (last_call_location)
					{
						write_debuggee_memory((LPVOID)last_call_location, &k_break_instruction, sizeof(k_break_instruction), &bytes_written);
						FlushInstructionCache(m_process.get_process_handle(), (LPVOID)last_call_location, 1);

						//DWORD module_offset = /*0x00400000*/PE32_BASE - (DWORD)module_info.lpBaseOfDll;

						decltype(s_breakpoint::callback) callback = nullptr;
						LPWSTR name = NULL;
						bool print_registers = true;
						for (SIZE_T i = 0; i < m_breakpoints.count; i++)
						{
							s_breakpoint& breakpoint = m_breakpoints[i];
							SIZE_T instruction_pointer_offset = context.Xip - (SIZE_T)module_info.lpBaseOfDll;
							SIZE_T last_call_location_offset = last_call_location - (SIZE_T)module_info.lpBaseOfDll;

							if (instruction_pointer_offset == breakpoint.module_offset || last_call_location_offset == breakpoint.module_offset)
							{
								callback = breakpoint.callback;
								name = breakpoint.name;
								print_registers = breakpoint.print_registers;
								break;
							}
						}

						if (print_registers)
						{
							SIZE_T return_address = 0;
							SIZE_T call_address = -5;
							read_debuggee_memory((LPCVOID)context.Xsp, &return_address, sizeof(return_address), NULL);
							call_address += return_address;

							printf("\n%ls\n  ", (name && *name) ? name : L"no name");
							if (call_address > (SIZE_T)module_info.lpBaseOfDll)
							{
								printf("thread id: %08d, call %ls+0x%08zX\n", m_debug_event.dwThreadId, m_process.get_process_name(), call_address - (SIZE_T)module_info.lpBaseOfDll);
							}

							SIZE_T stack_data_size = 64; // context.Xbp - context.Xsp;
							unsigned char* stack_data = new unsigned char[stack_data_size] {};
							read_debuggee_memory((LPVOID)context.Xsp, stack_data, stack_data_size, &bytes_read);

							printf("  registers:\n");
							printf("    destination         (di): 0x%016zX\n", context.Xdi);
							printf("    source              (si): 0x%016zX\n", context.Xsi);
							printf("    base                (bx): 0x%016zX\n", context.Xbx);
							printf("    data                (dx): 0x%016zX\n", context.Xdx);
							printf("    counter             (cx): 0x%016zX\n", context.Xcx);
							printf("    accumulator         (ax): 0x%016zX\n", context.Xax);
							printf("    stack base pointer  (bp): 0x%016zX\n", context.Xbp);
							printf("    instruction pointer (ip): 0x%016zX\n", context.Xip);
							printf("    stack pointer       (sp): 0x%016zX\n", context.Xsp);

							printf("  stack:\n");
							dump_to_console("    ", stack_data, stack_data_size, 64, 16);

							SAFE_DELETE(stack_data);
						}

						last_call_location = 0;

						if (callback)
							callback(*this, registers);
					}

					SetThreadContext(m_thread_handle, &context);
					ResumeThread(m_thread_handle);
					CloseHandle(m_thread_handle);
				}

				m_continue_status = DBG_CONTINUE;
				break;
			}
			default:
			{
				m_continue_status = DBG_EXCEPTION_NOT_HANDLED;
				break;
			}
			}
			break;
		}
		case EXIT_PROCESS_DEBUG_EVENT:
		{
			m_process_is_attached = false;
			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			OUTPUT_DEBUG_STRING_INFO& DebugString = m_debug_event.u.DebugString;

			if (DebugString.fUnicode)
			{
				WCHAR* buffer = new WCHAR[DebugString.nDebugStringLength * sizeof(WCHAR)] {};
				read_debuggee_memory(DebugString.lpDebugStringData, buffer, DebugString.nDebugStringLength * sizeof(WCHAR), NULL);

				WORD nDebugStringLength = DebugString.nDebugStringLength;
				if (buffer[nDebugStringLength - 1] == 0)
					nDebugStringLength--;

				while (buffer[nDebugStringLength - 1] == '\n' || buffer[nDebugStringLength - 1] == '\r')
				{
					buffer[nDebugStringLength - 1] = 0;
					nDebugStringLength--;
				}

				if (*buffer && nDebugStringLength != 0)
				{
					//onecore\com\combase\objact\objact.cxx(826)\combase.dll!7622DC19: (caller: 7622C61B) ReturnHr(1) tid(4154) 800401F0 CoInitialize has not been called.
					if (wcsstr(buffer, L"combase.dll") != 0)
					{
						m_continue_status = DBG_CONTINUE;
						break;
					}

					if (m_print_debug_strings)
						wprintf_s(L"[debug] %s\n", buffer);
				}

				SAFE_DELETE(buffer);
			}
			else
			{
				CHAR* buffer = new CHAR[DebugString.nDebugStringLength * sizeof(CHAR)] {};

				read_debuggee_memory(DebugString.lpDebugStringData, buffer, DebugString.nDebugStringLength * sizeof(CHAR), NULL);

				WORD nDebugStringLength = DebugString.nDebugStringLength;
				if (buffer[nDebugStringLength - 1] == 0)
					nDebugStringLength--;

				while (buffer[nDebugStringLength - 1] == '\n' || buffer[nDebugStringLength - 1] == '\r')
				{
					buffer[nDebugStringLength - 1] = 0;
					nDebugStringLength--;
				}

				if (*buffer && nDebugStringLength != 0)
				{
					//onecore\com\combase\objact\objact.cxx(826)\combase.dll!7622DC19: (caller: 7622C61B) ReturnHr(1) tid(4154) 800401F0 CoInitialize has not been called.
					if (strstr(buffer, "combase.dll") != 0)
					{
						m_continue_status = DBG_CONTINUE;
						break;
					}

					if (m_print_debug_strings)
						printf_s("[debug] %s\n", buffer);
				}

				SAFE_DELETE(buffer);
			}

			m_continue_status = DBG_CONTINUE;
			break;
		}
		default:
		{
			m_continue_status = DBG_EXCEPTION_NOT_HANDLED;
			break;
		}
		}

		ContinueDebugEvent(m_debug_event.dwProcessId, m_debug_event.dwThreadId, m_continue_status);
	}

	m_process.close();
}


LPVOID c_debugger::allocate_debuggee_memory(
	_In_opt_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ DWORD flAllocationType,
	_In_ DWORD flProtect
)
{
	return VirtualAllocEx(m_process.get_process_handle(), lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL c_debugger::protect_debuggee_memory(
	_In_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ DWORD flNewProtect,
	_Out_ PDWORD lpflOldProtect
)
{
	return VirtualProtectEx(m_process.get_process_handle(), lpAddress, dwSize, flNewProtect, lpflOldProtect);
}

SIZE_T c_debugger::query_debuggee_memory(
	_In_opt_ LPCVOID lpAddress,
	_Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer,
	_In_ SIZE_T dwLength
)
{
	return VirtualQueryEx(m_process.get_process_handle(), lpAddress, lpBuffer, dwLength);
}

BOOL c_debugger::read_debuggee_memory(
	_In_ LPCVOID lpBaseAddress,
	_Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_opt_ SIZE_T* lpNumberOfBytesRead
)
{
	return ReadProcessMemory(m_process.get_process_handle(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

BOOL c_debugger::read_debuggee_pointer(
	_In_ LPCVOID lpBaseAddress,
	_Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_opt_ SIZE_T* lpNumberOfBytesRead
)
{
	SIZE_T data_ptr = 0;
	read_debuggee_memory(lpBaseAddress, &data_ptr, sizeof(void*), lpNumberOfBytesRead);
	return read_debuggee_memory((LPCVOID)data_ptr, lpBuffer, nSize, lpNumberOfBytesRead);
}

BOOL c_debugger::write_debuggee_memory(
	_In_ LPVOID lpBaseAddress,
	_In_reads_bytes_(nSize) LPCVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_opt_ SIZE_T* lpNumberOfBytesWritten
)
{
	return WriteProcessMemory(m_process.get_process_handle(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

BOOL c_debugger::write_debuggee_pointer(
	_In_ LPVOID lpBaseAddress,
	_In_ LPCVOID lpAddress,
	_Out_opt_ SIZE_T* lpNumberOfBytesWritten
)
{
	return write_debuggee_memory(lpBaseAddress, &lpAddress, sizeof(void*), lpNumberOfBytesWritten);
}

LPVOID c_debugger::allocate_and_write_debuggee_memory(
	_In_reads_bytes_(nSize) LPCVOID lpBuffer,
	_In_ SIZE_T nSize,
	_In_ DWORD flAllocationType,
	_In_ DWORD flProtect,
	_Out_opt_ SIZE_T* lpNumberOfBytesWritten
)
{
	LPVOID lpAddress = allocate_debuggee_memory(NULL, nSize, flAllocationType, flProtect);
	write_debuggee_memory(lpAddress, lpBuffer, nSize, lpNumberOfBytesWritten);

	return lpAddress;
}

BOOL c_debugger::dump_debuggee_memory(
	_In_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ LPCWSTR lpFileName
)
{
	if (!lpAddress || !dwSize || !(*lpFileName))
		return FALSE;

	FILE* file;
	_wfopen_s(&file, lpFileName, L"w");
	if (file == NULL || file == INVALID_HANDLE_VALUE)
		return FALSE;

	unsigned char* memory = new unsigned char[dwSize];
	read_debuggee_memory(lpAddress, memory, dwSize, NULL);
	fwrite(memory, sizeof(unsigned char), dwSize, file);

	fflush(file);
	fclose(file);

	SAFE_DELETE(memory);

	return TRUE;
}

void debugger_unprotect_module(c_debugger& debugger, LPCWSTR module_name)
{
	c_process& process = debugger.get_process();
	HANDLE process_handle = process.get_process_handle();

	HMODULE* modules = nullptr;
	DWORD module_count = process_get_modules(process, &modules);
	assert(modules != nullptr);

	DWORD module_index = process_get_module_index(process, modules, module_count, module_name);

	MODULEINFO module_info = { 0 };
	GetModuleInformation(process_handle, modules[module_index], &module_info, sizeof(module_info));

	SAFE_DELETE(modules);

	// which of these is better?
	if (false)
	{
		// is this better?
		DWORD protect;
		debugger.protect_debuggee_memory(module_info.lpBaseOfDll, module_info.SizeOfImage, PAGE_EXECUTE_READWRITE, &protect);
	}
	else
	{
		// or is this better?
		MEMORY_BASIC_INFORMATION memory_info = { 0 };
		for (SIZE_T module_offset = 0; module_offset < debugger.query_debuggee_memory(reinterpret_cast<LPCVOID>(reinterpret_cast<SIZE_T>(module_info.lpBaseOfDll) + module_offset), &memory_info, sizeof(MEMORY_BASIC_INFORMATION)); module_offset += memory_info.RegionSize)
		{
			if (memory_info.Protect != PAGE_EXECUTE_READ)
				continue;

			debugger.protect_debuggee_memory(memory_info.BaseAddress, memory_info.RegionSize, PAGE_EXECUTE_READWRITE, &memory_info.Protect);
		}
	}
}
