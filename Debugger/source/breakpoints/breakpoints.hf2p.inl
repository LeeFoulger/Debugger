#pragma once

#include <game/game_options.h>

const bool disable_saber_code_applied_in_scenario_load = false;

void on_command_line_get_credentials_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static LPVOID debuggee_command_line = debugger_allocate_and_write_debuggee_string(debugger, "--account 123 --sign-in-code 123 --environment 123");

	c_remote_reference<LPVOID>(debugger, registers.get_runtime_addr(0x052BE944 - PE32_BASE)) = debuggee_command_line;
	c_remote_reference<bool>(debugger, registers.get_runtime_addr(0x01346881 - PE32_BASE)) = !disable_saber_code_applied_in_scenario_load;
}

void on_restricted_region_add_member_internal_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_pointer<c_string<char, 128>> name(debugger);
	static c_remote_pointer<c_string<char, 128>> type(debugger);
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

void on_main_kick_startup_masking_sequence_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// .text:0049713F	cmp		[4BC1E48h],	al		<--- current breakpoint
	// .text:00497145	jz      short loc_4971CD	<--- current eip
	// .text:004971CD	mov		[42E1E49h],	1		<--- new eip

	const size_t jump_size = 0x004971CD - 0x00497145;
	registers.get_raw_context().Xip += jump_size;
}

void bink_format_patch(c_debugger& debugger, LPMODULEINFO module_info, bool use_breakpoint)
{
	//c_remote_reference<c_string<char, 32>>(debugger, ((size_t)module_info->lpBaseOfDll + (0x011992C8 - PE32_BASE))) = (c_string<char, 32>)"_ink\\%s.bik";

	if (use_breakpoint)
	{
		debugger.add_breakpoint(0x0049713F - PE32_BASE, false, "cmp [", L"main_kick_startup_masking_sequence: startup_sequence check result", on_main_kick_startup_masking_sequence_breakpoint);
	}
	else
	{
		size_t bink_format_offset = 0;
		static c_remote_reference<c_bytes<0x1000>> page(debugger);
		for (size_t page_offset = 0; page_offset < module_info->SizeOfImage; page_offset += sizeof(page()))
		{
			page.set_address((size_t)module_info->lpBaseOfDll + page_offset);

			// 'bink\%s.bik'
			size_t offset = find_pattern(page().value, "62 69 6E 6B 5C 25 73 2E 62 69 6B");
			if (offset != 'nope')
			{
				// main_status
				bink_format_offset = page_offset + offset;
				break;
			}
		}
		c_remote_reference<char>(debugger, (size_t)module_info->lpBaseOfDll + bink_format_offset) = '_';
	}
}

void language_patch(c_debugger& debugger, LPMODULEINFO module_info)
{
	static c_remote_reference<unsigned char> patch0(debugger);
	static c_remote_reference<unsigned long> patch1(debugger);

	size_t found_offset = 0;
	static c_remote_reference<c_bytes<0x1000>> page(debugger);
	for (size_t page_offset = 0; page_offset < module_info->SizeOfImage; page_offset += sizeof(page()))
	{
		page.set_address((size_t)module_info->lpBaseOfDll + page_offset);

		// cmp     g_game_language, 0Bh
		// jz      short loc_6B9263
		// mov     g_game_language, 0Bh
		size_t offset = find_pattern(page().value, "83 3D ?? ?? ?? ?? 0B 74 ?? ?? ?? ?? ?? ?? ?? 0B 00 00 00");
		if (offset != 'nope')
		{
			found_offset = page_offset + offset;
			break;
		}
	}

	if (!found_offset)
		return;

	patch0.set_address((size_t)module_info->lpBaseOfDll + found_offset + 0x6);
	patch1.set_address((size_t)module_info->lpBaseOfDll + found_offset + 0xF);

	if (patch0.get_address() && patch0() == 11) patch0 = 0;
	if (patch1.get_address() && patch1() == 11) patch1 = 0;

	printf("");
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

void on_main_game_load_map_breakpoint(c_debugger& debugger, c_registers& registers)
{
	static c_remote_reference<game_options> options(debugger);
	options.set_address(registers.cast_cx_as<size_t>());

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
		c_game_variant& game_variant = options().game_variant();

		options().game_mode = _game_mode_multiplayer;
		options().scenario_path(scenario_path);

		game_variant.m_game_engine_index = _game_engine_slayer_variant;

		game_variant.m_base_variant.m_miscellaneous_options.m_flags.set(_game_engine_miscellaneous_option_teams_enabled, false);
		game_variant.m_base_variant.m_miscellaneous_options.m_round_time_limit = 0;
		game_variant.m_slayer_variant.m_map_override_options.m_player_traits.m_appearance_traits.m_forced_change_color_setting = _forced_change_color_setting_extra1;

		game_variant.m_base_variant.m_respawn_options.m_respawn_time = 0;
		game_variant.m_base_variant.m_respawn_options.m_suicide_penalty = 0;

		game_variant.m_base_variant.m_map_override_options.m_flags.set(_game_engine_map_override_options_grenades_on_map, true);
		game_variant.m_base_variant.m_map_override_options.m_flags.set(_game_engine_map_override_options_indestructible_vehicles, true);
		game_variant.m_base_variant.m_map_override_options.m_player_traits.m_weapon_traits.m_initial_grenade_count = _grenade_count_setting_map_default;
		game_variant.m_base_variant.m_map_override_options.m_player_traits.m_weapon_traits.m_recharging_grenades = _recharging_grenades_setting_enabled;
		game_variant.m_base_variant.m_map_override_options.m_player_traits.m_weapon_traits.m_infinite_ammo = _infinite_ammo_setting_bottomless_clip;
		game_variant.m_base_variant.m_map_override_options.m_player_traits.m_weapon_traits.m_weapon_pickup = _weapon_pickup_setting_allowed;

		if (game_variant.m_game_engine_index == _game_engine_slayer_variant)
		{
			game_variant.m_slayer_variant.m_score_to_win = 50;
			game_variant.m_slayer_variant.m_suicide_points = 10;
		}

		options = options();
	}
}
