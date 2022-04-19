#pragma once

void add_break_on_winmain(c_debugger&, LPMODULEINFO);
void add_breaks_following_winmain(c_debugger&, LPMODULEINFO);
void add_test_breaks(c_debugger&, LPMODULEINFO);

void on_command_line_get_credentials_breakpoint(c_debugger&, c_registers&);
void on_restricted_region_add_member_internal_breakpoint(c_debugger&, c_registers&);
void on_rasterizer_draw_watermark_breakpoint(c_debugger&, c_registers&);
void on_cached_map_files_open_all_breakpoint(c_debugger&, c_registers&);
void on_main_game_load_map_breakpoint(c_debugger&, c_registers&);

void on_is_debugger_present_breakpoint(c_debugger&, c_registers&);
void on_shell_screen_pause_breakpoint(c_debugger&, c_registers&);
void on_shell_get_external_host_breakpoint(c_debugger&, c_registers&);
void on_restricted_region_add_member_breakpoint(c_debugger&, c_registers&);
void on_shell_get_system_identifier_breakpoint(c_debugger&, c_registers&);
void on_shell_get_gamertag_return_breakpoint(c_debugger&, c_registers&);