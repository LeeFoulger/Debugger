#pragma once

template<typename t_string_type, size_t k_length>
struct c_string
{
	t_string_type value[k_length];
};

template<size_t k_data_size>
struct c_bytes
{
	unsigned char value[k_data_size];
};

void csstrncpy(char* dest, size_t size_in_bytes, const char* src, size_t max_count);
void cswcsncpy(wchar_t* dest, size_t size_in_bytes, const wchar_t* src, size_t max_count);

bool string_starts(const char* s1, const char* s2);
bool string_ends(const char* s1, const char* s2);
