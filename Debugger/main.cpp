#include "main.h"

c_debugger g_debugger(new c_process());

int wmain(int argc, wchar_t* argv[])
{
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, WS_POPUP);
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	c_process& process = g_debugger.get_process();

	if (!process.open())
		process.create();

	process.suspend_thread();

	create_debugger_additions(g_debugger);

	g_debugger.run_debugger();

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
