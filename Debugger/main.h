#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cassert>
#include <Psapi.h>
#include <shellapi.h>
#include <stdio.h>
#include <tlhelp32.h>

#include "macros.h"
#include "process.h"
#include "debugger.h"
#include "breakpoints/breakpoints.h"

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count);
void cswcsncpy(wchar_t* dest, rsize_t size_in_bytes, const wchar_t* src, rsize_t max_count);

template<typename t_string_type, size_t k_string_size>
using c_string = t_string_type[k_string_size];
