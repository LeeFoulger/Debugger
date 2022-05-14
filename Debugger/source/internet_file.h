#pragma once

class c_internet_file
{
public:
	c_internet_file();
	c_internet_file(const wchar_t* user_agent);
	~c_internet_file();

	void set_connection_handle(const wchar_t* user_agent);
	bool download_file(class c_buffer* out_buffer, const wchar_t* url);

private:
	void* m_connection;
};
