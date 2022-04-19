#include "main.h"

static wchar_t g_command_line[4096]{};

int wmain(int argc, wchar_t* argv[])
{
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, WS_POPUP);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	static c_process process;

	for (int i = 1; i < argc; i++)
	{
		wcscat_s(g_command_line, 4096, argv[i]);
		wcscat_s(g_command_line, 4096, L" ");
	}

	if (!process.open(argv[1]))
		process.create(L"%s", g_command_line);

	process.suspend_thread();

	static c_debugger debugger(process);

	if (argc < 3 && wcscmp(process.get_name(), L"halo_online.exe") == 0)
		debugger.add_breakpoint(_instruction_call, 0x0075227E - PE32BASE, L"command_line_get_credentials", false, on_command_line_get_credentials_breakpoint);

	debugger.add_module_info_callback(add_break_on_winmain);
	debugger.add_module_info_callback(add_breaks_following_winmain);
	debugger.add_module_info_callback(add_test_breaks);

	debugger.run_debugger();

	return 0;
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
