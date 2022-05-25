#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

enum e_slayer_team_scoring_settings
{
	_slayer_team_scoring_settings_sum = 0,
	_slayer_team_scoring_settings_minimum,
	_slayer_team_scoring_settings_maximum,

	k_slayer_team_scoring_settings
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_slayer_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring` override
	// c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings> m_team_scoring

	short m_score_to_win;
	short m_kill_points;
	short m_assist_points;
	char m_death_points;
	char m_suicide_points;
	char m_betrayal_points;
	char m_leader_kill_points;
	char m_elimination_points;
	char m_assassination_bonus_points;
	char m_headshot_bonus_points;
	char m_melee_bonus_points;
	char m_sticky_bonus_points;
	char m_splatter_bonus_points;
	char m_killing_spree_bonus_points;

	char unknown0;

	c_player_traits m_leader_player_traits;

	char slayer_padding[2];
});

#pragma pack(pop)