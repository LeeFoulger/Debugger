#pragma once

#define PE32_BASE 0x00400000
#define PE64_BASE 0x0000000140000000
#define PE64_DLL_BASE 0x0000000180000000

#define NUMBER_OF(value) sizeof(value)/sizeof(value[0])

#define SAFE_DELETE(address) { delete[] address; address = nullptr; }

#define RUNONCE(once, ...) static bool once = false; if (once) { __VA_ARGS__ } once = true

#define TODO(...)