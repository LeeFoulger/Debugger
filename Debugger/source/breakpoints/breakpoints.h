#pragma once

void create_debugger_additions(c_debugger&);

void add_break_on_winmain(c_debugger&, LPMODULEINFO);
void add_breaks_following_winmain(c_debugger&, LPMODULEINFO);
void add_test_breaks(c_debugger&, LPMODULEINFO);

void on_command_line_get_credentials_breakpoint(c_debugger&, c_registers&);

void on_restricted_region_add_member_internal_breakpoint(c_debugger&, c_registers&);
void on_rasterizer_draw_watermark_breakpoint(c_debugger&, c_registers&);
void on_machinima_camera_debug_breakpoint(c_debugger&, c_registers&);
void on_cache_file_blocking_read_breakpoint(c_debugger&, c_registers&);
void on_cache_files_verify_header_rsa_signature_breakpoint(c_debugger&, c_registers&);
void on_contrails_render_callback_breakpoint(c_debugger&, c_registers&);
void on_main_kick_startup_masking_sequence_breakpoint(c_debugger&, c_registers&);

void on_cached_map_files_open_all_breakpoint(c_debugger&, c_registers&);
void on_main_game_load_map_breakpoint(c_debugger&, c_registers&);

void on_is_debugger_present_breakpoint(c_debugger&, c_registers&);
void on_shell_screen_pause_breakpoint(c_debugger&, c_registers&);
void on_shell_get_external_host_breakpoint(c_debugger&, c_registers&);
void on_restricted_region_add_member_breakpoint(c_debugger&, c_registers&);
void on_shell_get_system_identifier_breakpoint(c_debugger&, c_registers&);
void on_shell_get_gamertag_return_breakpoint(c_debugger&, c_registers&);
void on_string_id_get_string_const_breakpoint(c_debugger&, c_registers&);