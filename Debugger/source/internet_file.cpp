#include <main.h>

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

c_internet_file::c_internet_file() :
	m_connection()
{
	set_connection_handle(L"Mozilla/5.0");
}

c_internet_file::c_internet_file(const wchar_t* user_agent) :
	m_connection()
{
	set_connection_handle(user_agent);
}

c_internet_file::~c_internet_file()
{
	if (m_connection)
		InternetCloseHandle(m_connection);
}

void c_internet_file::set_connection_handle(const wchar_t* user_agent)
{
	m_connection = InternetOpenW(user_agent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
}

bool c_internet_file::download_file(c_buffer& buffer, const wchar_t* url)
{
	if (!m_connection)
		return false;

	const HINTERNET open_address = InternetOpenUrlW(m_connection, url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, NULL);
	if (!open_address)
		return false;

	buffer.expand(4096);
	DWORD bytes_read = 0;
	DWORD total_bytes_read = 0;
	while (InternetReadFile(open_address, buffer[total_bytes_read], 4096, &bytes_read) && bytes_read)
	{
		buffer.expand(bytes_read);
		total_bytes_read += bytes_read;
	}
	InternetCloseHandle(open_address);
	buffer.compact(4096);

	return true;
}
