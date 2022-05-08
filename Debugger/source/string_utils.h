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

struct s_byte
{
	unsigned char value;
	bool masked;
};

bool pattern_to_bytes(s_byte** bytes, unsigned long* byte_count, const char* pattern);

// https://github.com/B1nGh0/FindPattern/blob/709154d5bf0d6d31944c5df7f3d5c79cc87a7d2f/findpattern.c
// https://github.com/OneshotGH/CSGOSimple-master/blob/master/CSGOSimple/helpers/utils.cpp
template<unsigned long k_size, unsigned long k_length>
unsigned long find_pattern(unsigned char(&start)[k_size], const char(&signature)[k_length])
{
	s_byte* bytes = nullptr;
	unsigned long byte_count = 0;
	if (!pattern_to_bytes(&bytes, &byte_count, signature))
		return 'nope';

	unsigned long i = 0, j = 0;
	unsigned char fnd = 0;

	for (i = 0; i < k_size - byte_count; ++i)
	{
		for (fnd = 1, j = 0; j < byte_count; ++j)
			fnd &= bytes[j].masked == true || bytes[j].value == start[i + j];

		if (fnd)
			return i;
	}

	return 'nope';
}
