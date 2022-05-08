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
bool pattern_to_bytes(s_byte* bytes, const char* pattern)
{
	char* start = const_cast<char*>(pattern);
	char* end = const_cast<char*>(pattern) + strlen(pattern);

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

	return true;
};
