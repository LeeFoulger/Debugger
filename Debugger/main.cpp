#include "main.h"

int wmain(int argc, wchar_t* argv[])
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	c_process* process = new c_process();

	if (!process->open(argv[1]))
		process->create(CREATE_SUSPENDED, L"%s", argv[1]);

	if (!process->is_thread_suspended())
		process->suspend_thread();

	c_debugger* debugger = new c_debugger(process);


	debugger->run_debugger();

	return 0;
}

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count)
{
	strncpy_s(dest, size_in_bytes, src, max_count);
	memset(dest + strlen(src), 0, max_count - strlen(src));
}