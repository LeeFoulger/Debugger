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

// based off https://github.com/B1nGh0/FindPattern/blob/709154d5bf0d6d31944c5df7f3d5c79cc87a7d2f/findpattern.c
template<unsigned long k_size, unsigned long k_length>
unsigned char* find_pattern(unsigned char(&start)[k_size], const unsigned char(&pattern)[k_length], const char(&mask)[k_length + 1])
{
	unsigned long i = 0, j = 0;
	unsigned char fnd = 0;

	if (k_length >= k_size)
		return 0;

	for (i = 0; !fnd && i <= k_size - k_length; i++)
	{
		for (fnd = 1, j = 0; fnd && j < k_length; j++)
		{
			fnd &= mask[j] == '?' || pattern[j] == (start)[i + j];
		}
	}

	return fnd ? start + i - 1 : nullptr;
}