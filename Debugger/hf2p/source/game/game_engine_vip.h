#pragma once

#include <cseries/cseries.h>

#pragma pack(push, 1)

#pragma region enums

enum e_vip_flags
{
	_vip_flags_single_vip = 0,
	_vip_flags_destination_zones_enabled,
	_vip_flags_end_round_on_vip_death,

	k_vip_flags,
	k_vip_flags_default
};

enum e_vip_selection_settings
{
	_vip_selection_settings_random,
	_vip_selection_settings_next_respawn,
	_vip_selection_settings_next_kill,
	_vip_selection_settings_unchanged,

	k_vip_selection_settings,
	k_vip_selection_settings_default = _vip_selection_settings_random
};

enum e_vip_zone_movement_settings
{
	_vip_zone_movement_settings_off,
	_vip_zone_movement_settings_10_seconds,
	_vip_zone_movement_settings_15_seconds,
	_vip_zone_movement_settings_30_seconds,
	_vip_zone_movement_settings_1_minute,
	_vip_zone_movement_settings_2_minutes,
	_vip_zone_movement_settings_3_minutes,
	_vip_zone_movement_settings_4_minutes,
	_vip_zone_movement_settings_5_minutes,
	_vip_zone_movement_settings_on_arrival,
	_vip_zone_movement_settings_on_switch,

	k_vip_zone_movement_settings,
	k_vip_zone_movement_settings_default = _vip_zone_movement_settings_off
};

enum e_vip_zone_order_settings
{
	_vip_zone_order_settings_random,
	_vip_zone_order_settings_sequence,

	k_vip_zone_order_settings,
	k_vip_zone_order_settings_default = _vip_zone_order_settings_sequence
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_vip_variant, c_game_engine_base_variant,
{
	// default: 15
	// maximum: 500
	short m_score_to_win_round;

	// default: 10
	// maximum: 500
	short m_score_unknown; // halo online specific

	c_flags<e_vip_flags, ushort, k_vip_flags> m_flags;

	// default: 0
	// maximum: 20
	char m_kill_points;

	// default: 0
	// maximum: 20
	char m_takedown_points;

	// default: 0
	// maximum: 20
	char m_kill_as_vip_points;

	// default: 0
	// maximum: 20
	char m_vip_death_points;

	// default: 0
	// maximum: 20
	char m_destination_arrival_points;

	// default: 0
	// maximum: 20
	char m_suicide_points;

	// default: 0
	// maximum: 20
	char m_betrayal_points;

	// default: 0
	// maximum: 20
	char m_vip_suicide_points;

	c_enum<e_vip_selection_settings, char, k_vip_selection_settings_default, k_vip_selection_settings> m_vip_selection;
	c_enum<e_vip_zone_movement_settings, char, k_vip_zone_movement_settings_default, k_vip_zone_movement_settings> m_zone_movement;
	c_enum<e_vip_zone_order_settings, char, k_vip_zone_order_settings_default, k_vip_zone_order_settings> m_zone_order;

	t_padding<1> pad;

	// default: 0
	// maximum: 50
	short m_influence_radius;

	c_player_traits m_vip_team_traits;
	c_player_traits m_vip_influence_traits;
	c_player_traits m_vip_traits;

	t_padding<0x28> unused;
});

#pragma pack(pop)