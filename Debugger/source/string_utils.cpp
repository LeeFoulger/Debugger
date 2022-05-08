#include "string_utils.h"

#include <string.h>
#include <stdlib.h>

void csstrncpy(char* dest, size_t size_in_bytes, const char* src, size_t max_count)
{
	strncpy_s(dest, size_in_bytes, src, max_count);
	memset(dest + strlen(src), 0, (max_count - strlen(src)) * sizeof(char));
}

void cswcsncpy(wchar_t* dest, size_t size_in_words, const wchar_t* src, size_t max_count)
{
	wcsncpy_s(dest, size_in_words, src, max_count);
	memset(dest + wcslen(src), 0, (max_count - wcslen(src)) * sizeof(wchar_t));
}

bool string_starts(const char* s1, const char* s2)
{
	return strstr(s1, s2) == s1;
}

bool string_ends(const char* s1, const char* s2)
{
	return strstr(s1, s2) == s1 + (strlen(s1) - strlen(s2));
}

// https://github.com/OneshotGH/CSGOSimple-master/blob/master/CSGOSimple/helpers/utils.cpp
// bytes separated by <space>
// must not end with <space>
bool pattern_to_bytes(s_byte** out_bytes, unsigned long* out_byte_count, const char* pattern)
{
	if (!out_bytes || !out_byte_count)
		return false;

	unsigned long length = (unsigned long)strlen(pattern);

	unsigned long byte_count = 1;
	for (unsigned long i = 0; i < length; i++)
		byte_count += pattern[i] == ' ';

	s_byte* bytes = new s_byte[byte_count]{};

	char* start = const_cast<char*>(pattern);
	char* end = const_cast<char*>(pattern) + length;

	unsigned long index = 0;
	for (char* current = start; current < end; ++current, index++)
	{
		if (*current == '?')
		{
			++current;
			if (*current == '?')
				++current;

			bytes[index].value = -1;
			bytes[index].masked = true;
		}
		else
		{
			bytes[index].value = (unsigned char)strtoul(current, &current, 16);
			bytes[index].masked = false;
		}
	}

	*out_bytes = bytes;
	*out_byte_count = byte_count;
	return true;
};
