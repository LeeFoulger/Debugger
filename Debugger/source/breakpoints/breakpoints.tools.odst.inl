#pragma once

void on_is_debugger_present_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_reference<bool> g_set_always_a_debugger_present(debugger);
	static c_remote_reference<bool> g_set_never_a_debugger_present(debugger);

	g_set_always_a_debugger_present.set_address(registers.get_runtime_addr(0x318A210));
	g_set_never_a_debugger_present.set_address(registers.get_runtime_addr(0x318A211));

	// forces `is_debugger_present()` to return true
	static bool set_always_a_debugger_present = false;

	// forces `is_debugger_present()` to return false
	static bool set_never_a_debugger_present = false;
	
	if (set_always_a_debugger_present != g_set_always_a_debugger_present)
		g_set_always_a_debugger_present = set_always_a_debugger_present;

	if (set_never_a_debugger_present != g_set_never_a_debugger_present)
		g_set_never_a_debugger_present = set_never_a_debugger_present;

	printf("");
}

void on_shell_screen_pause_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// g_shell_application_paused
	c_remote_reference<bool>(debugger, registers.get_runtime_addr(0x2C204B9)) = false;
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
	static c_remote_reference<c_string<char, 64 + 1>> name(debugger);
	static c_remote_reference<c_string<char, 64 + 1>> type(debugger);
	static SIZE_T size = 0;

	name.set_address(registers.get_raw_context().Rdx);
	type.set_address(registers.get_raw_context().R8);
	size = registers.get_raw_context().R9;

	static wchar_t filename[MAX_PATH]{};
	swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.txt", debugger.get_process().get_current_directory());

	static FILE* file = NULL;
	if (_wfopen_s(&file, filename, L"a+"), file != NULL)
	{
		fprintf(file, "0x%08zX", size);
		fprintf(file, ", %s", name());
		fprintf(file, ", %s", type());
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

void on_string_id_get_string_const_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_reference<size_t> g_string_id_globals_ascii_storage_address(debugger, registers.get_runtime_addr(0x318DDC0));
	static c_remote_reference<size_t> g_string_id_globals_ascii_strings_address(debugger, registers.get_runtime_addr(0x318DDD0));
	static c_remote_reference<size_t> g_string_id_globals_ascii_string_count(debugger, registers.get_runtime_addr(0x318DDD8));
	static c_remote_reference<char[0x340000]> g_string_id_globals_ascii_storage(debugger, g_string_id_globals_ascii_storage_address());
	static c_remote_reference<size_t[0x14000]> g_string_id_globals_ascii_strings(debugger, g_string_id_globals_ascii_strings_address());
	static size_t string_id_globals_ascii_strings[0x14000]{};

	RUNONCE(test, { return; });

	if (g_string_id_globals_ascii_storage_address() == g_string_id_globals_ascii_strings()[0])
	{
		if (string_id_globals_ascii_strings[1] != (g_string_id_globals_ascii_strings()[1] - g_string_id_globals_ascii_storage_address()))
		{
			for (size_t i = 0; i < g_string_id_globals_ascii_string_count(); i++)
			{
				if (g_string_id_globals_ascii_strings()[i])
					string_id_globals_ascii_strings[i] = (g_string_id_globals_ascii_strings()[i] - g_string_id_globals_ascii_storage_address());
			}
		}

		for (size_t i = 0; i < g_string_id_globals_ascii_string_count(); i++)
			printf("0x%08zX, 0x%08zX, %s\n", i, string_id_globals_ascii_strings[i], g_string_id_globals_ascii_storage() + string_id_globals_ascii_strings[i]);
	}

	printf("");
}