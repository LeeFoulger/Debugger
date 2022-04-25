#pragma once

void dissassemble_x86_32(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...));
void dissassemble_x86_64(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...));