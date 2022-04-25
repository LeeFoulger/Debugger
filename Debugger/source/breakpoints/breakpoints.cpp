#include <main.h>

void create_debugger_additions(c_debugger& debugger)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	if (argc < 3 && wcscmp(argv[1], L"halo_online.exe") == 0)
		debugger.add_breakpoint(_instruction_call, 0x0075227E - PE32_BASE, L"command_line_get_credentials", false, on_command_line_get_credentials_breakpoint);

#define module_info_callback(callback) debugger.add_module_info_callback((#callback), (callback))
	module_info_callback(add_break_on_winmain);
	module_info_callback(add_breaks_following_winmain);
	module_info_callback(add_test_breaks);
#undef module_info_callback
}

#include "breakpoints.hf2p.inl"

#ifdef _WIN64
//#include "breakpoints.tools.halo1.inl"
//#include "breakpoints.tools.halo2.inl"
//#include "breakpoints.tools.halo3.inl"
#include "breakpoints.tools.odst.inl"
//#include "breakpoints.tools.reach.inl"
//#include "breakpoints.tools.halo4.inl"
#endif // _WIN64

void add_test_breaks(c_debugger& debugger, LPMODULEINFO module_info)
{
	{
		wchar_t filename[MAX_PATH]{};
		swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.txt", debugger.get_process().get_current_directory());

		FILE* file = NULL;
		if (_wfopen_s(&file, filename, L"w"), file != NULL)
		{
			fprintf(file, "size, name, type");
			fclose(file);
		}
	}

#ifdef _WIN64
	if (wcscmp(debugger.get_process().get_process_name(), L"atlas_tag_test.exe") == 0)
	{
		debugger.add_breakpoint(0x40, 0x00000001403916E0 - PE64_BASE, L"is_debugger_present", false, on_is_debugger_present_breakpoint);
		debugger.add_breakpoint(0xC2, 0x00000001401A9A60 - PE64_BASE, L"shell_screen_pause", false, on_shell_screen_pause_breakpoint);
		//debugger.add_breakpoint(0x33, 0x00000001401A9540 - PE64_BASE, L"shell_get_external_host", false, on_shell_get_external_host_breakpoint);
		debugger.add_breakpoint(0x48, 0x00000001404A18D0 - PE64_BASE, L"restricted_region_add_member", false, on_restricted_region_add_member_breakpoint);
		debugger.add_breakpoint(0x48, 0x000000014036A700 - PE64_BASE, L"shell_get_system_identifier", false, on_shell_get_system_identifier_breakpoint);
		debugger.add_breakpoint(0xC3, 0x0000000140235724 - PE64_BASE, L"shell_get_gamertag on return", false, on_shell_get_gamertag_return_breakpoint); // name isn't correct but that's fine
	}
#else
	if (wcscmp(debugger.get_process().get_process_name(), L"halo_online.exe") == 0)
	{
		debugger.add_breakpoint(0xFF, 0x0056918C - PE32_BASE, L"restricted_region_add_member::internal", false, on_restricted_region_add_member_internal_breakpoint);
		debugger.add_breakpoint(_instruction_call, 0x005B103C - PE32_BASE, L"rasterizer_draw_watermark", false, on_rasterizer_draw_watermark_breakpoint);
		debugger.add_breakpoint(0x84, 0x004E2A3F - PE32_BASE, L"machinima_camera_debug check result", false, on_machinima_camera_debug_breakpoint);
		debugger.add_breakpoint(0x55, 0x00483D40 - PE32_BASE, L"cache_file_blocking_read", false, on_cache_file_blocking_read_breakpoint);
		debugger.add_breakpoint(0x84, 0x00482DB2 - PE32_BASE, L"cache_files_verify_header_rsa_signature check result", false, on_cache_files_verify_header_rsa_signature_breakpoint);
		debugger.add_breakpoint(0x3A, 0x004C6CB3 - PE32_BASE, L"game_engine_update_round_conditions: game_is_distributed check result", false, [](c_debugger& debugger, c_registers& registers)
			{
				// .text:004C6CB3	cmp     cl, al				<--- current breakpoint
				// .text:004C6CB5	jz      short loc_4C6D0A	<--- current eip
				// .text:004C6D0A	cmp     pop     edi			<--- new eip

				const size_t jump_size = 0x004C6D0A - 0x004C6CB5;
				registers.get_raw_context().Xip += jump_size;
			});
	}
#endif // _WIN64
}

void add_breaks_following_winmain(c_debugger& debugger, LPMODULEINFO module_info)
{
	if (wcscmp(debugger.get_process().get_process_name(), L"halo_online.exe") == 0)
	{
		// EntryPoint->WinMain
		debugger.add_breakpoint(_instruction_call, 0x004013C3 - PE32_BASE, L"static_string<64>::print", true);
		debugger.add_breakpoint(_instruction_call, 0x00401501 - PE32_BASE, L"shell_initialize", true);
		debugger.add_breakpoint(_instruction_call, 0x0040151C - PE32_BASE, L"main_loop", true);
		debugger.add_breakpoint(_instruction_call, 0x0040152F - PE32_BASE, L"shell_dispose", true);

		// shell_initialize->cache_files_initialize
		debugger.add_breakpoint(_instruction_call, 0x004ED644 - PE32_BASE, L"cached_map_files_open_all", true, on_cached_map_files_open_all_breakpoint);

		// WinMain->main_loop
		debugger.add_breakpoint(_instruction_call, 0x004A9640 - PE32_BASE, L"main_game_load_map", true, on_main_game_load_map_breakpoint);
		debugger.add_breakpoint(_instruction_call, 0x004A9670 - PE32_BASE, L"main_game_start", true);

		// main_loop->main_game_load_map
		debugger.add_breakpoint(_instruction_call, 0x004A9B96 - PE32_BASE, L"scenario_load", true);
		debugger.add_breakpoint(_instruction_call, 0x004A9BA4 - PE32_BASE, L"main_game_internal_map_load_complete", true);

		// main_game_load_map->scenario_load
		debugger.add_breakpoint(_instruction_call, 0x0047E8EC - PE32_BASE, L"scenario_tags_load", true);
		debugger.add_breakpoint(_instruction_call, 0x0047E973 - PE32_BASE, L"scenario_tags_fixup", true);

		// scenario_load->scenario_tags_load
		debugger.add_breakpoint(_instruction_call, 0x00483843 - PE32_BASE, L"cache_file_open", true);
		debugger.add_breakpoint(_instruction_call, 0x00483856 - PE32_BASE, L"cache_file_header_verify", true);
		debugger.add_breakpoint(_instruction_call, 0x0048389C - PE32_BASE, L"tags_file_open", true);
	}
}

void add_break_on_winmain(c_debugger& debugger, LPMODULEINFO module_info)
{
	c_process& process = debugger.get_process();

	HANDLE process_handle = process.get_process_handle();
	LPVOID entry_point = module_info->EntryPoint;
	LPVOID image_base = module_info->lpBaseOfDll;
	SIZE_T image_size = module_info->SizeOfImage;

	SIZE_T entry_point_addr = reinterpret_cast<SIZE_T>(module_info->EntryPoint);
	SIZE_T image_base_addr = reinterpret_cast<SIZE_T>(module_info->lpBaseOfDll);

	debugger.add_breakpoint(_instruction_call, entry_point_addr - image_base_addr, L"EntryPoint", true);

	if (wcscmp(process.get_process_name(), L"halo_online.exe") == 0)
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
		debugger.read_debuggee_memory(entry_point, entry_point_instructions, READ_PAGE_SIZE, &bytes_read);

		for (SIZE_T entry_point_offset = 0; entry_point_offset < READ_PAGE_SIZE; entry_point_offset++)
		{
			if (memcmp(entry_point_instructions + entry_point_offset, winmain_call_pattern, sizeof(winmain_call_pattern)) == 0)
			{
				SIZE_T offset = entry_point_addr + entry_point_offset + sizeof(winmain_call_pattern);

				SIZE_T call_location = 0;
				SIZE_T call_location_bytes_read = 0;
				debugger.read_debuggee_memory(reinterpret_cast<LPVOID>(offset + 1), &call_location, 4, &call_location_bytes_read);

				call_location += offset + 5;
				if (call_location < image_base_addr || call_location >image_base_addr + image_size)
					continue;

				SIZE_T call_location_module_offset = offset - image_base_addr;
				debugger.add_breakpoint(_instruction_call, call_location_module_offset, L"WinMain", true);
				break;
			}
		}
	}
}
