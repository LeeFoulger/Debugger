#include "main.h"

int wmain(int argc, wchar_t* argv[])
{
	//  " --account 123 --sign-in-code 123 --environment 123"
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	c_process* process = new c_process();

	if (!process->open(argv[1]))
		process->create(CREATE_SUSPENDED, L"%s", argv[1]);

	if (!process->is_thread_suspended())
		process->suspend_thread();

	c_debugger* debugger = new c_debugger(process);

	if (argc < 3 && wcscmp(process->get_name(), L"halo_online.exe") == 0)
		debugger->add_breakpoint(_instruction_call, 0x0075227E - PE32BASE, L"command_line_get_credentials", false, on_command_line_get_credentials_breakpoint);

	debugger->add_module_info_callback(add_break_on_winmain);
	debugger->add_module_info_callback(add_breaks_following_winmain);
	debugger->add_module_info_callback(add_test_breaks);

	debugger->run_debugger();

	delete debugger;
	delete process;

	return 0;
}

const bool disable_saber_code_applied_in_scenario_load = false;

void on_command_line_get_credentials_breakpoint(c_debugger* debugger, c_registers* registers)
{
	LPVOID debuggee_command_line = debugger_allocate_and_write_debuggee_string(debugger, "--account 123 --sign-in-code 123 --environment 123");
	debugger->write_debuggee_pointer(registers->get_runtime_addr_as<LPVOID>(0x052BE944 - PE32BASE), debuggee_command_line, NULL);

	static char* data[0x375F0] {};
	if (*data == 0)
	{
		debugger->read_debuggee_memory(registers->get_runtime_addr_as<LPVOID>(0x043E1408 - PE32BASE), data, 0x375F0, NULL);
	}

	printf("");

	if (disable_saber_code_applied_in_scenario_load)
	{
		unsigned char patch_data_bytes[] = { 0x00 };
		debugger->write_debuggee_memory(registers->get_runtime_addr_as<LPVOID>(0x01346881 - PE32BASE), patch_data_bytes, sizeof(patch_data_bytes), NULL);
	}

	printf("");
}

void add_test_breaks(c_debugger* debugger, LPMODULEINFO module_info)
{
	if (wcscmp(debugger->get_process()->get_name(), L"atlas_tag_test.exe") == 0)
	{
		debugger->add_breakpoint(0x40, 0x00000001403916E0 - PE64BASE, L"is_debugger_present", false, on_is_debugger_present_breakpoint);
		debugger->add_breakpoint(0xC2, 0x00000001401A9A60 - PE64BASE, L"shell_screen_pause", false, on_shell_screen_pause_breakpoint);
		debugger->add_breakpoint(0x33, 0x00000001401A9540 - PE64BASE, L"shell_get_external_host", false, on_shell_get_external_host_breakpoint);
	}
	else if (wcscmp(debugger->get_process()->get_name(), L"halo_online.exe") == 0)
	{
		{
			wchar_t filename[MAX_PATH]{};
			swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.txt", debugger->get_process()->get_current_directory());

			FILE* file = NULL;
			if (_wfopen_s(&file, filename, L"w"), file != NULL)
				fclose(file);
		}

		debugger->add_breakpoint(0xFF, 0x0056918C - PE32BASE, L"restricted_region_add_member::internal", false, on_restricted_region_add_member_internal_breakpoint);
		debugger->add_breakpoint(_instruction_call, 0x005B103C - PE32BASE, L"rasterizer_draw_watermark", false, on_rasterizer_draw_watermark_breakpoint);

		//debugger->add_breakpoint(_instruction_call, 0x00496EEE - PE32BASE, L"main_status->memset", true, [](c_debugger* debugger, c_registers* registers) -> void {
		//	DWORD data_size = registers->cast_esi_as<DWORD>();
		//	char* data = new char[data_size + 1] {};
		//
		//	debugger->read_debuggee_memory(registers->cast_edi_as<LPCVOID>(), data, data_size, NULL);
		//
		//	printf("[main_status] %s\n", data);
		//	delete[] data;
		//});
	}
}

void on_restricted_region_add_member_internal_breakpoint(c_debugger* debugger, c_registers* registers)
{
	static SIZE_T size = 0;
	debugger->read_debuggee_memory((LPCVOID)(registers->cast_bp_as<SIZE_T>(0x10)), &size, 4, NULL);

	static char name0[64 + 1] {};
	debugger->read_debuggee_pointer((LPCVOID)(registers->cast_bp_as<SIZE_T>(0x8)), name0, 64, NULL);

	static char name1[64 + 1] {};
	debugger->read_debuggee_pointer((LPCVOID)(registers->cast_bp_as<SIZE_T>(0xC)), name1, 64, NULL);

	static wchar_t filename[MAX_PATH] {};
	swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.txt", debugger->get_process()->get_current_directory());

	static FILE* file = NULL;
	if (_wfopen_s(&file, filename, L"a+"), file != NULL)
	{
		fprintf(file, "size: 0x%08zX", size);
		if (*name0)
			fprintf(file, ", %s", name0);

		if (*name1)
			fprintf(file, " | %s", name1);

		fprintf(file, "\n");

		fclose(file);
	}
}

void on_rasterizer_draw_watermark_breakpoint(c_debugger* debugger, c_registers* registers)
{
	static c_string<wchar_t, 1024> watermark_old{};
	if (*watermark_old == 0)
	{
		SIZE_T old_watermark_ptr = 0;
		debugger->read_debuggee_pointer((LPCVOID)(registers->cast_sp_as<SIZE_T>(0x8)), watermark_old, 1024 * sizeof(wchar_t), NULL);
	}

	static c_string<wchar_t, 1024> watermark{};
	static LPVOID watermark_addr = debugger_allocate_and_write_debuggee_string(debugger, watermark);

	if (*watermark == 0)
	{
		swprintf_s(watermark, L"DEBUGGER ATTACHED");
		debugger_write_debuggee_string(debugger, watermark_addr, watermark);
	}

	debugger->write_debuggee_pointer((LPVOID)(registers->cast_sp_as<SIZE_T>(0x8)), watermark_addr, NULL);
}

void add_breaks_following_winmain(c_debugger* debugger, LPMODULEINFO module_info)
{
	if (wcscmp(debugger->get_process()->get_name(), L"halo_online.exe") == 0)
	{
		// EntryPoint->WinMain
		debugger->add_breakpoint(_instruction_call, 0x004013C3 - PE32BASE, L"static_string<64>::print", true);
		debugger->add_breakpoint(_instruction_call, 0x00401501 - PE32BASE, L"shell_initialize", true);
		debugger->add_breakpoint(_instruction_call, 0x0040151C - PE32BASE, L"main_loop", true);
		debugger->add_breakpoint(_instruction_call, 0x0040152F - PE32BASE, L"shell_dispose", true);

		// shell_initialize->cache_files_initialize
		debugger->add_breakpoint(_instruction_call, 0x004ED644 - PE32BASE, L"cached_map_files_open_all", true, on_cached_map_files_open_all_breakpoint);

		// WinMain->main_loop
		debugger->add_breakpoint(_instruction_call, 0x004A9640 - PE32BASE, L"main_game_load_map", true, on_main_game_load_map_breakpoint);
		debugger->add_breakpoint(_instruction_call, 0x004A9670 - PE32BASE, L"main_game_start", true);

		// main_loop->main_game_load_map
		debugger->add_breakpoint(_instruction_call, 0x004A9B96 - PE32BASE, L"scenario_load", true);
		debugger->add_breakpoint(_instruction_call, 0x004A9BA4 - PE32BASE, L"main_game_internal_map_load_complete", true);

		// main_game_load_map->scenario_load
		debugger->add_breakpoint(_instruction_call, 0x0047E8EC - PE32BASE, L"scenario_tags_load", true);
		debugger->add_breakpoint(_instruction_call, 0x0047E973 - PE32BASE, L"scenario_tags_fixup", true);

		// scenario_load->scenario_tags_load
		debugger->add_breakpoint(_instruction_call, 0x00483843 - PE32BASE, L"cache_file_open", true);
		debugger->add_breakpoint(_instruction_call, 0x00483856 - PE32BASE, L"cache_file_header_verify", true);
		debugger->add_breakpoint(_instruction_call, 0x0048389C - PE32BASE, L"tags_file_open", true);
	}
}

void add_break_on_winmain(c_debugger* debugger, LPMODULEINFO module_info)
{
	if (!debugger || !module_info)
		return;

	c_process* process = debugger->get_process();
	if (!process)
		return;

	HANDLE process_handle = process->get_process_handle();
	LPVOID entry_point = module_info->EntryPoint;
	LPVOID image_base = module_info->lpBaseOfDll;
	SIZE_T image_size = module_info->SizeOfImage;

	SIZE_T entry_point_addr = reinterpret_cast<SIZE_T>(module_info->EntryPoint);
	SIZE_T image_base_addr = reinterpret_cast<SIZE_T>(module_info->lpBaseOfDll);

	debugger->add_breakpoint(_instruction_call, entry_point_addr - image_base_addr, L"EntryPoint", true);

	if (wcscmp(process->get_name(), L"halo_online.exe") == 0)
	{
		unsigned char winmain_call_pattern[] =
		{
			'\x56',									  /* 	0:   56,               push    esi     ; nShowCmd      */
			'\x50',									  /* 	1:   50,               push    eax     ; lpCmdLine     */
			'\x6A', '\x00',							  /* 	2:   6A, 00            push    0       ; hPrevInstance */
			'\x68', '\x00', '\x00', '\x40', '\x00'	  /* 	4:   68, 00 00 40 00   push    400000h ; hInstance     */
		};

		// replace `hInstance` with `image_base`
		*reinterpret_cast<DWORD*>(winmain_call_pattern + 5) = (DWORD)image_base_addr;

		unsigned char entry_point_instructions[READ_PAGE_SIZE] = { 0 };
		SIZE_T bytes_read = 0;
		debugger->read_debuggee_memory(entry_point, &entry_point_instructions, READ_PAGE_SIZE, &bytes_read);

		for (SIZE_T entry_point_offset = 0; entry_point_offset < READ_PAGE_SIZE; entry_point_offset++)
		{
			if (memcmp(entry_point_instructions + entry_point_offset, winmain_call_pattern, sizeof(winmain_call_pattern)) == 0)
			{
				SIZE_T offset = entry_point_addr + entry_point_offset + sizeof(winmain_call_pattern);

				SIZE_T call_location = 0;
				SIZE_T call_location_bytes_read = 0;
				debugger->read_debuggee_memory(reinterpret_cast<LPVOID>(offset + 1), &call_location, 4, &call_location_bytes_read);

				call_location += offset + 5;
				if (call_location < image_base_addr || call_location >image_base_addr + image_size)
					continue;

				SIZE_T call_location_module_offset = offset - image_base_addr;
				debugger->add_breakpoint(_instruction_call, call_location_module_offset, L"WinMain", true);
				break;
			}
		}
	}
}

void on_cached_map_files_open_all_breakpoint(c_debugger* debugger, c_registers* registers)
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

	LPVOID remote_page = debugger->allocate_and_write_debuggee_memory(resource_paths, sizeof(resource_paths), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE, NULL);

	for (size_t i = 0; i < sizeof(resource_paths) / 32; i++)
	{
		LPVOID lpBaseAddress = registers->get_runtime_addr_as<LPVOID>(resource_paths_offset + i * 4);
		LPVOID lpAddress = reinterpret_cast<LPVOID>(reinterpret_cast<SIZE_T>(remote_page) + i * 32);
		debugger->write_debuggee_pointer(lpBaseAddress, lpAddress, NULL);
	}

	printf("");
}

enum e_game_mode : ULONG
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mapeditor,
	_game_mode_savefilm,
	_game_mode_survival,

	k_game_mode_count,
};

enum e_game_engine_variant : ULONG
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

void on_main_game_load_map_breakpoint(c_debugger* debugger, c_registers* registers)
{
	static char game_options[0xE620] {};
	memset(game_options, 0, sizeof(game_options));

	char scenario_path[MAX_PATH] {};
	csstrncpy(scenario_path, MAX_PATH, "default", MAX_PATH);

	{
		wchar_t filename[MAX_PATH] {};
		swprintf_s(filename, MAX_PATH, L"%s\\test\\scenario_path.txt", debugger->get_process()->get_current_directory());

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
		debugger->read_debuggee_memory(registers->cast_cx_as<LPCVOID>(), game_options, sizeof(game_options), NULL);

		// game mode: multiplayer
		*reinterpret_cast<ULONG*>(game_options) = _game_mode_multiplayer;

		// scenario path: scenario_path
		csstrncpy(game_options + 0x24, MAX_PATH, scenario_path, MAX_PATH);

		// game engine: slayer
		*reinterpret_cast<ULONG*>(game_options + 0x32C) = _game_engine_slayer_variant;

		debugger->write_debuggee_memory(registers->cast_cx_as<LPVOID>(), game_options, sizeof(game_options), NULL);
	}
}

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count)
{
	strncpy_s(dest, size_in_bytes, src, max_count);
	memset(dest + strlen(src), 0, max_count - strlen(src));
}

void cswcsncpy(wchar_t* dest, rsize_t size_in_bytes, const wchar_t* src, rsize_t max_count)
{
	wcsncpy_s(dest, size_in_bytes, src, max_count);
	memset(dest + wcslen(src), 0, max_count - wcslen(src));
}

void on_is_debugger_present_breakpoint(c_debugger* debugger, c_registers* registers)
{
	// forces is_debugger_present() to return true
	static bool g_set_always_a_debugger_present = false;

	// forces is_debugger_present() to return false
	static bool g_set_never_a_debugger_present = false;

	bool set_always_a_debugger_present;
	bool set_never_a_debugger_present;
	debugger->read_debuggee_memory(registers->get_runtime_addr_as<LPCVOID>(0x318A210), &set_always_a_debugger_present, sizeof(set_always_a_debugger_present), NULL);
	debugger->read_debuggee_memory(registers->get_runtime_addr_as<LPCVOID>(0x318A211), &set_never_a_debugger_present, sizeof(set_never_a_debugger_present), NULL);

	if (set_always_a_debugger_present != g_set_always_a_debugger_present)
	{
		g_set_always_a_debugger_present = set_always_a_debugger_present;
		debugger->write_debuggee_memory(registers->get_runtime_addr_as<LPVOID>(0x318A210), &g_set_always_a_debugger_present, sizeof(g_set_always_a_debugger_present), NULL);
	}
	if (g_set_never_a_debugger_present != set_never_a_debugger_present)
	{
		g_set_never_a_debugger_present = set_never_a_debugger_present;
		debugger->write_debuggee_memory(registers->get_runtime_addr_as<LPVOID>(0x318A211), &g_set_never_a_debugger_present, sizeof(g_set_never_a_debugger_present), NULL);
	}

	printf("");
}

void on_shell_screen_pause_breakpoint(c_debugger* debugger, c_registers* registers)
{
	bool g_shell_application_paused = false;
	debugger->write_debuggee_memory(registers->get_runtime_addr_as<LPVOID>(0x2C204B9), &g_shell_application_paused, sizeof(g_shell_application_paused), NULL);
	printf("");
}


void on_shell_get_external_host_breakpoint(c_debugger* debugger, c_registers* registers)
{
	SIZE_T return_address = 0;
	SIZE_T call_address = -5;
	debugger->read_debuggee_memory((LPCVOID)registers->cast_sp_as<SIZE_T>(), &return_address, sizeof(return_address), NULL);
	call_address += return_address;

	//printf("%016zX\tcall shell_get_external_host\n", call_address);
	printf("");
}
