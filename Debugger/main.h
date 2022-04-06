#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cassert>
#include <stdio.h>

#include <Psapi.h>
#include <tlhelp32.h>

#include "macros.h"
#include "logger.h"
#include "process.h"
#include "debugger.h"

void add_break_on_winmain(c_debugger* debugger, LPMODULEINFO module_info);
void add_breaks_following_winmain(c_debugger* debugger, LPMODULEINFO module_info);
void add_test_breaks(c_debugger* debugger, LPMODULEINFO module_info);

void on_command_line_get_credentials_breakpoint(c_debugger* debugger, c_registers* registers);

void on_restricted_region_add_member_internal_breakpoint(c_debugger* debugger, c_registers* registers);
void on_rasterizer_draw_watermark_breakpoint(c_debugger* debugger, c_registers* registers);

void on_cached_map_files_open_all_breakpoint(c_debugger* debugger, c_registers* registers);

void on_main_game_load_map_breakpoint(c_debugger* debugger, c_registers* registers);

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count);
void cswcsncpy(wchar_t* dest, rsize_t size_in_bytes, const wchar_t* src, rsize_t max_count);

template<typename t_string_type, size_t k_string_size>
using c_string = t_string_type[k_string_size];
