#include <main.h>

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

	g_debugger.attach();
	g_debugger.run_debugger();

	return 0;
}
