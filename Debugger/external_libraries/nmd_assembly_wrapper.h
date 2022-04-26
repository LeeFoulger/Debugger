#pragma once

void disassemble_x86_impl(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...));

bool instruction_startswith_impl(const unsigned char* buffer, const unsigned char* buffer_end, const char* _instruction);

template<size_t k_length>
void disassemble_x86(const unsigned char(&buffer)[k_length], int(print_func)(const char*, ...))
{
	disassemble_x86_impl(buffer, buffer + k_length, print_func);
}

template<size_t k_length>
bool instruction_startswith(const unsigned char(&buffer)[k_length], const char* search)
{
	return instruction_startswith_impl(buffer, buffer + k_length, search);
}
