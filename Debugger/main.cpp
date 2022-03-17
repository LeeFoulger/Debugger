#include "main.h"

const bool disable_saber_code_applied_in_scenario_load = false;

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

	if (argc < 3)
		debugger->add_breakpoint(_instruction_call, 0x0075227E - PE32BASE, L"command_line_get_credentials", on_command_line_get_credentials_breakpoint);

	//debugger->add_module_info_callback(add_break_on_winmain);
	//debugger->add_module_info_callback(add_breaks_following_winmain);
	//debugger->add_module_info_callback(add_test_breaks);

	debugger->run_debugger();

	return 0;
}

void on_command_line_get_credentials_breakpoint(c_debugger* debugger, c_registers* registers)
{
	LPVOID new_debuggee_string = debugger_write_debuggee_string(debugger, "--account 123 --sign-in-code 123 --environment 123");
	LPVOID debuggee_command_line = registers->get_runtime_addr_as<LPVOID>(0x052BE944 - PE32BASE);
	debugger->write_debuggee_pointer(debugger, debuggee_command_line, new_debuggee_string, NULL);

	size_t data_size = 0x375F0;
	char* data = new char[data_size] {};
	LPVOID data_addr = registers->get_runtime_addr_as<LPVOID>(0x043E1408 - PE32BASE);
	debugger->read_debuggee_memory(data_addr, data, data_size, NULL);

	printf("");

	if (disable_saber_code_applied_in_scenario_load)
	{
		unsigned char patch_data_bytes[] = { 0x00 };
		LPVOID data_addr1 = registers->get_runtime_addr_as<LPVOID>(0x01346881 - PE32BASE);
		debugger->write_debuggee_memory(data_addr1, patch_data_bytes, sizeof(patch_data_bytes), NULL);
	}

	printf("");
}

void add_test_breaks(c_debugger* debugger, LPMODULEINFO module_info)
{

	//debugger->add_breakpoint(0x00496EEE - PE32BASE, L"main_status->memset", [](c_debugger* debugger, c_registers* registers) -> void {
	//	DWORD data_size = registers->cast_esi_as<DWORD>();
	//	char* data = new char[data_size + 1]{};
	//
	//	debugger->read_debuggee_memory(registers->cast_edi_as<LPCVOID>(), data, data_size, NULL);
	//
	//	printf("[main_status] %s\n", data);
	//	delete[] data;
	//});
}

void add_breaks_following_winmain(c_debugger* debugger, LPMODULEINFO module_info)
{
	// EntryPoint->WinMain
	debugger->add_breakpoint(_instruction_call, 0x004013C3 - PE32BASE, L"static_string<64>::print");
	debugger->add_breakpoint(_instruction_call, 0x00401501 - PE32BASE, L"shell_initialize");
	debugger->add_breakpoint(_instruction_call, 0x0040151C - PE32BASE, L"main_loop");
	debugger->add_breakpoint(_instruction_call, 0x0040152F - PE32BASE, L"shell_dispose");

	// shell_initialize->cache_files_initialize
	debugger->add_breakpoint(_instruction_call, 0x004ED644 - PE32BASE, L"cached_map_files_open_all", on_cached_map_files_open_all_breakpoint);

	// WinMain->main_loop
	debugger->add_breakpoint(_instruction_call, 0x004A9640 - PE32BASE, L"main_game_load_map", on_main_game_load_map_breakpoint);
	debugger->add_breakpoint(_instruction_call, 0x004A9670 - PE32BASE, L"main_game_start");

	// main_loop->main_game_load_map
	debugger->add_breakpoint(_instruction_call, 0x004A9B96 - PE32BASE, L"scenario_load");
	debugger->add_breakpoint(_instruction_call, 0x004A9BA4 - PE32BASE, L"main_game_internal_map_load_complete");

	// main_game_load_map->scenario_load
	debugger->add_breakpoint(_instruction_call, 0x0047E8EC - PE32BASE, L"scenario_tags_load");
	debugger->add_breakpoint(_instruction_call, 0x0047E973 - PE32BASE, L"scenario_tags_fixup");

	// scenario_load->scenario_tags_load
	debugger->add_breakpoint(_instruction_call, 0x00483843 - PE32BASE, L"cache_file_open");
	debugger->add_breakpoint(_instruction_call, 0x00483856 - PE32BASE, L"cache_file_header_verify");
	debugger->add_breakpoint(_instruction_call, 0x0048389C - PE32BASE, L"tags_file_open");
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
	DWORD image_size = module_info->SizeOfImage;

	DWORD entry_point_addr = reinterpret_cast<DWORD>(module_info->EntryPoint);
	DWORD image_base_addr = reinterpret_cast<DWORD>(module_info->lpBaseOfDll);

	debugger->add_breakpoint(_instruction_call, entry_point_addr - image_base_addr, L"EntryPoint");

	if (wcscmp(process->get_name(), L"halo_online.exe"))
		return;

	unsigned char winmain_call_pattern[] =
	{
		'\x56',									  /* 	0:   56,               push    esi     ; nShowCmd      */
		'\x50',									  /* 	1:   50,               push    eax     ; lpCmdLine     */
		'\x6A', '\x00',							  /* 	2:   6A, 00            push    0       ; hPrevInstance */
		'\x68', '\x00', '\x00', '\x40', '\x00'	  /* 	4:   68, 00 00 40 00   push    400000h ; hInstance     */
	};

	// replace `hInstance` with `image_base`
	*reinterpret_cast<DWORD*>(winmain_call_pattern + 5) = image_base_addr;

	unsigned char entry_point_instructions[READ_PAGE_SIZE] = { 0 };
	DWORD bytes_read = 0;
	debugger->read_debuggee_memory(entry_point, &entry_point_instructions, READ_PAGE_SIZE, &bytes_read);

	for (DWORD entry_point_offset = 0; entry_point_offset < READ_PAGE_SIZE; entry_point_offset++)
	{
		if (memcmp(entry_point_instructions + entry_point_offset, winmain_call_pattern, sizeof(winmain_call_pattern)) == 0)
		{
			DWORD offset = entry_point_addr + entry_point_offset + sizeof(winmain_call_pattern);

			DWORD call_location = 0;
			DWORD call_location_bytes_read = 0;
			debugger->read_debuggee_memory(reinterpret_cast<LPVOID>(offset + 1), &call_location, 4, &call_location_bytes_read);

			call_location += offset + 5;
			if (call_location < image_base_addr || call_location >image_base_addr + image_size)
				continue;

			DWORD call_location_module_offset = offset - image_base_addr;
			debugger->add_breakpoint(_instruction_call, call_location_module_offset, L"WinMain");
			break;
		}
	}
}

void on_cached_map_files_open_all_breakpoint(c_debugger* debugger, c_registers* registers)
{
	DWORD resource_paths_offset = 0x012ECEA4 - PE32BASE;
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
		LPVOID lpAddress = reinterpret_cast<LPVOID>(reinterpret_cast<DWORD>(remote_page) + i * 32);
		debugger->write_debuggee_pointer(debugger, lpBaseAddress, lpAddress, NULL);
	}

	printf("");
}

void on_main_game_load_map_breakpoint(c_debugger* debugger, c_registers* registers)
{
	static char game_options[0xE620]{};
	memset(game_options, 0, sizeof(game_options));

	char scenario_path[MAX_PATH]{};

	{
		wchar_t current_directory[MAX_PATH]{};
		swprintf_s(current_directory, MAX_PATH, L"%s\\test\\scenario_path.txt", debugger->get_process()->get_current_directory());

		FILE* file = NULL;
		if (_wfopen_s(&file, current_directory, L"r"), file != NULL)
		{
			fgets(scenario_path, MAX_PATH, file);
			fclose(file);
		}

		if (*scenario_path == 0 || *scenario_path == '\r' || *scenario_path == '\n')
		{
			//fputs("enter scenario path: ", stdout);
			//fgets(scenario_path, MAX_PATH, stdin);
		}

		if (*scenario_path)
		{
			while (scenario_path[strlen(scenario_path) - 1] == '\r' || scenario_path[strlen(scenario_path) - 1] == '\n')
				scenario_path[strlen(scenario_path) - 1] = 0;
		}
	}

	if (*scenario_path)
	{
		debugger->read_debuggee_memory(registers->cast_ecx_as<LPCVOID>(), game_options, sizeof(game_options), NULL);

		// game mode: multiplayer
		*reinterpret_cast<ULONG*>(game_options) = 2;

		// scenario path: scenario_path
		csstrncpy(game_options + 0x24, MAX_PATH, scenario_path, MAX_PATH);

		// game engine: slayer
		*reinterpret_cast<ULONG*>(game_options + 0x32C) = 2;

		debugger->write_debuggee_memory(registers->cast_ecx_as<LPVOID>(), game_options, sizeof(game_options), NULL);
	}
}

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count)
{
	strncpy_s(dest, size_in_bytes, src, max_count);
	memset(dest + strlen(src), 0, max_count - strlen(src));
}