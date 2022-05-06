#include <main.h>
#include <string_utils.h>

#include <shellapi.h>
#include <stdio.h>

void create_debugger_additions(c_debugger& debugger)
{
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

#ifdef _WIN64
#else
	if (argc < 3 && wcscmp(argv[1], L"halo_online.exe") == 0)
		debugger.add_breakpoint(0x0075227E - PE32_BASE, false, "call", L"command_line_get_credentials", on_command_line_get_credentials_breakpoint);
#endif // _WIN64

#define module_info_callback(callback) debugger.add_module_info_callback((#callback), (callback))
	module_info_callback(add_break_on_winmain);
	module_info_callback(add_breaks_following_winmain);
	module_info_callback(add_test_breaks);
#undef module_info_callback
}

#ifdef _WIN64
//#include "breakpoints.tools.halo1.inl"
//#include "breakpoints.tools.halo2.inl"
#include "breakpoints.tools.halo3.inl"
#include "breakpoints.tools.odst.inl"
//#include "breakpoints.tools.reach.inl"
//#include "breakpoints.tools.halo4.inl"
#else
#include "breakpoints.hf2p.inl"
#endif // _WIN64

void add_test_breaks(c_debugger& debugger, LPMODULEINFO module_info)
{
	{
		wchar_t filename[MAX_PATH]{};
		swprintf_s(filename, MAX_PATH, L"%s\\bin\\globals.csv", debugger.get_process().get_current_directory());

		FILE* file = NULL;
		if (_wfopen_s(&file, filename, L"w"), file != NULL)
		{
			fprintf(file, "size, type, name\n");
			fclose(file);
		}
	}

#ifdef _WIN64
	if (wcscmp(debugger.get_process().get_process_name(), L"halo3_tag_test.exe") == 0)
	{
		debugger.add_breakpoint(0x00000001408DABAF - PE64_BASE, false, "lea rbx,[rbx*8]", L"c_rasterizer_global_shaders::setup_global_shader", on_setup_global_shader_breakpoint);
	}
	else if (wcscmp(debugger.get_process().get_process_name(), L"atlas_tag_test.exe") == 0)
	{
		debugger.add_breakpoint(0x00000001403916E0 - PE64_BASE, false, "push rbx", L"is_debugger_present", on_is_debugger_present_breakpoint);
		debugger.add_breakpoint(0x00000001401A9A60 - PE64_BASE, false, "ret", L"shell_screen_pause", on_shell_screen_pause_breakpoint);
		//debugger.add_breakpoint(0x00000001401A9540 - PE64_BASE, false, "xor eax,eax", L"shell_get_external_host", on_shell_get_external_host_breakpoint);
		debugger.add_breakpoint(0x00000001404A18D0 - PE64_BASE, false, "mov [rsp+8],rbx", L"restricted_region_add_member", on_restricted_region_add_member_breakpoint);
		debugger.add_breakpoint(0x000000014036A700 - PE64_BASE, false, "mov rax,rsp", L"shell_get_system_identifier", on_shell_get_system_identifier_breakpoint);
		debugger.add_breakpoint(0x0000000140235724 - PE64_BASE, false, "ret", L"shell_get_gamertag on return", on_shell_get_gamertag_return_breakpoint); // name isn't correct but that's fine
		//debugger.add_breakpoint(0x00000001403AF200 - PE64_BASE, false, "sub rsp,28h", L"string_id_get_string_const", on_string_id_get_string_const_breakpoint);
	}
#else
	if (wcscmp(debugger.get_process().get_process_name(), L"halo_online.exe") == 0)
	{
		debugger.add_breakpoint(0x0056918C - PE32_BASE, false, "call", L"restricted_region_add_member::internal", on_restricted_region_add_member_internal_breakpoint);
		debugger.add_breakpoint(0x005B103C - PE32_BASE, false, "call", L"rasterizer_draw_watermark", on_rasterizer_draw_watermark_breakpoint);
		debugger.add_breakpoint(0x004E2A3F - PE32_BASE, false, "test al,al", L"machinima_camera_debug check result", on_machinima_camera_debug_breakpoint);
		debugger.add_breakpoint(0x00483D40 - PE32_BASE, false, "push ebp", L"cache_file_blocking_read", on_cache_file_blocking_read_breakpoint);
		debugger.add_breakpoint(0x00482DB2 - PE32_BASE, false, "test al,al", L"cache_files_verify_header_rsa_signature check result", on_cache_files_verify_header_rsa_signature_breakpoint);
		debugger.add_breakpoint(0x005D6B76 - PE32_BASE, false, "pop ebp", L"contrails_render_callback", on_contrails_render_callback_breakpoint);
		debugger.add_breakpoint(0x0049713F - PE32_BASE, false, "cmp [4BC1E48h],al", L"main_kick_startup_masking_sequence: startup_sequence check result", on_main_kick_startup_masking_sequence_breakpoint);
	}
#endif // _WIN64
}

void add_breaks_following_winmain(c_debugger& debugger, LPMODULEINFO module_info)
{
#ifdef _WIN64
#else
	if (wcscmp(debugger.get_process().get_process_name(), L"halo_online.exe") == 0)
	{
		// EntryPoint->WinMain
		debugger.add_breakpoint(0x004013C3 - PE32_BASE, true, "call", L"static_string<64>::print");
		debugger.add_breakpoint(0x00401501 - PE32_BASE, true, "call", L"shell_initialize");
		debugger.add_breakpoint(0x0040151C - PE32_BASE, true, "call", L"main_loop");
		debugger.add_breakpoint(0x0040152F - PE32_BASE, true, "call", L"shell_dispose");

		// shell_initialize->cache_files_initialize
		debugger.add_breakpoint(0x004ED644 - PE32_BASE, true, "call", L"cached_map_files_open_all", on_cached_map_files_open_all_breakpoint);

		// WinMain->main_loop
		debugger.add_breakpoint(0x004A9640 - PE32_BASE, true, "call", L"main_game_load_map", on_main_game_load_map_breakpoint);
		debugger.add_breakpoint(0x004A9670 - PE32_BASE, true, "call", L"main_game_start");

		// main_loop->main_game_load_map
		debugger.add_breakpoint(0x004A9B96 - PE32_BASE, true, "call", L"scenario_load");
		debugger.add_breakpoint(0x004A9BA4 - PE32_BASE, true, "call", L"main_game_internal_map_load_complete");

		// main_game_load_map->scenario_load
		debugger.add_breakpoint(0x0047E8EC - PE32_BASE, true, "call", L"scenario_tags_load");
		debugger.add_breakpoint(0x0047E973 - PE32_BASE, true, "call", L"scenario_tags_fixup");

		// scenario_load->scenario_tags_load
		debugger.add_breakpoint(0x00483843 - PE32_BASE, true, "call", L"cache_file_open");
		debugger.add_breakpoint(0x00483856 - PE32_BASE, true, "call", L"cache_file_header_verify");
		debugger.add_breakpoint(0x0048389C - PE32_BASE, true, "call", L"tags_file_open");
	}
#endif // _WIN64
}

void add_break_on_winmain(c_debugger& debugger, LPMODULEINFO module_info)
{
	c_process& process = debugger.get_process();

	HANDLE process_handle = process.get_process_handle();
	LPVOID entry_point = module_info->EntryPoint;
	LPVOID image_base = module_info->lpBaseOfDll;
	size_t image_size = module_info->SizeOfImage;

	size_t entry_point_addr = reinterpret_cast<size_t>(module_info->EntryPoint);
	size_t image_base_addr = reinterpret_cast<size_t>(module_info->lpBaseOfDll);

	debugger.add_breakpoint(entry_point_addr - image_base_addr, true, "call", L"EntryPoint");

#ifdef _WIN64
#else
	if (wcscmp(process.get_process_name(), L"halo_online.exe") == 0)
	{
		unsigned char winmain_call_pattern[] =
		{
			'\x56',									  /* 	0:   56,               push    esi     ; nShowCmd      */
			'\x50',									  /* 	1:   50,               push    eax     ; lpCmdLine     */
			'\x6A', '\x00',							  /* 	2:   6A, 00            push    0       ; hPrevInstance */
			'\x68', '\x00', '\x00', '\x40', '\x00'	  /* 	4:   68, 00 00 40 00   push    400000h ; hInstance     */
		};

		// replace `hInstance` with `lpBaseOfDll`
		*reinterpret_cast<DWORD*>(winmain_call_pattern + 5) = (DWORD)image_base_addr;

		unsigned char entry_point_instructions[READ_PAGE_SIZE] = { 0 };
		SIZE_T bytes_read = 0;
		debugger.read_debuggee_memory(entry_point, entry_point_instructions, READ_PAGE_SIZE, &bytes_read);

		for (size_t entry_point_offset = 0; entry_point_offset < READ_PAGE_SIZE; entry_point_offset++)
		{
			if (memcmp(entry_point_instructions + entry_point_offset, winmain_call_pattern, sizeof(winmain_call_pattern)) == 0)
			{
				size_t offset = entry_point_addr + entry_point_offset + sizeof(winmain_call_pattern);

				size_t call_location = 0;
				SIZE_T call_location_bytes_read = 0;
				debugger.read_debuggee_memory(reinterpret_cast<LPVOID>(offset + 1), &call_location, 4, &call_location_bytes_read);

				call_location += offset + 5;
				if (call_location < image_base_addr || call_location > (image_base_addr + image_size))
					continue;

				size_t call_location_module_offset = offset - image_base_addr;
				debugger.add_breakpoint(call_location_module_offset, true, "call", L"WinMain");
				break;
			}
		}
	}
#endif // _WIN64
}
