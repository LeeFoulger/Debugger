#pragma once

void dissassemble_x86(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...));

bool instruction_startswith(const unsigned char* buffer, const unsigned char* buffer_end, const char* _instruction);