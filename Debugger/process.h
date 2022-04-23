#pragma once

class c_process
{
public:
	c_process();
	~c_process();

	bool create();
	bool open();
	void close();

	void suspend_thread();
	void resume_thread();
	bool thread_is_suspended();

	HANDLE get_process_handle();
	void set_process_handle(HANDLE process_handle);

	HANDLE get_thread_handle();
	void set_thread_handle(HANDLE thread_handle);

	DWORD get_process_id();
	void set_process_id(DWORD process_id);

	DWORD get_thread_id();
	void set_thread_id(DWORD thread_id);

	const wchar_t* get_current_directory();
	void set_current_directory(const wchar_t* current_directory);

	const wchar_t* get_process_name();
	void set_process_name(const wchar_t* process_name);

protected:
	wchar_t* m_command_line;
	wchar_t* m_process_name;
	wchar_t* m_current_directory;

	STARTUPINFO m_startup_info;
	PROCESS_INFORMATION m_process_info;

	bool m_suspended;
};
