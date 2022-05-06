#pragma once

const bool disable_saber_code_applied_in_scenario_load = false;

void on_command_line_get_credentials_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static LPVOID debuggee_command_line = debugger_allocate_and_write_debuggee_string(debugger, "--account 123 --sign-in-code 123 --environment 123");

	c_remote_reference<LPVOID>(debugger, registers.get_runtime_addr(0x052BE944 - PE32_BASE)) = debuggee_command_line;
	c_remote_reference<bool>(debugger, registers.get_runtime_addr(0x01346881 - PE32_BASE)) = !disable_saber_code_applied_in_scenario_load;
}

void on_restricted_region_add_member_internal_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_pointer<c_string<char, 64 + 1>> name(debugger);
	static c_remote_pointer<c_string<char, 64 + 1>> type(debugger);
	static c_remote_reference<size_t> size(debugger);

	name.set_address(registers.cast_bp_as<size_t>(0x08));
	type.set_address(registers.cast_bp_as<size_t>(0x0C));
	size.set_address(registers.cast_bp_as<size_t>(0x10));

	static c_string<wchar_t, MAX_PATH> filename{};
	swprintf_s(filename.value, MAX_PATH, L"%s\\bin\\globals.csv", debugger.get_process().get_current_directory());

	static FILE* file = NULL;
	if (_wfopen_s(&file, filename.value, L"a+"), file != NULL)
	{
		fprintf(file, "0x%08zX", size());
		fprintf(file, ", %s", type().value);
		fprintf(file, ", %s", name().value);
		fprintf(file, "\n");
		fclose(file);
	}
}

void on_rasterizer_draw_watermark_breakpoint(c_debugger& debugger, c_registers& registers)
{
	//static c_remote_pointer<c_string<wchar_t, 1024>> watermark_old(debugger);
	//watermark_old.set_address(registers.cast_sp_as<size_t>(0x8));

	static c_string<wchar_t, 1024> watermark{};
	static LPVOID watermark_addr = debugger_allocate_and_write_debuggee_string(debugger, watermark.value);

	if (*watermark.value == 0)
	{
		swprintf_s(watermark.value, L"DEBUGGER ATTACHED");
		debugger_write_debuggee_string(debugger, watermark_addr, watermark.value);
	}

	c_remote_reference<LPVOID>(debugger, registers.cast_sp_as<size_t>(0x8)) = watermark_addr;
}

void on_machinima_camera_debug_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:004E2A3F	test    al, al			<--- current breakpoint
	// .text:004E2A41	jz      loc_4E2C7B		<--- current eip
	// .text:004E2A47	mov     eax, [esi+84h]	<--- new eip

	const size_t jump_size = 0x004E2A47 - 0x004E2A41;
	registers.get_raw_context().Xip += jump_size;
}

void on_cache_file_blocking_read_breakpoint(c_debugger& debugger, c_registers& registers)
{
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x042DDCD0 - PE32_BASE), 0x000034F0, L"bin\\cache_file_tag_globals.bin");
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x043E1408 - PE32_BASE), 0x000375F0, L"bin\\cache_file_table_of_contents.bin");
	debugger.dump_debuggee_memory(registers.get_runtime_addr_as<LPCVOID>(0x044189F8 - PE32_BASE), 0x000036E8, L"bin\\cache_file_copy_globals.bin");

	printf("");
}

void on_cache_files_verify_header_rsa_signature_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:00482DB2	test    al, al			<--- current breakpoint
	// .text:00482DB4	jz      loc_482DD2		<--- current eip
	// .text:00482DD2	mov     al, 1			<--- new eip

	const size_t jump_size = 0x00482DD2 - 0x00482DB4;
	registers.get_raw_context().Xip += jump_size;
}

void on_contrails_render_callback_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:005D6B76	pop     ebp				<--- current breakpoint
	// .text:005D6B77	jmp     loc_5D6B80		<--- current eip
	// .text:005D6CB9	ret						<--- new eip

	const size_t jump_size = 0x005D6CB9 - 0x005D6B77;
	registers.get_raw_context().Xip += jump_size;
}

void on_cached_map_files_open_all_breakpoint(c_debugger& debugger, c_registers& registers)
{
	const size_t resource_path_count = 7;
	const size_t resource_path_length = 32;

	size_t resource_path_offset_address = registers.get_runtime_addr(0x012ECEA4 - PE32_BASE);
	static c_string<char, resource_path_length> resource_paths[resource_path_count] = {
		"test\\resources.dat",
		"test\\textures.dat",
		"test\\textures_b.dat",
		"test\\sounds.dat",
		"test\\video.dat",
		"test\\render_models.dat",
		"test\\lightmaps.dat"
	};

	static size_t allocated_resource_paths = (size_t)debugger.allocate_and_write_debuggee_memory(resource_paths, sizeof(resource_paths), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);
	static c_remote_reference<decltype(allocated_resource_paths)> resource_path_offset(debugger);

	for (size_t i = 0; i < resource_path_count; i++)
	{
		resource_path_offset.set_address(resource_path_offset_address + (i * sizeof(void*)));
		resource_path_offset = allocated_resource_paths + (i * resource_path_length);
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

struct s_game_options
{
	char __data[0xE620];

	void game_mode(e_game_mode game_mode)
	{
		*reinterpret_cast<e_game_mode*>(__data) = game_mode;
	}

	void scenario_path(c_string<char, MAX_PATH> scenario_path)
	{
		csstrncpy(__data + 0x24, MAX_PATH, scenario_path.value, MAX_PATH);
	}

	void game_engine_variant(e_game_engine_variant game_engine_variant)
	{
		*reinterpret_cast<e_game_engine_variant*>(__data + 0x32C) = game_engine_variant;
	}
};

void on_main_game_load_map_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_reference<s_game_options> game_options(debugger);
	game_options.set_address(registers.cast_cx_as<size_t>());

	static c_string<char, MAX_PATH> scenario_path{};

	csstrncpy(scenario_path.value, MAX_PATH, "default", MAX_PATH);

	if (strcmp(scenario_path.value, "default") == 0)
	{
		wchar_t filename[MAX_PATH]{};
		swprintf_s(filename, MAX_PATH, L"%s\\test\\scenario_path.txt", debugger.get_process().get_current_directory());
	
		FILE* file = NULL;
		if (_wfopen_s(&file, filename, L"r"), file != NULL)
		{
			fgets(scenario_path.value, MAX_PATH, file);
			fclose(file);
		}
	
		if (*scenario_path.value == 0 || *scenario_path.value == '\r' || *scenario_path.value == '\n')
		{
			fputs("enter scenario path: ", stdout);
			fgets(scenario_path.value, MAX_PATH, stdin);
		}
	
		if (*scenario_path.value)
		{
			while (scenario_path.value[strlen(scenario_path.value) - 1] == '\r' || scenario_path.value[strlen(scenario_path.value) - 1] == '\n')
				scenario_path.value[strlen(scenario_path.value) - 1] = 0;
		}
	}

	if (*scenario_path.value && strcmp(scenario_path.value, "default") != 0)
	{
		game_options().game_mode(_game_mode_multiplayer);
		game_options().scenario_path(scenario_path);
		game_options().game_engine_variant(_game_engine_slayer_variant);

		game_options = game_options();
	}
}
