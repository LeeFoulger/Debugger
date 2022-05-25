#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

enum e_juggernaut_initial_juggernaut_settings
{
	_juggernaut_initial_juggernaut_settings_random = 0,
	_juggernaut_initial_juggernaut_settings_first_kill,
	_juggernaut_initial_juggernaut_settings_first_death,

	k_juggernaut_initial_juggernaut_settings
};

enum e_juggernaut_next_juggernaut_settings
{
	_juggernaut_next_juggernaut_settings_on_killing_juggernaut = 0,
	_juggernaut_next_juggernaut_settings_on_killed_by_juggernaut,
	_juggernaut_next_juggernaut_settings_unchanged,
	_juggernaut_next_juggernaut_settings_random,

	k_juggernaut_next_juggernaut_settings
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

	k_juggernaut_zone_movement_settings
};

enum e_juggernaut_zone_order_settings
{
	_juggernaut_zone_order_settings_random = 0,
	_juggernaut_zone_order_settings_sequence,

	k_juggernaut_zone_order_settings
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_juggernaut_variant, c_game_engine_base_variant,
{
	// one of these is `score to win round`
	short m_score_to_win0;
	short m_score_to_win1;

	short unknown0;

	c_enum<e_juggernaut_initial_juggernaut_settings, char, k_juggernaut_initial_juggernaut_settings> m_initial_juggernaut;
	c_enum<e_juggernaut_next_juggernaut_settings, char, k_juggernaut_next_juggernaut_settings> m_next_juggernaut;
	c_flags<e_juggernaut_flags, unsigned char, k_juggernaut_flags> m_flags;
	c_enum<e_juggernaut_zone_movement_settings, char, k_juggernaut_zone_movement_settings> m_zone_movement;
	c_enum<e_juggernaut_zone_order_settings, char, k_juggernaut_zone_order_settings> m_zone_order;

	char m_kill_points;
	char m_juggeraut_kill_points;
	char m_kill_as_juggernaut_points;
	char m_destination_arrival_points;
	char m_suicide_points;
	char m_betrayal_points;
	char m_juggernaut_delay;

	c_player_traits m_juggernaut_traits;

	char juggernaut_padding[2];
});

#pragma pack(pop)