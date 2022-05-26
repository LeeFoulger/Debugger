#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_slayer_team_scoring_settings
{
	_slayer_team_scoring_settings_sum = 0,
	_slayer_team_scoring_settings_minimum,
	_slayer_team_scoring_settings_maximum,

	k_slayer_team_scoring_settings
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_slayer_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring_method` override
	// c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings> m_team_scoring

	// default: 25, max: 251
	short m_score_to_win;

	// default: 20, max: 250
	short m_score_unknown; // halo online specific

	// default: 1, max: 20
	short m_kill_points;

	// default: 0, max: 20
	char m_assist_points;

	// default: 0, max: 20
	char m_death_points;

	// default: -1, max: 20
	char m_suicide_points;

	// default: -1, max: 20
	char m_betrayal_points;

	// default: 0, max: 20
	char m_leader_killed_points;

	// default: 0, max: 20
	char m_elimination_points;

	// default: 0, max: 20
	char m_assassination_points;

	// default: 0, max: 20
	char m_headshot_points;

	// default: 0, max: 20
	char m_melee_points;

	// default: 0, max: 20
	char m_sticky_points;

	// default: 0, max: 20
	char m_splatter_points;

	// default: 0, max: 20
	char m_killing_spree_points;

	// leader team traits if teams enabled
	c_player_traits m_leader_traits;

	t_padding<2> pad;

	t_padding<0x60> unused;
});

#pragma pack(pop)