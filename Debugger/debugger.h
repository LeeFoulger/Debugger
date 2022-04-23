#pragma once

#define READ_PAGE_SIZE 4096

#ifdef _WIN64
#define Xdi Rdi
#define Xsi Rsi
#define Xbx Rbx
#define Xdx Rdx
#define Xcx Rcx
#define Xax Rax
#define Xbp Rbp
#define Xip Rip
#define Xsp Rsp
#else
#define Xdi Edi
#define Xsi Esi
#define Xbx Ebx
#define Xdx Edx
#define Xcx Ecx
#define Xax Eax
#define Xbp Ebp
#define Xip Eip
#define Xsp Esp
#endif // WIN64

enum e_instruction : BYTE
{
	_instruction_call = 0xE8,
	_instruction_break = 0xCC
};

template<typename t_type, SIZE_T k_max_count>
struct s_static_array
{
	SIZE_T count;
	t_type data[k_max_count];

	t_type& operator[](SIZE_T index) { return data[index]; }

	constexpr SIZE_T get_max_count() { return k_max_count; };
};

class c_debugger;
class c_registers;
struct s_breakpoint
{
	BYTE break_on;
	bool print_registers;
	SIZE_T module_offset;
	WCHAR name[64];
	void(*callback)(class c_debugger&, class c_registers&);
};

class c_debugger
{
public:
	c_debugger(c_process* process);
	~c_debugger();

	void attach();
	void detach();

	void add_breakpoint(BYTE, SIZE_T, const wchar_t*, bool, void(*callback)(c_debugger&, class c_registers&) = nullptr);

	void add_module_info_callback(const char*, void(*callback)(c_debugger&, LPMODULEINFO));

	c_process& get_process();

	void run_debugger(bool print_debug_strings = true);

	LPVOID allocate_debuggee_memory(
		_In_opt_ LPVOID lpAddress,
		_In_ SIZE_T dwSize,
		_In_ DWORD flAllocationType,
		_In_ DWORD flProtect
	);
	BOOL protect_debuggee_memory(
		_In_ LPVOID lpAddress,
		_In_ SIZE_T dwSize,
		_In_ DWORD flNewProtect,
		_Out_ PDWORD lpflOldProtect
	);
	SIZE_T query_debuggee_memory(
		_In_opt_ LPCVOID lpAddress,
		_Out_writes_bytes_to_(dwLength, return) PMEMORY_BASIC_INFORMATION lpBuffer,
		_In_ SIZE_T dwLength
	);
	BOOL read_debuggee_memory(
		_In_ LPCVOID lpBaseAddress,
		_Out_writes_bytes_to_(nSize, *lpNumberOfBytesRead) LPVOID lpBuffer,
		_In_ SIZE_T nSize,
		_Out_opt_ SIZE_T* lpNumberOfBytesRead
	);
	BOOL read_debuggee_pointer(
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
	BOOL dump_debuggee_memory(
		_In_ LPVOID lpAddress,
		_In_ SIZE_T dwSize,
		_In_ LPCWSTR lpFileName
	);

protected:
	c_process& m_process;
	bool m_process_is_attached;

	DEBUG_EVENT m_debug_event;
	DWORD m_continue_status;
	HANDLE m_thread_handle;
	bool m_print_debug_strings;

	s_static_array<s_breakpoint, 256> m_breakpoints;
	s_static_array<void(*)(c_debugger&, LPMODULEINFO), 32> m_module_info_callbacks;
};

template<typename t_string_type, size_t k_string_size>
LPVOID debugger_allocate_and_write_debuggee_string(
	c_debugger& debugger,
	t_string_type(&string)[k_string_size]
)
{
	long string_size_in_bytes = k_string_size * sizeof(t_string_type);
	return debugger.allocate_and_write_debuggee_memory(string, string_size_in_bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);
}

template<typename t_string_type, size_t k_string_size>
BOOL debugger_write_debuggee_string(
	c_debugger& debugger,
	LPVOID lpAddress,
	t_string_type(&string)[k_string_size]
)
{
	long string_size_in_bytes = k_string_size * sizeof(t_string_type);
	return debugger.write_debuggee_memory(lpAddress, string, string_size_in_bytes, NULL);
}

class c_registers
{
public:
	c_registers(LPMODULEINFO module_info, CONTEXT& context) :
		module_info(*module_info),
		context(context),

#ifdef _WIN64
		static_base_addr(PE64BASE),
#else
		static_base_addr(PE32BASE),
#endif // WIN64
		runtime_base_addr(reinterpret_cast<SIZE_T>(module_info->lpBaseOfDll))
	{
	}

	// destination
	template<typename t_type>
	t_type cast_di_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xdi);
	}

	// source
	template<typename t_type>
	t_type cast_si_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xsi);
	}

	// base
	template<typename t_type>
	t_type cast_bx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xbx);
	}

	// data
	template<typename t_type>
	t_type cast_dx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xdx);
	}

	// counter
	template<typename t_type>
	t_type cast_cx_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xcx);
	}

	// accumulator
	template<typename t_type>
	t_type cast_ax_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xax);
	}

	// stack base pointer
	template<typename t_type>
	t_type cast_bp_as(SIZE_T offset = 0, bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xbp) + offset;
	}

	// instruction pointer
	template<typename t_type>
	t_type cast_ip_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xip);
	}

	// stack pointer
	template<typename t_type>
	t_type cast_sp_as(SIZE_T offset = 0, bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.Xsp) + offset;
	}

#ifdef _WIN64
	template<typename t_type>
	t_type* cast_r8_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R8);
	}

	template<typename t_type>
	t_type* cast_r9_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R9);
	}

	template<typename t_type>
	t_type* cast_r10_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R10);
	}

	template<typename t_type>
	t_type* cast_r11_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R11);
	}

	template<typename t_type>
	t_type* cast_r12_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R12);
	}

	template<typename t_type>
	t_type* cast_r13_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R13);
	}

	template<typename t_type>
	t_type* cast_r14_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R14);
	}

	template<typename t_type>
	t_type* cast_r15_as(bool static_addr = false)
	{
		return *reinterpret_cast<t_type*>((static_addr ? static_base_addr - runtime_base_addr : 0) + &context.R15);
	}
#endif // _WIN64

	SIZE_T get_stack_size()
	{
		return context.Xbp - context.Xsp;
	}

	template<typename t_type>
	t_type* get_runtime_addr_as(SIZE_T offset = 0)
	{
		return reinterpret_cast<t_type*>(runtime_base_addr + offset);
	}

	CONTEXT& get_raw_context()
	{
		return context;
	}

protected:

	MODULEINFO& module_info;
	CONTEXT& context;
	SIZE_T static_base_addr;
	SIZE_T runtime_base_addr;
};
