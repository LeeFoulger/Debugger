#pragma once

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <cassert>
#include <Psapi.h>
#include <shellapi.h>
#include <stdio.h>
#include <tlhelp32.h>

// solution files
#include <macros.h>
#include <process.h>
#include <debugger.h>
#include <breakpoints/breakpoints.h>

// external library wrappers
#include <nmd_assembly_wrapper.h>
