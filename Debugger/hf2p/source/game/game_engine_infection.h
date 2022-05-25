#pragma once

#include <cseries/cseries.h>

#pragma pack(push, 1)

#pragma region enums

enum e_infection_variant_flags
{
	_infection_variant_flags_respawn_on_haven_move = 0,

	k_infection_variant_flags
};

enum e_infection_safe_havens_settings
{
	_infection_safe_havens_off = 0,
	_infection_safe_havens_sequential,
	_infection_safe_havens_random,

	k_infection_safe_havens_settings,
	k_infection_safe_havens_settings_default = _infection_safe_havens_off
};

enum e_infection_next_zombie_settings
{
	_infection_next_zombie_winner = 0,
	_infection_next_zombie_chump,
	_infection_next_zombie_unchanged,
	_infection_next_zombie_random,

	k_infection_next_zombie_settings,
	k_infection_next_zombie_settings_default = _infection_next_zombie_winner
};

enum e_infection_initial_zombie_count_settings
{
	_infection_initial_zombie_count_25_percent = 0,
	_infection_initial_zombie_count_50_percent,
	_infection_initial_zombie_count_1,
	_infection_initial_zombie_count_2,
	_infection_initial_zombie_count_3,
	_infection_initial_zombie_count_4,
	_infection_initial_zombie_count_5,
	_infection_initial_zombie_count_6,
	_infection_initial_zombie_count_7,
	_infection_initial_zombie_count_8,
	_infection_initial_zombie_count_9,
	_infection_initial_zombie_count_10,
	_infection_initial_zombie_count_11,
	_infection_initial_zombie_count_12,
	_infection_initial_zombie_count_13,
	_infection_initial_zombie_count_14,
	_infection_initial_zombie_count_15,

	k_infection_initial_zombie_count_settings,
	k_infection_initial_zombie_count_settings_default = _infection_initial_zombie_count_50_percent
};

#pragma endregion


DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_infection_variant, c_game_engine_base_variant,
{
	c_flags<e_infection_variant_flags, unsigned char, k_infection_variant_flags> m_flags;
	c_enum<e_infection_safe_havens_settings, char, k_infection_safe_havens_settings_default, k_infection_safe_havens_settings> m_safe_havens;
	c_enum<e_infection_next_zombie_settings, char, k_infection_next_zombie_settings_default, k_infection_next_zombie_settings> m_next_zombie;
	c_enum<e_infection_initial_zombie_count_settings, char, k_infection_initial_zombie_count_settings_default, k_infection_initial_zombie_count_settings> m_initial_zombie_count;
	
	// 0 is no movement
	// default: 30, max: 120
	short m_safe_haven_movement_time;
	
	// default: 1, max: 20
	char m_zombie_kill_points;
	
	// default: 0, max: 20
	char m_infection_points;
	
	// default: 0, max: 20
	char m_safe_haven_arrival_points;
	
	// default: -1, max: 20
	char m_suicide_points;
	
	// default: -1, max: 20
	char m_betrayal_points;
	
	// default: 0, max: 20
	char m_last_man_bonus_points;

	c_player_traits m_zombie_traits;
	c_player_traits m_first_zombie_traits;
	c_player_traits m_safe_haven_defender_traits;
	c_player_traits m_last_human_traits;

	unsigned char pad[4];

	unsigned char unused[0x10];
});

#pragma pack(pop)