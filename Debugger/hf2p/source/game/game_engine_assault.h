#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_assault_variant_flags
{
	_assault_variant_flags_reset_bomb_on_disarm = 0,

	k_assault_variant_flags
};

enum e_assault_respawn_settings
{
	_assault_respawn_settings_disabled = 0,
	_assault_respawn_settings_on_friendly_detonation,
	_assault_respawn_settings_on_enemy_detonation,
	_assault_respawn_settings_on_any_detonation,

	k_assault_respawn_settings,
	k_assault_respawn_settings_default = _assault_respawn_settings_disabled
};

enum e_assault_game_type_settings
{
	_assault_game_type_settings_multi_bomb = 0,
	_assault_game_type_settings_one_bomb,
	_assault_game_type_settings_neutral_bomb,

	k_assault_game_type_settings,
	k_assault_game_type_settings_default = _assault_game_type_settings_multi_bomb
};

enum e_assault_enemy_bomb_waypoint_settings
{
	_assault_enemy_bomb_waypoint_settings_never = 0,
	_assault_enemy_bomb_waypoint_settings_when_not_carried_by_enemy,
	_assault_enemy_bomb_waypoint_settings_when_armed,
	_assault_enemy_bomb_waypoint_settings_alway,

	k_assault_enemy_bomb_waypoint_settings,
	k_assault_enemy_bomb_waypoint_settings_default = _assault_enemy_bomb_waypoint_settings_when_not_carried_by_enemy
};

enum e_assault_variant_sudden_death_time
{
	_assault_variant_sudden_death_time_infinite = -1,
	_assault_variant_sudden_death_time_off = 0,
	_assault_variant_sudden_death_time_15_seconds = 15,
	_assault_variant_sudden_death_time_1_minute = 60,
	_assault_variant_sudden_death_time_2_minute = 120,
	_assault_variant_sudden_death_time_5_minute = 300,

	k_assault_variant_sudden_death_time,
	k_assault_variant_sudden_death_time_default = _assault_variant_sudden_death_time_off
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_assault_variant, c_game_engine_base_variant,
{
	c_flags<e_assault_variant_flags, unsigned short, k_assault_variant_flags> m_flags;

	// default: `disabled`
	c_enum<e_assault_respawn_settings, short, k_assault_respawn_settings_default, k_assault_respawn_settings> m_respawn;

	// default: `multi bomb`
	c_enum<e_assault_game_type_settings, short, k_assault_game_type_settings_default, k_assault_game_type_settings> m_game_type;

	// default: `when not carried by enemy`
	c_enum<e_assault_enemy_bomb_waypoint_settings, short, k_assault_enemy_bomb_waypoint_settings_default, k_assault_enemy_bomb_waypoint_settings> m_enemy_bomb_waypoint;

	// default: 3, max: 50
	short m_score_to_win;

	// default: 2, max: 50
	short m_score_unknown0; // halo online specific

	// default: 0, max: 50
	short m_score_unknown1; // halo online specific

	// default: 0, max: 50
	short m_score_unknown2; // halo online specific

	// default: 0, max: 50
	short m_score_unknown3; // halo online specific

	// default: `off`
	c_enum<e_assault_variant_sudden_death_time, short, k_assault_variant_sudden_death_time_default, k_assault_variant_sudden_death_time> m_sudden_death_time;

	// default: 30, max: 90
	short m_bomb_reset_time; // seconds

	// default: 35, max: 90
	short m_bomb_arming_time; // seconds

	// default: 35, max: 90
	short m_bomb_disarming_time; // seconds

	// default: 35, max: 90
	short m_bomb_fuse_time; // seconds

	c_player_traits m_carrier_traits;
	c_player_traits m_arming_traits;

	unsigned char pad[4];

	unsigned char unused[0x38];
});

#pragma pack(pop)