#pragma once

#include <cseries/cseries.h>

#pragma pack(push, 1)

enum e_ctf_variant_flags
{
	_ctf_variant_flag_at_home_to_score_flag,

	k_ctf_variant_flags
};

enum e_ctf_variant_home_flag_waypoint_settings
{
	_ctf_variant_home_flag_waypoint_settings_never,
	_ctf_variant_home_flag_waypoint_settings_when_not_carried_by_enemy,
	_ctf_variant_home_flag_waypoint_settings_away_from_home,
	_ctf_variant_home_flag_waypoint_settings_always,

	k_ctf_variant_home_flag_waypoint_settings
};

enum e_ctf_variant_game_type_settings
{
	_ctf_variant_game_type_settings_multi_flag,
	_ctf_variant_game_type_settings_single_flag,
	_ctf_variant_game_type_settings_neutral_flag,

	k_ctf_variant_game_type_settings
};

enum e_ctf_variant_respawn_settings
{
	_ctf_variant_respawn_settings_normal,
	_ctf_variant_respawn_settings_on_friendly_capture,
	_ctf_variant_respawn_settings_on_enemy_capture,
	_ctf_variant_respawn_settings_on_any_capture,

	k_ctf_variant_respawn_settings
};

enum e_ctf_variant_touch_return_settings
{
	_ctf_variant_touch_return_settings_off = -1,
	_ctf_variant_touch_return_settings_instant = 0,
	_ctf_variant_touch_return_settings_5_seconds = 5,
	_ctf_variant_touch_return_settings_10_seconds = 10,
	_ctf_variant_touch_return_settings_15_seconds = 15,
	_ctf_variant_touch_return_settings_20_seconds = 20,
	_ctf_variant_touch_return_settings_30_seconds = 30,
	_ctf_variant_touch_return_settings_45_seconds = 45,
	_ctf_variant_touch_return_settings_1_minute = 60,

	k_ctf_variant_touch_return_settings
};

enum e_ctf_variant_sudden_death_time
{
	_ctf_variant_sudden_death_time_infinite = -1,
	_ctf_variant_sudden_death_time_off = 0,
	_ctf_variant_sudden_death_time_15_seconds = 15,
	_ctf_variant_sudden_death_time_1_minute = 60,
	_ctf_variant_sudden_death_time_2_minute = 120,
	_ctf_variant_sudden_death_time_5_minute = 300,

	k_ctf_variant_sudden_death_times
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_ctf_variant, c_game_engine_base_variant,
{
	c_flags<e_ctf_variant_flags, unsigned char, k_ctf_variant_flags> m_flags;

	c_enum<e_ctf_variant_home_flag_waypoint_settings, char, k_ctf_variant_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_variant_game_type_settings, char, k_ctf_variant_game_type_settings> m_game_type;
	c_enum<e_ctf_variant_respawn_settings, char, k_ctf_variant_respawn_settings> m_capture_respawn;
	c_enum<e_ctf_variant_touch_return_settings, short, k_ctf_variant_touch_return_settings> m_touch_return_time;
	c_enum<e_ctf_variant_sudden_death_time, short, k_ctf_variant_sudden_death_times> m_sudden_death_time;

	short m_score_to_win;
	short m_flag_reset_time;

	short unknown;

	c_player_traits carrier_traits;

	char pad[6];
});

#pragma pack(pop)