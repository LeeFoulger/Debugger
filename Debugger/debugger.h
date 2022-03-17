#pragma once

#define READ_PAGE_SIZE 4096

enum e_instruction : BYTE
{
	_instruction_call = 0xE8,
	_instruction_break = 0xCC
};

template<typename t_type, DWORD k_max_count>
struct s_static_array
{
	DWORD count;
	t_type data[k_max_count];

	t_type& operator[](DWORD index) { return data[index]; }

	constexpr DWORD get_max_count() { return k_max_count; };
};

class c_debugger;
class c_registers;
struct s_breakpoint
{
	BYTE break_on;
	bool print_registers;
	DWORD module_offset;
	WCHAR name[64];
	void(*callback)(class c_debugger*, class c_registers*);
};

class c_debugger
{
public:
	c_debugger(c_process* process);

	void run_debugger(bool print_debug_strings = true);

	void add_breakpoint(BYTE, DWORD, const wchar_t*, bool, void(*callback)(c_debugger*, class c_registers*) = nullptr);

	void add_module_info_callback(void(*callback)(c_debugger*, LPMODULEINFO));

	c_process* get_process();

	LPVOID allocate_debuggee_memory(
		_In_opt_ LPVOID lpAddress,
		_In_ SIZE_T dwSize,
		_In_ DWORD flAllocationType,
		_In_ DWORD flProtect
	);
	BOOL read_debuggee_memory(
		_In_ LPCVOID lpBaseAddress,
		_Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
		_In_ SIZE_T nSize,
		_Out_opt_ SIZE_T* lpNumberOfBytesRead
	);
	BOOL write_debuggee_memory(
		_In_ LPVOID lpBaseAddress,
		_In_reads_bytes_(nSize) LPCVOID lpBuffer,
		_In_ SIZE_T nSize,
		_Out_opt_ SIZE_T* lpNumberOfBytesWritten
	);
	BOOL write_debuggee_pointer(
		c_debugger* debugger,
		_In_ LPVOID lpBaseAddress,
		_In_ LPCVOID lpAddress,
		_Out_opt_ SIZE_T* lpNumberOfBytesWritten
	);
	LPVOID allocate_and_write_debuggee_memory(
		_In_reads_bytes_(nSize) LPCVOID lpBuffer,
		_In_ SIZE_T nSize,
		_In_ DWORD flAllocationType,
		_In_ DWORD flProtect,
		_Out_opt_ SIZE_T* lpNumberOfBytesWritten
	);

protected:
	c_process* m_process;

	s_static_array<s_breakpoint, 256> m_breakpoints;
	s_static_array<void(*)(c_debugger*, LPMODULEINFO), 32> m_module_info_callbacks;

	DEBUG_EVENT m_debug_event;
	DWORD m_continue_status;

	HANDLE m_thread_handle;
	bool m_print_debug_strings;
};

template<typename t_string_type, size_t k_string_size>
LPVOID debugger_write_debuggee_string(
	c_debugger* debugger,
	t_string_type(&string)[k_string_size]
)
{
	long string_size_in_bytes = k_string_size * sizeof(t_string_type);
	return debugger->allocate_and_write_debuggee_memory(string, string_size_in_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);
}

class c_registers
{
public:
	c_registers(LPMODULEINFO module_info, LPCONTEXT context) :
		module_info(*module_info),
		context(*context),
		static_base_addr(PE32BASE),
		runtime_base_addr(reinterpret_cast<DWORD>(module_info->lpBaseOfDll))
	{
	}

	// destination
	template<typename t_type>
	t_type cast_edi_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Edi);
	}

	// source
	template<typename t_type>
	t_type cast_esi_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Esi);
	}

	// base
	template<typename t_type>
	t_type cast_ebx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Ebx);
	}

	// data
	template<typename t_type>
	t_type cast_edx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Edx);
	}

	// counter
	template<typename t_type>
	t_type cast_ecx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Ecx);
	}

	// accumulator
	template<typename t_type>
	t_type cast_eax_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Eax);
	}

	// stack base pointer
	template<typename t_type>
	t_type cast_ebp_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Ebp);
	}

	// instruction pointer
	template<typename t_type>
	t_type cast_eip_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Eip);
	}

	// stack pointer
	template<typename t_type>
	t_type cast_esp_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Esp);
	}

	template<typename t_type>
	t_type* get_runtime_addr_as(DWORD offset = 0)
	{
		return reinterpret_cast<t_type*>(runtime_base_addr + offset);
	}

protected:

	MODULEINFO& module_info;
	CONTEXT& context;
	DWORD static_base_addr;
	DWORD runtime_base_addr;
};
