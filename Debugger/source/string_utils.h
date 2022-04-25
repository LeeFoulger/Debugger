#pragma once

template<typename t_string_type, size_t k_string_size>
using c_string = t_string_type[k_string_size];

void csstrncpy(char* dest, size_t size_in_bytes, const char* src, size_t max_count);
void cswcsncpy(wchar_t* dest, size_t size_in_bytes, const wchar_t* src, size_t max_count);

bool string_starts(const char* s1, const char* s2);
bool string_ends(const char* s1, const char* s2);
