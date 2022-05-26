#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_juggernaut_initial_juggernaut_settings
{
	_juggernaut_initial_juggernaut_settings_random = 0,
	_juggernaut_initial_juggernaut_settings_first_kill,
	_juggernaut_initial_juggernaut_settings_first_death,

	k_juggernaut_initial_juggernaut_settings,
	k_juggernaut_initial_juggernaut_settings_default = _juggernaut_initial_juggernaut_settings_random
};

enum e_juggernaut_next_juggernaut_settings
{
	_juggernaut_next_juggernaut_settings_on_killing_juggernaut = 0,
	_juggernaut_next_juggernaut_settings_on_killed_by_juggernaut,
	_juggernaut_next_juggernaut_settings_unchanged,
	_juggernaut_next_juggernaut_settings_random,

	k_juggernaut_next_juggernaut_settings,
	k_juggernaut_next_juggernaut_settings_default = _juggernaut_next_juggernaut_settings_on_killing_juggernaut
};

enum e_juggernaut_flags
{
	_juggernaut_flags_allied_against_juggernaut = 0,
	_juggernaut_flags_respawn_on_lone_juggernaut,
	_juggernaut_flags_destination_zones_enabled,

	k_juggernaut_flags
};

enum e_juggernaut_zone_movement_settings
{
	_juggernaut_zone_movement_settings_off = 0,
	_juggernaut_zone_movement_settings_10_seconds,
	_juggernaut_zone_movement_settings_15_seconds,
	_juggernaut_zone_movement_settings_30_seconds,
	_juggernaut_zone_movement_settings_1_minute,
	_juggernaut_zone_movement_settings_2_minutes,
	_juggernaut_zone_movement_settings_3_minutes,
	_juggernaut_zone_movement_settings_4_minutes,
	_juggernaut_zone_movement_settings_5_minutes,
	_juggernaut_zone_movement_settings_on_arrival,
	_juggernaut_zone_movement_settings_on_switch,

	k_juggernaut_zone_movement_settings,
	k_juggernaut_zone_movement_settings_default = _juggernaut_zone_movement_settings_off
};

enum e_juggernaut_zone_order_settings
{
	_juggernaut_zone_order_settings_random = 0,
	_juggernaut_zone_order_settings_sequence,

	k_juggernaut_zone_order_settings,
	k_juggernaut_zone_order_settings_default = _juggernaut_zone_order_settings_random
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_juggernaut_variant, c_game_engine_base_variant,
{
	// default: 15, max: 500
	short m_score_to_win_round;

	// default: 13, max: 500
	short m_score_unknown;

	// always set to '0'
	short m_unknown;

	c_enum<e_juggernaut_initial_juggernaut_settings, char, k_juggernaut_initial_juggernaut_settings_default, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, char, k_juggernaut_next_juggernaut_settings_default, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_flags, uchar, k_juggernaut_flags> m_flags;
	c_enum<e_juggernaut_zone_movement_settings, char, k_juggernaut_zone_movement_settings_default, k_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, char, k_juggernaut_zone_order_settings_default, k_juggernaut_zone_order_settings> m_zone_order;

	// default: 0, max: 20
	char m_kill_points;

	// default: 1, max: 20
	char m_juggeraut_kill_points;

	// default: 1, max: 20
	char m_kill_as_juggernaut_points;

	// default: 1, max: 20
	char m_destination_arrival_points;

	// default: 1, max: 20
	char m_suicide_points;

	// default: 1, max: 20
	char m_betrayal_points;

	// default: 0, max: 10
	char m_juggernaut_delay;

	c_player_traits m_juggernaut_traits;

	t_padding<2> pad;

	t_padding<0x60> unused;
});

#pragma pack(pop)