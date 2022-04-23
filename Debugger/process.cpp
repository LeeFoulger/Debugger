#include <main.h>

c_process::c_process():
	m_command_line(new wchar_t[4096]),
	m_process_name(new wchar_t[MAX_PATH]),
	m_current_directory(new wchar_t[MAX_PATH])
{
	ZeroMemory(m_command_line, 4096);
	ZeroMemory(m_process_name, MAX_PATH);
	ZeroMemory(m_current_directory, MAX_PATH);

	ZeroMemory(&m_startup_info, sizeof(m_startup_info));
	ZeroMemory(&m_process_info, sizeof(m_process_info));
	m_startup_info.cb = sizeof(m_startup_info);

	m_suspended = false;

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	wcscat_s(m_process_name, MAX_PATH, argv[1]);
	wcscat_s(m_command_line, MAX_PATH, m_process_name);

	for (int i = 2; i < argc; i++)
	{
		wcscat_s(m_command_line, 4096, L" ");
		wcscat_s(m_command_line, 4096, argv[i]);
	}
}

c_process::~c_process()
{
	delete[] m_command_line;
	delete[] m_process_name;
	delete[] m_current_directory;
}

bool c_process::create()
{
	bool result = false;

	result = CreateProcessW(NULL, m_command_line, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, get_current_directory(), &m_startup_info, &m_process_info) == TRUE;
	Sleep(25);

	set_process_name(NULL);

	return result;
}

bool c_process::open()
{
	set_current_directory(nullptr);

	PROCESSENTRY32 process_entry32 = { 0 };
	process_entry32.dwSize = sizeof(PROCESSENTRY32);

	// iterate through all active processes and find our process
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(process_snapshot, &process_entry32);

	do
	{
		if (wcscmp(process_entry32.szExeFile, m_process_name) == 0)
		{
			// save the process_id and open a handle to the process

			m_process_info.dwProcessId = process_entry32.th32ProcessID;
			m_process_info.hProcess = OpenProcess(PROCESS_ALL_ACCESS, true, process_entry32.th32ProcessID);

			return m_process_info.hProcess != INVALID_HANDLE_VALUE;
		}
	} while (Process32Next(process_snapshot, &process_entry32));

	return false;
}

void c_process::close()
{
	if (m_process_info.hThread)
		CloseHandle(m_process_info.hThread);

	if (m_process_info.hProcess)
		CloseHandle(m_process_info.hProcess);
}

void c_process::suspend_thread()
{
	if (!m_process_info.hThread || m_suspended)
		return;

	SuspendThread(m_process_info.hThread);
	m_suspended = true;
}
void c_process::resume_thread()
{
	if (!m_process_info.hThread || !m_suspended)
		return;

	ResumeThread(m_process_info.hThread);
	m_suspended = false;
}

bool c_process::thread_is_suspended()
{
	return m_suspended;
}

HANDLE c_process::get_process_handle()
{
	return m_process_info.hProcess;
}
void c_process::set_process_handle(HANDLE process_handle)
{
	m_process_info.hProcess = process_handle;
}

HANDLE c_process::get_thread_handle()
{
	return m_process_info.hThread;
}
void c_process::set_thread_handle(HANDLE thread_handle)
{
	m_process_info.hThread = thread_handle;
}

DWORD c_process::get_process_id()
{
	return m_process_info.dwProcessId;
}
void c_process::set_process_id(DWORD process_id)
{
	m_process_info.dwProcessId = process_id;
}

DWORD c_process::get_thread_id()
{
	return m_process_info.dwThreadId;
}
void c_process::set_thread_id(DWORD thread_id)
{
	m_process_info.dwThreadId = thread_id;
}

const wchar_t* c_process::get_current_directory()
{
	if (*m_current_directory)
		return m_current_directory;

	set_current_directory(nullptr);
	return m_current_directory;
}
void c_process::set_current_directory(const wchar_t* current_directory)
{
	if (current_directory)
	{
		wcsncpy_s(m_current_directory, MAX_PATH, current_directory, MAX_PATH);
		return;
	}

	GetCurrentDirectory(MAX_PATH, m_current_directory);
}

const wchar_t* c_process::get_process_name()
{
	if (*m_process_name)
		return m_process_name;

	set_process_name(nullptr);
	return m_process_name;
}
void c_process::set_process_name(const wchar_t* process_name)
{
	if (process_name)
	{
		wcsncpy_s(m_process_name, MAX_PATH, process_name, MAX_PATH);
		return;
	}

	GetModuleBaseName(m_process_info.hProcess, NULL, m_process_name, MAX_PATH);
}
