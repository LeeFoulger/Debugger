#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_king_team_scoring_settings
{
	_king_team_scoring_settings_sum = 0,
	_king_team_scoring_settings_minimum,
	_king_team_scoring_settings_maximum,
	_king_team_scoring_settings_team_score,

	k_king_team_scoring_settings
};

enum e_king_variant_flags
{
	_king_variant_flags_opaque_hill = 0,

	k_king_variant_flags
};

enum e_king_moving_hill_settings
{
	_king_moving_hill_settings_off = 0,
	_king_moving_hill_settings_10_seconds,
	_king_moving_hill_settings_15_seconds,
	_king_moving_hill_settings_30_seconds,
	_king_moving_hill_settings_1_minute,
	_king_moving_hill_settings_2_minutes,
	_king_moving_hill_settings_3_minutes,
	_king_moving_hill_settings_4_minutes,
	_king_moving_hill_settings_5_minutes,

	k_king_moving_hill_settings,
	k_king_moving_hill_settings_default = _king_moving_hill_settings_30_seconds
};

enum e_king_moving_hill_order_settings
{
	_king_moving_hill_order_settings_random = 0,
	_king_moving_hill_order_settings_sequence,

	k_king_moving_hill_order_settings,
	k_king_moving_hill_order_settings_default = _king_moving_hill_order_settings_sequence
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_king_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_king_team_scoring_settings, short, k_king_team_scoring_settings> m_team_scoring

	c_flags<e_king_variant_flags, ulong, k_king_variant_flags> m_variant_flags;

	// default: 100
	// maximum: 1000
	short m_score_to_win;

	// halo online specific
	// default: 90
	// maximum: 1000
	short m_score_unknown;

	c_enum<e_king_moving_hill_settings, char, k_king_moving_hill_settings_default, k_king_moving_hill_settings> m_moving_hill;
	c_enum<e_king_moving_hill_order_settings, char, k_king_moving_hill_order_settings_default, k_king_moving_hill_order_settings> m_moving_hill_order;

	// default: 0
	// maximum: 20
	char m_uncontested_hill_bonus_points;

	// default: 0
	// maximum: 20
	char m_points_per_kill;

	// default: 0
	// maximum: 20
	char m_inside_hill_points;

	// default: 0
	// maximum: 20
	char m_outside_hill_points;

	c_player_traits m_inside_hill_traits_name;

	t_padding<6> pad;
});

#pragma pack(pop)