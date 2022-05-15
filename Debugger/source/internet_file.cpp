#include <main.h>

#include <stdlib.h>

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

bool c_internet_file::download_data(c_buffer& buffer, const wchar_t* url)
{
	if (!m_connection || !url)
		return false;

	if (wcsstr(url, L"http://") != url && wcsstr(url, L"https://") != url)
	{
		wchar_t tmp[256]{};
		wcscat_s(tmp, 256, L"http://");
		wcscat_s(tmp, 256, url);
		url = tmp;
	}

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

bool c_internet_file::download_data(c_buffer& buffer, const char* url)
{
	if (!url)
		return false;

	size_t temp;
	wchar_t url_wide[256]{};
	mbstowcs_s(&temp, url_wide, 256, url, 256);

	return download_data(buffer, url_wide);
}