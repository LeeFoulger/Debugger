#pragma once

const bool disable_saber_code_applied_in_scenario_load = false;

void on_command_line_get_credentials_breakpoint(c_debugger& debugger, c_registers& registers)
{
	LPVOID debuggee_command_line = debugger_allocate_and_write_debuggee_string(debugger, "--account 123 --sign-in-code 123 --environment 123");
	debugger.write_debuggee_pointer(registers.get_runtime_addr_as<LPVOID>(0x052BE944 - PE32BASE), debuggee_command_line, NULL);

	static char* data[0x375F0]{};
	if (*data == 0)
	{
		debugger.read_debuggee_memory(registers.get_runtime_addr_as<LPVOID>(0x043E1408 - PE32BASE), data, 0x375F0, NULL);
	}

	printf("");

	if (disable_saber_code_applied_in_scenario_load)
	{
		debugger_write_array(debugger, registers.get_runtime_addr_as<LPVOID>(0x01346881 - PE32BASE), unsigned char, { 0x00 });
	}

	printf("");
}

void on_restricted_region_add_member_internal_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static SIZE_T size = 0;
	debugger.read_debuggee_memory((LPCVOID)(registers.cast_bp_as<SIZE_T>(0x10)), &size, 4, NULL);

	static char name[64 + 1]{};
	debugger.read_debuggee_pointer((LPCVOID)(registers.cast_bp_as<SIZE_T>(0x8)), name, 64, NULL);

	static char type[64 + 1]{};
	debugger.read_debuggee_pointer((LPCVOID)(registers.cast_bp_as<SIZE_T>(0xC)), type, 64, NULL);

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
}

void on_rasterizer_draw_watermark_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_string<wchar_t, 1024> watermark_old{};
	if (*watermark_old == 0)
	{
		SIZE_T old_watermark_ptr = 0;
		debugger.read_debuggee_pointer((LPCVOID)(registers.cast_sp_as<SIZE_T>(0x8)), watermark_old, 1024 * sizeof(wchar_t), NULL);
	}

	static c_string<wchar_t, 1024> watermark{};
	static LPVOID watermark_addr = debugger_allocate_and_write_debuggee_string(debugger, watermark);

	if (*watermark == 0)
	{
		swprintf_s(watermark, L"DEBUGGER ATTACHED");
		debugger_write_debuggee_string(debugger, watermark_addr, watermark);
	}

	debugger.write_debuggee_pointer((LPVOID)(registers.cast_sp_as<SIZE_T>(0x8)), watermark_addr, NULL);
}

void on_machinima_camera_debug_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:004E2A3F	test    al, al			<--- current breakpoint
	// .text:004E2A41	jz      loc_4E2C7B		<--- current eip
	// .text:004E2A47	mov     eax, [esi+84h]	<--- new eip
	registers.get_raw_context().Xip += (0x004E2A47 - 0x004E2A41);
}

void on_cache_file_blocking_read_breakpoint(c_debugger& debugger, c_registers& registers)
{
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x042DDCD0 - PE32BASE), 0x000034F0, L"bin\\cache_file_tag_globals.bin");
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x043E1408 - PE32BASE), 0x000375F0, L"bin\\cache_file_table_of_contents.bin");
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x044189F8 - PE32BASE), 0x000036E8, L"bin\\cache_file_copy_globals.bin");

	printf("");
}

void on_cache_files_verify_header_rsa_signature_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:00482DB2	test    al, al			<--- current breakpoint
	// .text:00482DB4	jz      loc_482DD2		<--- current eip
	// .text:00482DD2	mov     al, 1			<--- new eip
	registers.get_raw_context().Xip += (0x00482DD2 - 0x00482DB4);
}

void on_cached_map_files_open_all_breakpoint(c_debugger& debugger, c_registers& registers)
{
	SIZE_T resource_paths_offset = 0x012ECEA4 - PE32BASE;
	char resource_paths[7][32] = {
		"test\\resources.dat",
		"test\\textures.dat",
		"test\\textures_b.dat",
		"test\\sounds.dat",
		"test\\video.dat",
		"test\\render_models.dat",
		"test\\lightmaps.dat"
	};

	LPVOID allocated_resource_paths = debugger.allocate_and_write_debuggee_memory(resource_paths, sizeof(resource_paths), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);

	for (size_t i = 0; i < sizeof(resource_paths) / 32; i++)
	{
		LPVOID resource_path_offset_address = registers.get_runtime_addr_as<LPVOID>(resource_paths_offset + i * 4);
		LPVOID allocated_resource_path_address = reinterpret_cast<LPVOID>(reinterpret_cast<SIZE_T>(allocated_resource_paths) + i * 32);
		debugger.write_debuggee_pointer(resource_path_offset_address, allocated_resource_path_address, NULL);
	}

	printf("");
}

enum e_game_mode : unsigned long
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mapeditor,
	_game_mode_savefilm,
	_game_mode_survival,

	k_game_mode_count,
};

enum e_game_engine_variant : unsigned long
{
	_game_engine_base_variant = 0,
	_game_engine_ctf_variant,
	_game_engine_slayer_variant,
	_game_engine_oddball_variant,
	_game_engine_king_variant,
	_game_engine_sandbox_variant,
	_game_engine_vip_variant,
	_game_engine_juggernaut_variant,
	_game_engine_territories_variant,
	_game_engine_assault_variant,
	_game_engine_infection_variant,

	k_game_engine_variant_count,
};

void on_main_game_load_map_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static char game_options[0xE620]{};
	memset(game_options, 0, sizeof(game_options));

	char scenario_path[MAX_PATH]{};
	csstrncpy(scenario_path, MAX_PATH, "default", MAX_PATH);

	{
		wchar_t filename[MAX_PATH]{};
		swprintf_s(filename, MAX_PATH, L"%s\\test\\scenario_path.txt", debugger.get_process().get_current_directory());

		FILE* file = NULL;
		if (_wfopen_s(&file, filename, L"r"), file != NULL)
		{
			fgets(scenario_path, MAX_PATH, file);
			fclose(file);
		}

		if (*scenario_path == 0 || *scenario_path == '\r' || *scenario_path == '\n')
		{
			fputs("enter scenario path: ", stdout);
			fgets(scenario_path, MAX_PATH, stdin);
		}

		if (*scenario_path)
		{
			while (scenario_path[strlen(scenario_path) - 1] == '\r' || scenario_path[strlen(scenario_path) - 1] == '\n')
				scenario_path[strlen(scenario_path) - 1] = 0;
		}
	}

	if (*scenario_path && strcmp(scenario_path, "default") != 0)
	{
		debugger.read_debuggee_memory(registers.cast_cx_as<LPCVOID>(), game_options, sizeof(game_options), NULL);

		// game mode: multiplayer
		*reinterpret_cast<unsigned long*>(game_options) = _game_mode_multiplayer;

		// scenario path: scenario_path
		csstrncpy(game_options + 0x24, MAX_PATH, scenario_path, MAX_PATH);

		// game engine: slayer
		*reinterpret_cast<unsigned long*>(game_options + 0x32C) = _game_engine_slayer_variant;

		debugger_write_data(debugger, registers.cast_cx_as<LPVOID>(), game_options);
	}
}
