#include "main.h"

c_process::c_process()
{
	ZeroMemory(&m_process_name, sizeof(m_process_name));
	ZeroMemory(&m_command_line, sizeof(m_command_line));

	ZeroMemory(&m_startup_info, sizeof(m_startup_info));
	ZeroMemory(&m_process_info, sizeof(m_process_info));
	m_startup_info.cb = sizeof(m_startup_info);

	m_suspended = false;
}

bool c_process::create(const wchar_t* format, ...)
{
	bool result = false;

	va_list va_args;
	va_start(va_args, format);
	vswprintf_s(m_command_line, 1024, format, va_args);
	va_end(va_args);

	result = CreateProcessW(NULL, m_command_line, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE, NULL, get_current_directory(), &m_startup_info, &m_process_info) == TRUE;
	Sleep(25);

	set_name(NULL);

	return result;
}

bool c_process::open(const wchar_t* process_name)
{
	bool result = false;

	set_name(process_name);
	set_current_directory(nullptr);

	PROCESSENTRY32 process_entry32 = { 0 };
	process_entry32.dwSize = sizeof(PROCESSENTRY32);

	// Iterate through all active processes and find the Wesnoth process
	HANDLE process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(process_snapshot, &process_entry32);

	do
	{
		if (wcscmp(process_entry32.szExeFile, process_name) == 0)
		{
			m_process_info.dwProcessId = process_entry32.th32ProcessID;

			// Save the process_id and open a handle to the process
			m_process_info.hProcess = OpenProcess(PROCESS_ALL_ACCESS, true, process_entry32.th32ProcessID);

			result = m_process_info.hProcess != INVALID_HANDLE_VALUE;
		}
	} while (Process32Next(process_snapshot, &process_entry32));

	return result;
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
	if (m_process_info.hThread && !m_suspended)
		m_suspended = SUCCEEDED(SuspendThread(m_process_info.hThread));
}
void c_process::resume_thread()
{
	if (m_process_info.hThread && m_suspended)
		m_suspended = !SUCCEEDED(ResumeThread(m_process_info.hThread));
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
		wcsncpy_s(m_current_directory, current_directory, MAX_PATH);
		return;
	}

	GetCurrentDirectory(MAX_PATH, m_current_directory);
}

const wchar_t* c_process::get_name()
{
	if (*m_process_name)
		return m_process_name;

	set_name(nullptr);
	return m_process_name;
}
void c_process::set_name(const wchar_t* process_name)
{
	if (process_name)
	{
		wcsncpy_s(m_process_name, process_name, MAX_PATH);
		return;
	}

	GetModuleBaseName(m_process_info.hProcess, NULL, m_process_name, MAX_PATH);
}
