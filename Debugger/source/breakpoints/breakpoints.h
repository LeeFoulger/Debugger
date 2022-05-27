#pragma once

void create_debugger_additions(c_debugger&);

void add_break_on_winmain(c_debugger&, LPMODULEINFO);
void add_breaks_following_winmain(c_debugger&, LPMODULEINFO);
void add_test_breaks(c_debugger&, LPMODULEINFO);

void on_command_line_get_credentials_breakpoint(c_debugger&, c_registers&);
