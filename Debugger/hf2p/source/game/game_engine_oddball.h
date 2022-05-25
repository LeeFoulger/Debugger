#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

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

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_oddball_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_oddball_variant_team_scoring_settings, short, k_oddball_variant_team_scoring_settings> m_team_scoring

	c_flags<e_oddball_variant_flags, unsigned long, k_oddball_variant_flags> m_flags;

	// default: 200, max: 1001
	short m_score_to_win;

	// points per second when carrying the ball
	// default: 180, max: 1000
	short m_carrying_points;

	// default: 1, max: 20
	short m_unknown; // halo online specific

	// default: 0, max: 20
	char m_kill_points;

	// number of points for a melee kill with the ball
	// default: 0, max: 20
	char m_ball_kill_points;

	// number of points for killing the ball carrier
	// default: 0, max: 20
	char m_carrier_kill_points;

	// default: 1, max: 2
	char m_ball_count;

	// time until first ball spawn after round start, and also delay in respawning ball if it goes out of play
	// default: 5, max: 120
	short m_ball_spawn_delay; // seconds

	// time until ball respawns if it is inactive
	// default: 30, max: 120
	short m_ball_inactive_respawn; // seconds

	c_player_traits m_carrier_traits;

	unsigned char pad[2];

	unsigned char unused[0x60];
});

#pragma pack(pop)