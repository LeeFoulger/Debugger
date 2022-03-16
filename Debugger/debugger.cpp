#include "main.h"

void dump_to_console(const char* string_before_line, unsigned char* buffer, unsigned long buffer_length, unsigned long max_length, unsigned long line_width = 16)
{
	printf("%ssize: %lu\n", string_before_line, buffer_length);
	printf("%soffset(h) ", string_before_line);
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
			printf("\n%s%08X: ", string_before_line, buffer_offset);

		printf("%02X ", buffer[buffer_offset++]);
	}
	printf("\n");
}

namespace instructions
{
	const unsigned char call_op = 0xE8;
	const unsigned char break_op = 0xCC;
}

void add_break_on_winmain(c_debugger*, LPMODULEINFO);

c_debugger::c_debugger(c_process* process)
{
	m_process = process;
	m_debug_event = { 0 };
	m_continue_status = DBG_CONTINUE;
	m_thread_handle = NULL;

	ZeroMemory(&m_call_breakpoints, sizeof(m_call_breakpoints));
}

void c_debugger::add_breakpoint(DWORD call_offset, const wchar_t* name, void(*callback)(c_debugger*, c_registers*))
{
	if (m_call_breakpoints.count < m_call_breakpoints.get_max_count())
	{
		s_breakpoint& call_breakpoint = m_call_breakpoints[m_call_breakpoints.count++];

		printf("Adding breakpoint at 0x%08X for %ls\n", call_offset, name);
		call_breakpoint.module_offset = call_offset;
		wcsncpy_s(call_breakpoint.name, name, 64);
		call_breakpoint.callback = callback;
	}
	else
	{
		printf("Unable to add breakpoint at 0x%08X for %ls\n", call_offset, name);
	}
}

void c_debugger::add_module_info_callback(void(*callback)(c_debugger*, LPMODULEINFO))
{
	if (m_module_info_callbacks.count < m_module_info_callbacks.get_max_count())
	{
		printf("Adding `MODULEINFO` callback for 0x%p\n", callback);
		m_module_info_callbacks[m_module_info_callbacks.count++] = callback;
	}
	else
	{
		printf("Unable to add `MODULEINFO` callback for 0x%p\n", callback);
	}
}

c_process* c_debugger::get_process()
{
	return m_process;
}

void c_debugger::run_debugger()
{
	CONTEXT context = { 0 };

	bool first_break_has_occurred = false;

	HMODULE modules[128] = { 0 };
	MODULEINFO module_info = { 0 };

	DWORD bytes_written = 0;
	DWORD bytes_read = 0;

	DWORD offset = 0;

	DWORD call_location = 0;
	DWORD call_location_bytes_read = 0;
	DWORD last_call_location = 0;

	unsigned char instructions[READ_PAGE_SIZE] = { 0 };

	DWORD breakpoints_set = 0;

	WCHAR module_name[MAX_PATH] = { 0 };

	DebugActiveProcess(m_process->get_process_id());

	bool process_running = true;
	while (process_running)
	{
		memset(module_name, 0, MAX_PATH * 2);

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
				if (!first_break_has_occurred)
				{
					m_thread_handle = OpenThread(THREAD_ALL_ACCESS, true, m_debug_event.dwThreadId);

					EnumProcessModules(m_process->get_process_handle(), modules, sizeof(modules), &bytes_read);
					if (*modules)
						GetModuleInformation(m_process->get_process_handle(), *modules, &module_info, sizeof(module_info));

					for (DWORD i = 0; i < m_module_info_callbacks.count; i++)
					{
						decltype(*m_module_info_callbacks.data) callback = m_module_info_callbacks[i];
						callback(this, &module_info);
					}

					for (DWORD i = 0; i < m_call_breakpoints.count; i++)
					{
						LPVOID call_breakpoint_address = (LPVOID)((DWORD)module_info.lpBaseOfDll + m_call_breakpoints[i].module_offset);

						read_debuggee_memory(call_breakpoint_address, &instructions, READ_PAGE_SIZE, &bytes_read);
						for (DWORD c = 0; c < bytes_read; c++)
						{
							if (instructions[c] == instructions::call_op)
							{
								offset = (DWORD)call_breakpoint_address + c;
								read_debuggee_memory((LPVOID)(offset + 1), &call_location, 4, &call_location_bytes_read);
								call_location += offset + 5;
								if (call_location < (DWORD)module_info.lpBaseOfDll || call_location >(DWORD)module_info.lpBaseOfDll + module_info.SizeOfImage)
									continue;

								if (breakpoints_set < m_call_breakpoints.get_max_count())
								{
									write_debuggee_memory((LPVOID)offset, &instructions::break_op, sizeof(instructions::break_op), &bytes_written);
									FlushInstructionCache(m_process->get_process_handle(), (LPVOID)offset, 1);

									breakpoints_set++;
									break;
								}
							}
						}
					}

					m_process->resume_thread();
				}
				else
				{
					m_thread_handle = OpenThread(THREAD_ALL_ACCESS, true, m_debug_event.dwThreadId);
					if (m_thread_handle != NULL)
					{
						context.ContextFlags = CONTEXT_ALL;
						GetThreadContext(m_thread_handle, &context);

						context.Eip--;
						context.EFlags |= 0x100;

						SetThreadContext(m_thread_handle, &context);
						CloseHandle(m_thread_handle);

						write_debuggee_memory((LPVOID)context.Eip, &instructions::call_op, sizeof(instructions::call_op), &bytes_written);
						FlushInstructionCache(m_process->get_process_handle(), (LPVOID)context.Eip, 1);

						last_call_location = context.Eip;
					}
				}

				first_break_has_occurred = true;
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

					c_registers* registers = new c_registers(&module_info, &context);

					if (last_call_location)
					{
						write_debuggee_memory((LPVOID)last_call_location, &instructions::break_op, sizeof(instructions::break_op), &bytes_written);
						FlushInstructionCache(m_process->get_process_handle(), (LPVOID)last_call_location, 1);

						//DWORD module_offset = /*0x00400000*/PE32BASE - (DWORD)module_info.lpBaseOfDll;

						decltype(s_breakpoint::callback) callback = nullptr;

						LPWSTR name = NULL;
						for (DWORD i = 0; i < m_call_breakpoints.get_max_count(); i++)
						{
							s_breakpoint& call_breakpoint = m_call_breakpoints[i];
							DWORD instruction_pointer_offset = context.Eip - (DWORD)module_info.lpBaseOfDll;
							DWORD last_call_location_offset = last_call_location - (DWORD)module_info.lpBaseOfDll;

							if (instruction_pointer_offset == call_breakpoint.module_offset || last_call_location_offset == call_breakpoint.module_offset)
							{
								callback = call_breakpoint.callback;
								name = call_breakpoint.name;
								break;
							}
						}

						printf("\n%ls\n  ", (name && *name) ? name : L"no name");
						printf("thread id: %08d, %ls+0x%08X: call %ls+0x%08X\n", m_debug_event.dwThreadId,
							m_process->get_name(), last_call_location - (DWORD)module_info.lpBaseOfDll,
							m_process->get_name(), context.Eip - (DWORD)module_info.lpBaseOfDll);

						DWORD stack_data_size = context.Ebp - context.Esp;
						unsigned char* stack_data = new unsigned char[stack_data_size] {};
						read_debuggee_memory((LPVOID)context.Esp, stack_data, stack_data_size, &bytes_read);

						printf("  registers:\n");
						printf("    destination         (edi): 0x%08X\n", context.Edi);
						printf("    source              (esi): 0x%08X\n", context.Esi);
						printf("    base                (ebx): 0x%08X\n", context.Ebx);
						printf("    data                (edx): 0x%08X\n", context.Edx);
						printf("    counter             (ecx): 0x%08X\n", context.Ecx);
						printf("    accumulator         (eax): 0x%08X\n", context.Eax);
						printf("    stack base pointer  (ebp): 0x%08X\n", context.Ebp);
						printf("    instruction pointer (eip): 0x%08X\n", context.Eip);
						printf("    stack pointer       (esp): 0x%08X\n", context.Esp);

						printf("  stack:\n");
						dump_to_console("    ", stack_data, stack_data_size, 64, 16);

						delete[] stack_data;
						stack_data = nullptr;

						last_call_location = 0;

						if (callback)
							callback(this, registers);
					}

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
			process_running = false;
			break;
		}
		case OUTPUT_DEBUG_STRING_EVENT:
		{
			OUTPUT_DEBUG_STRING_INFO& DebugString = m_debug_event.u.DebugString;

			if (DebugString.fUnicode)
			{
				WCHAR* buffer = new WCHAR[DebugString.nDebugStringLength * sizeof(WCHAR)]{};
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

					wprintf_s(L"[debug] %s\n", buffer);
				}

				delete[] buffer;
			}
			else
			{
				CHAR* buffer = new CHAR[DebugString.nDebugStringLength * sizeof(CHAR)]{};

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

					printf_s("[debug] %s\n", buffer);
				}

				delete[] buffer;
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

	m_process->close();
}


LPVOID c_debugger::allocate_debuggee_memory(
	_In_opt_ LPVOID lpAddress,
	_In_ SIZE_T dwSize,
	_In_ DWORD flAllocationType,
	_In_ DWORD flProtect
)
{
	return VirtualAllocEx(m_process->get_process_handle(), lpAddress, dwSize, flAllocationType, flProtect);
}

BOOL c_debugger::read_debuggee_memory(
	_In_ LPCVOID lpBaseAddress,
	_Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_opt_ SIZE_T* lpNumberOfBytesRead
)
{
	return ReadProcessMemory(m_process->get_process_handle(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
}

BOOL c_debugger::write_debuggee_memory(
	_In_ LPVOID lpBaseAddress,
	_In_reads_bytes_(nSize) LPCVOID lpBuffer,
	_In_ SIZE_T nSize,
	_Out_opt_ SIZE_T* lpNumberOfBytesWritten
)
{
	return WriteProcessMemory(m_process->get_process_handle(), lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesWritten);
}

BOOL c_debugger::write_debuggee_pointer(
	c_debugger* debugger,
	_In_ LPVOID lpBaseAddress,
	_In_ LPCVOID lpAddress,
	_Out_opt_ SIZE_T* lpNumberOfBytesWritten
)
{
	return debugger->write_debuggee_memory(lpBaseAddress, &lpAddress, sizeof(void*), lpNumberOfBytesWritten);
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
