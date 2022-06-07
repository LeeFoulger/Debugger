
#include <windows.h>
#include <cassert>
#include <ctime>
#include <cstdio>

#define UNIMPLEMENTED (DWORD)-1;

struct s_file_last_modification_date
{
	FILETIME filetime;
};

struct s_file_handle
{
	HANDLE handle;
};

typedef struct s_file_reference
{
	unsigned long signature;
	unsigned short flags;
	unsigned short location;

	char path[256];

	s_file_handle handle;
	unsigned long api_result; // position
} file_reference_info;

struct s_find_file_data
{
	unsigned long flags;
	unsigned short depth;
	unsigned short location;

	char path[256];
	char search_spec[256];

	s_file_handle handles[16];
	WIN32_FIND_DATAA win32_find_file_data;

	char __data[32];
};

void directory_create_or_delete_contents(char const* directory);
BOOL file_create_parent_directories_if_not_present(s_file_reference const* info);
BOOL file_is_readable(s_file_reference* info, unsigned long* error);
void file_location_set_volume(short location, char const* volume_name);
void file_printf(s_file_reference* info, char const* fmt, ...);
BOOL file_read_into_buffer(s_file_reference* info, void* buffer, unsigned long buffer_length);
void* file_read_into_memory(s_file_reference* info, unsigned long* out_length);
void* file_read_into_memory_with_null_padding(s_file_reference* info, unsigned long* out_length, unsigned long padding);
s_file_reference* file_reference_add_directory(s_file_reference* info, char const* name);
s_file_reference* file_reference_agnostic_create(s_file_reference* info, short location);
s_file_reference* file_reference_copy(s_file_reference* dst_info, s_file_reference const* src_info);
s_file_reference* file_reference_create_from_path(s_file_reference* info, char const* name, BOOL add_directory);
char* file_reference_get_fullpath(s_file_reference const* info, char* out_full_path, long full_path_length);
short file_reference_get_location(s_file_reference const* info);
char* file_reference_get_name(s_file_reference const* info, unsigned long flags, char* out_name, long name_length);
char const* file_reference_get_path_for_debugging(s_file_reference const* info);
s_file_reference* file_reference_remove_directory(s_file_reference* info);
s_file_reference* file_reference_remove_name(s_file_reference* info);
s_file_reference* file_reference_set_name(s_file_reference* info, char const* name);
BOOL file_references_equal(s_file_reference const* info, s_file_reference const* other);
void file_trim(s_file_reference* info, long);
long find_files(unsigned long, s_file_reference const*, long, s_file_reference*);
void file_activity_debug_render(void);
BOOL file_close(s_file_reference* info);
int file_compare_last_modification_dates(s_file_last_modification_date const* date1, s_file_last_modification_date const* date2);
BOOL file_copy_to(s_file_reference const* info, s_file_reference const* other, BOOL);
BOOL file_copy_to_recursive(s_file_reference* info, s_file_reference*);
BOOL file_create(s_file_reference* info);
void file_date_format_for_output(s_file_last_modification_date*, char*, long);
BOOL file_delete(s_file_reference* info);
BOOL file_delete_recursive(s_file_reference* info);
BOOL file_exists(s_file_reference const* info);
BOOL file_get_creation_date(s_file_reference const* info, s_file_last_modification_date*);
unsigned long file_get_eof(s_file_reference const* info);
BOOL file_get_last_modification_date(s_file_reference const* info, s_file_last_modification_date*);
unsigned long file_get_position(s_file_reference const* info);
BOOL file_get_size(s_file_reference const* info, unsigned long*);
BOOL file_handle_is_valid(s_file_handle);
BOOL file_is_directory(s_file_reference const* info);
BOOL file_last_modification_date_to_time(s_file_last_modification_date const*, tm*);
void file_location_get_full_path(short, char const*, char*, long);
BOOL file_move_to(s_file_reference const* info, s_file_reference const* other);
BOOL file_open(s_file_reference* info, unsigned long flags, unsigned long* error);
void file_path_add_extension(char*, long, char const*);
void file_path_add_name(char(&path)[256], char const* name);
void file_path_add_name(char* path, long maximum_path_length, char const* name);
void file_path_remove_name(char* path);
void file_path_split(char*, char**, char**, char**, char**, BOOL);
BOOL file_read(s_file_reference* info, unsigned long buffer_length, BOOL a3, void* buffer);
BOOL file_read_from_position(s_file_reference* info, unsigned long offset, unsigned long buffer_length, BOOL a4, void* buffer);
BOOL file_read_only(s_file_reference const* info);
s_file_reference* file_reference_create(s_file_reference* info, short location);
BOOL file_reference_get_file_handle(s_file_reference const* info, s_file_handle* file_handle);
BOOL file_rename(s_file_reference* info, char const*);
BOOL file_set_eof(s_file_reference* info, unsigned long offset);
BOOL file_set_position(s_file_reference* info, unsigned long, BOOL);
BOOL file_set_writeable(s_file_reference const* info, BOOL);
BOOL file_write(s_file_reference* info, unsigned long buffer_length, void const* buffer);
BOOL file_write_to_position(s_file_reference* info, unsigned long offset, unsigned long buffer_length, void const* buffer);
void find_files_end(s_find_file_data*);
BOOL find_files_next(s_find_file_data*, s_file_reference* info, s_file_last_modification_date*);
void find_files_start(s_find_file_data*, unsigned long, s_file_reference const* info);
void find_files_start_with_search_spec(s_find_file_data*, unsigned long, s_file_reference const* info, char const*);
void get_current_file_time(s_file_last_modification_date* out_time);


void directory_create_or_delete_contents(char const* directory)
{
	s_file_reference info;
	s_file_reference find_info;
	s_find_file_data find_file_data;

	file_reference_create(&info, -1);
	assert(info.signature == 'filo');
	assert((info.flags & ~3) != 0);
	assert(info.location > -1 && info.location < 2);
	assert(directory);
	assert((info.flags & (1 << 0)) != 0);

	file_path_add_name(info.path, directory);
	if (file_exists(&info))
	{
		find_files_start(&find_file_data, 0, &info);
		while (find_files_next(&find_file_data, &find_info, NULL))
		{
			file_delete(&find_info);
		}
		find_files_end(&find_file_data);
	}
	else
	{
		file_create(&info);
	}
}

BOOL file_create_parent_directories_if_not_present(s_file_reference const* info)
{
	throw UNIMPLEMENTED;
}


BOOL file_is_readable(s_file_reference* info, unsigned long* error)
{
	if (file_open(info, (1 << 0), error))
	{
		file_close(info);
		return TRUE;
	}

	return FALSE;
}

void file_location_set_volume(short location, char const* volume_name)
{
	throw UNIMPLEMENTED;
}


void file_printf(s_file_reference* info, char const* fmt, ...)
{
	char buffer[1024];
	va_list va;

	va_start(va, fmt);
	if (fmt)
	{
		sprintf_s(buffer, fmt, va);
		unsigned long buffer_length = (unsigned long)strlen(buffer);
		file_write(info, buffer_length, buffer);
	}
}

BOOL file_read_into_buffer(s_file_reference* info, void* buffer, unsigned long buffer_length)
{
	BOOL result = FALSE;
	assert(buffer);

	unsigned long error;
	if (file_open(info, (1 << 0), &error))
	{
		unsigned long eof = file_get_eof(info);
		if (eof <= buffer_length)
		{
			result = file_read(info, eof, 0, buffer);
		}
		file_close(info);
	}
	return result;
}

void* file_read_into_memory(s_file_reference* info, unsigned long* out_length)
{
	unsigned long buffer_length = -1;
	void* buffer = nullptr;

	unsigned long error;
	if (file_open(info, (1 << 0), &error))
	{
		buffer_length = file_get_eof(info);
		if (buffer_length)
		{
			buffer = malloc(buffer_length);
			assert(buffer);

			if (!file_read(info, buffer_length, 0, buffer))
			{
				free(buffer);
				buffer = nullptr;
			}
		}
		file_close(info);
	}

	*out_length = buffer_length;
	return buffer;
}

void* file_read_into_memory_with_null_padding(s_file_reference* info, unsigned long* out_length, unsigned long padding)
{
	unsigned long buffer_length = padding;
	void* buffer = nullptr;

	unsigned long error;
	if (file_open(info, (1 << 0), &error))
	{
		unsigned long offset = file_get_eof(info);

		buffer_length += offset;
		if (buffer_length)
		{
			buffer = malloc(buffer_length);
			if (buffer)
			{
				if (file_read(info, offset, 0, buffer))
				{
					if (padding)
					{
						memset((char*)buffer + offset, 0, padding);
						file_close(info);
						return buffer;
					}
				}
				else
				{
					free(buffer);
					buffer = nullptr;
				}
			}
		}
		file_close(info);
		return buffer;
	}

	*out_length = buffer_length;
	return buffer;
}

s_file_reference* file_reference_add_directory(s_file_reference* info, char const* name)
{
	assert(name);
	assert((info->flags & (1 << 0)) != 0);

	file_path_add_name(info->path, name);
	return info;
}

s_file_reference* file_reference_agnostic_create(s_file_reference* info, short location)
{
	assert(info);
	assert(location >= -1 && location <= 2);

	memset(info, 0, sizeof(s_file_reference));
	info->signature = 'filo';
	info->location = location;
	return info;
}

s_file_reference* file_reference_copy(s_file_reference* dst_info, s_file_reference const* src_info)
{
	return (s_file_reference*)memcpy(dst_info, src_info, offsetof(s_file_reference, handle));
}

s_file_reference* file_reference_create_from_path(s_file_reference* info, char const* name, BOOL add_directory)
{
	file_reference_create(info, -1);

	if (add_directory)
		file_reference_add_directory(info, name);
	else
		file_reference_set_name(info, name);
	return info;
}

char* file_reference_get_fullpath(s_file_reference const* info, char* out_full_path, long full_path_length)
{
	file_location_get_full_path(info->location, info->path, out_full_path, full_path_length);
	return out_full_path;
}

short file_reference_get_location(s_file_reference const* info)
{
	return info->location;
}

char* file_reference_get_name(s_file_reference const* info, unsigned long flags, char* out_name, long name_length)
{
	throw UNIMPLEMENTED;
}


char const* file_reference_get_path_for_debugging(s_file_reference const* info)
{
	return info->path;
}

s_file_reference* file_reference_remove_directory(s_file_reference* info)
{
	assert((info->flags & (1 << 0)) != 0);

	file_path_remove_name(info->path);
	return info;
}

s_file_reference* file_reference_remove_name(s_file_reference* info)
{
	if ((info->flags & (1 << 0)) != 0)
	{
		file_path_remove_name(info->path);
		info->flags &= ~(1 << 0);
	}
	return info;
}

s_file_reference* file_reference_set_name(s_file_reference* info, char const* name)
{
	assert(name);

	if ((info->flags & (1 << 0)) != 0)
		file_path_remove_name(info->path);
	file_path_add_name(info->path, 256, name);
	info->flags |= (1 << 0);
	return info;
}

BOOL file_references_equal(s_file_reference const* info, s_file_reference const* other)
{
	return info->location == other->location && strcmp(info->path, other->path) == 0;
}

void file_trim(s_file_reference* info, long)
{
	throw UNIMPLEMENTED;
}

long find_files(unsigned long, s_file_reference const*, long, s_file_reference*)
{
	throw UNIMPLEMENTED;
}

void file_activity_debug_render(void)
{
}

BOOL file_close(s_file_reference* info)
{
	if (!CloseHandle(info->handle.handle))
		return FALSE;

	info->handle.handle = INVALID_HANDLE_VALUE;
	info->api_result = 0;
	return TRUE;
}

int file_compare_last_modification_dates(s_file_last_modification_date const* date1, s_file_last_modification_date const* date2)
{
	assert(date1);
	assert(date2);

	return CompareFileTime(&date1->filetime, &date2->filetime);
}

BOOL file_copy_to(s_file_reference const* info, s_file_reference const* other, BOOL)
{
	throw UNIMPLEMENTED;
}

BOOL file_copy_to_recursive(s_file_reference* info, s_file_reference* other)
{
	throw UNIMPLEMENTED;
}

BOOL file_create(s_file_reference* info)
{
	throw UNIMPLEMENTED;
}

void file_date_format_for_output(s_file_last_modification_date* info, char* buffer, long buffer_length)
{
	SYSTEMTIME system_time;
	FileTimeToSystemTime(&info->filetime, &system_time);
	snprintf(buffer, buffer_length, "%d/%d/%d", system_time.wMonth, system_time.wDay, system_time.wYear);
}

BOOL file_delete(s_file_reference* info)
{
	throw UNIMPLEMENTED;
}

BOOL file_delete_recursive(s_file_reference* info)
{
	throw UNIMPLEMENTED;
}

BOOL file_exists(s_file_reference const* info)
{
	throw UNIMPLEMENTED;
}

BOOL file_get_creation_date(s_file_reference const* info, s_file_last_modification_date*)
{
	throw UNIMPLEMENTED;
}

unsigned long file_get_eof(s_file_reference const* info)
{
	LARGE_INTEGER file_size;
	GetFileSizeEx(info->handle.handle, &file_size);
	return file_size.LowPart;
}

BOOL file_get_last_modification_date(s_file_reference const* info, s_file_last_modification_date*)
{
	throw UNIMPLEMENTED;
}

unsigned long file_get_position(s_file_reference const* info)
{
	return info->api_result;
}

BOOL file_get_size(s_file_reference const* info, unsigned long*)
{
	throw UNIMPLEMENTED;
}

BOOL file_handle_is_valid(s_file_handle file_handle)
{
	return file_handle.handle != INVALID_HANDLE_VALUE;
}

BOOL file_is_directory(s_file_reference const* info)
{
	return (info->flags & (1 << 0)) == 0;
}

BOOL file_last_modification_date_to_time(s_file_last_modification_date const*, tm*)
{
	throw UNIMPLEMENTED;
}

void file_location_get_full_path(short, char const*, char*, long)
{
	throw UNIMPLEMENTED;
}

BOOL file_move_to(s_file_reference const* info, s_file_reference const* other)
{
	throw UNIMPLEMENTED;
}

BOOL file_open(s_file_reference* info, unsigned long flags, unsigned long* error)
{
	throw UNIMPLEMENTED;
}

void file_path_add_extension(char*, long, char const*)
{
	throw UNIMPLEMENTED;
}

void file_path_add_name(char(&path)[256], char const* name)
{
	file_path_add_name(path, 256, name);
}

void file_path_add_name(char* path, long maximum_path_length, char const* name)
{
	if (*name)
	{
		unsigned long out_name_size = (unsigned long)strlen(path);
		char* out_name = &path[out_name_size];
		if (out_name != path && *(out_name - 1) != '\\')
		{
			*out_name++ = '\\';
			++out_name_size;
			*out_name = 0;
		}
		strncpy_s(out_name, maximum_path_length, name, (unsigned long)(maximum_path_length - out_name_size));
	}
}

void file_path_remove_name(char* path)
{
	short i;
	for (i = (short)strlen(path); i > 0; --i)
	{
		if (path[i] == '\\')
			break;
	}
	path[i] = 0;
}

void file_path_split(char*, char**, char**, char**, char**, BOOL)
{
	throw UNIMPLEMENTED;
}

BOOL file_read(s_file_reference* info, unsigned long buffer_length, BOOL a3, void* buffer)
{
	assert(buffer);

	BOOL result = FALSE;
	DWORD bytes_read;
	if (ReadFile(info->handle.handle, buffer, buffer_length, &bytes_read, NULL))
	{
		if (bytes_read == buffer_length)
			result = TRUE;
		else
			SetLastError(ERROR_HANDLE_EOF);
	}

	info->api_result += bytes_read;
	return result;
}

BOOL file_read_from_position(s_file_reference* info, unsigned long offset, unsigned long buffer_length, BOOL a4, void* buffer)
{
	if (!file_set_position(info, offset, a4))
		return FALSE;

	return file_read(info, buffer_length, a4, buffer);
}

BOOL file_read_only(s_file_reference const* info)
{
	throw UNIMPLEMENTED;
}

s_file_reference* file_reference_create(s_file_reference* info, short location)
{
	file_reference_agnostic_create(info, location);
	assert(info);

	info->handle.handle = INVALID_HANDLE_VALUE;
	return info;
}

BOOL file_reference_get_file_handle(s_file_reference const* info, s_file_handle* file_handle)
{
	assert(info);
	assert(file_handle);

	file_handle->handle = info->handle.handle;
	return file_handle->handle != NULL;
}

BOOL file_rename(s_file_reference* info, char const*)
{
	throw UNIMPLEMENTED;
}

BOOL file_set_eof(s_file_reference* info, unsigned long offset)
{
	BOOL position = file_set_position(info, offset, 0);
	BOOL eof = SetEndOfFile(info->handle.handle);

	if (!position || (position = eof, !eof))
		return position;
	return position;
}

BOOL file_set_position(s_file_reference* info, unsigned long, BOOL)
{
	throw UNIMPLEMENTED;
}

BOOL file_set_writeable(s_file_reference const* info, BOOL)
{
	throw UNIMPLEMENTED;
}

BOOL file_write(s_file_reference* info, unsigned long buffer_length, void const* buffer)
{
	assert(buffer);

	BOOL result = FALSE;
	DWORD bytes_written;
	if (WriteFile(info->handle.handle, buffer, buffer_length, &bytes_written, NULL))
	{
		if (bytes_written == buffer_length)
			result = TRUE;
		else
			SetLastError(ERROR_HANDLE_EOF);
	}

	info->api_result += bytes_written;
	return result;
}

BOOL file_write_to_position(s_file_reference* info, unsigned long offset, unsigned long buffer_length, void const* buffer)
{
	if (!file_set_position(info, offset, 0))
		return FALSE;

	return file_write(info, buffer_length, buffer);
}

void find_files_end(s_find_file_data*)
{
	throw UNIMPLEMENTED;
}

BOOL find_files_next(s_find_file_data* find_file_data, s_file_reference* info, s_file_last_modification_date*)
{
	throw UNIMPLEMENTED;
}

void find_files_start(s_find_file_data* find_file_data, unsigned long, s_file_reference const* info)
{
	throw UNIMPLEMENTED;
}

void find_files_start_with_search_spec(s_find_file_data* find_file_data, unsigned long, s_file_reference const* info, char const*)
{
	throw UNIMPLEMENTED;
}

void get_current_file_time(s_file_last_modification_date* out_time)
{
	FILETIME system_time_as_file_time;
	GetSystemTimeAsFileTime(&system_time_as_file_time);

	memcpy(out_time, &system_time_as_file_time, sizeof(s_file_last_modification_date));
}
