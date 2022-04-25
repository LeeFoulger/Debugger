#pragma once

void on_is_debugger_present_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// forces is_debugger_present() to return true
	static bool g_set_always_a_debugger_present = false;

	// forces is_debugger_present() to return false
	static bool g_set_never_a_debugger_present = false;

	bool set_always_a_debugger_present;
	bool set_never_a_debugger_present;
	debugger.read_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x318A210), &set_always_a_debugger_present, sizeof(set_always_a_debugger_present), NULL);
	debugger.read_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x318A211), &set_never_a_debugger_present, sizeof(set_never_a_debugger_present), NULL);

	if (set_always_a_debugger_present != g_set_always_a_debugger_present)
	{
		g_set_always_a_debugger_present = set_always_a_debugger_present;
		debugger.write_debuggee_memory(registers.get_runtime_addr_as<LPVOID>(0x318A210), &g_set_always_a_debugger_present, sizeof(g_set_always_a_debugger_present), NULL);
	}
	if (g_set_never_a_debugger_present != set_never_a_debugger_present)
	{
		g_set_never_a_debugger_present = set_never_a_debugger_present;
		debugger.write_debuggee_memory(registers.get_runtime_addr_as<LPVOID>(0x318A211), &g_set_never_a_debugger_present, sizeof(g_set_never_a_debugger_present), NULL);
	}

	printf("");
}

void on_shell_screen_pause_breakpoint(c_debugger& debugger, c_registers& registers)
{
	bool g_shell_application_paused = false;
	debugger.write_debuggee_memory(registers.get_runtime_addr_as<LPVOID>(0x2C204B9), &g_shell_application_paused, sizeof(g_shell_application_paused), NULL);
	printf("");
}


void on_shell_get_external_host_breakpoint(c_debugger& debugger, c_registers& registers)
{
	//SIZE_T return_address = 0;
	//SIZE_T call_address = -5;
	//debugger.read_debuggee_memory((LPCVOID)registers.cast_sp_as<SIZE_T>(), &return_address, sizeof(return_address), NULL);
	//call_address += return_address;

	//printf("%016zX\tcall shell_get_external_host\n", call_address);
	printf("");
}

void on_restricted_region_add_member_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static SIZE_T size = 0;
	size = registers.get_raw_context().R9;

	static char name[64 + 1]{};
	debugger.read_debuggee_memory((LPCVOID)(registers.get_raw_context().Rdx), name, 64, NULL);

	static char type[64 + 1]{};
	debugger.read_debuggee_memory((LPCVOID)(registers.get_raw_context().R8), type, 64, NULL);

	static wchar_t filename[MAX_PATH]{};
	swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.txt", debugger.get_process().get_current_directory());

	static FILE* file = NULL;
	if (_wfopen_s(&file, filename, L"a+"), file != NULL)
	{
		fprintf(file, "0x%08zX", size);
		fprintf(file, ", %s", name ? name : "(null)");
		fprintf(file, ", %s", type ? type : "(null)");
		fprintf(file, "\n");
		fclose(file);
	}

	printf("");
}

void on_shell_get_system_identifier_breakpoint(c_debugger& debugger, c_registers& registers)
{
	//	bool shell_get_system_identifier(
	//	    char *out_system_identifier,	// rcx
	//	    long system_identifier_length	// rdx
	//	);

	//	if (system_identifier_length <= 0)
	//		return 0;

	//	if (!shell_get_system_identifier(system_identifier_string, 160))
	//		csstrnzcpy(system_identifier_string, "unknown_system", 160);

	registers.get_raw_context().Xdx = 0;
	printf("");
}

static wchar_t g_gamertag[16];

void on_shell_get_gamertag_return_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static LPVOID allocated_gamertag = debugger.allocate_and_write_debuggee_memory(g_gamertag, sizeof(g_gamertag), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);

	// return value
	registers.get_raw_context().Xax = reinterpret_cast<decltype(registers.get_raw_context().Xax)>(allocated_gamertag);

	printf("");
}

// set gamertag here
//extern wchar_t g_gamertag[16] = L"gamertag";
