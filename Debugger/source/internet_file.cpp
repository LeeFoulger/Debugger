#include <main.h>

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

c_internet_file::c_internet_file() :
	m_connection()
{
	// Edge 87 on Windows 10
	set_connection_handle(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.141 Safari/537.36 Edg/87.0.664.75");
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

bool c_internet_file::download_file(c_buffer* out_buffer, const wchar_t* url)
{
	if (!out_buffer)
		return false;

	if (m_connection)
	{
		const HINTERNET open_address = InternetOpenUrlW(m_connection, url, NULL, 0, INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_KEEP_CONNECTION, NULL);
		if (open_address)
		{
			c_buffer& buffer = *out_buffer;
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
	}

	return false;
}
