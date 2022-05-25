#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

enum e_oddball_variant_team_scoring_settings
{
	_oddball_variant_team_scoring_settings_sum = 0,
	_oddball_variant_team_scoring_settings_minimum,
	_oddball_variant_team_scoring_settings_maximum,

	k_oddball_variant_team_scoring_settings
};

enum e_oddball_variant_flags
{
	_oddball_variant_flags_auto_ball_pickup = 0,
	_oddball_variant_flags_ball_effect_enabled,

	k_oddball_variant_flags
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_oddball_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_oddball_variant_team_scoring_settings, short, k_oddball_variant_team_scoring_settings> m_team_scoring

	c_flags<e_oddball_variant_flags, unsigned long, k_oddball_variant_flags> m_flags;

	short m_score_to_win;

	// points per second when carrying the ball
	short m_carrying_points;

	// 4 variables in 5
	/*
	kill_points

	// number of points for a melee kill with the ball
	ball_kill_points

	// number of points for killing the ball carrier
	carrier_kill_points

	ball_count
	*/

	short unknown0;
	char unknown1;
	char unknown2;
	char unknown3;
	char unknown4;

	// time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	// seconds
	short m_ball_spawn_delay;

	// time until ball respawns if it is inactive
	// seconds
	short m_ball_inactive_respawn;

	c_player_traits m_carrier_traits;

	char pad[2];
});

#pragma pack(pop)