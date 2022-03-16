#pragma once

#include <windows.h>

#include <cassert>
#include <stdio.h>

#include <Psapi.h>
#include <tlhelp32.h>

#include "process.h"
#include "debugger.h"

void csstrncpy(char* dest, rsize_t size_in_bytes, const char* src, rsize_t max_count);
