#pragma once
#pragma pack(push, 1)

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_health_traits,
{
	unsigned char m_damage_resistance;
	unsigned char m_shield_recharge_rate;
	unsigned char m_shield_vampirism;
	unsigned char m_headshot_immunity;
	unsigned char m_shield_multiplier;
	unsigned char __unknown5;
	unsigned char __unknown6;
	unsigned char __unknown7;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_weapon_traits,
{
	unsigned short m_grenade_count;
	unsigned char m_primary_weapon;
	unsigned char m_secondary_weapon;
	unsigned char m_damage_modifier;
	unsigned char m_grenade_recharge;
	unsigned char m_infinite_ammo;
	unsigned char m_weapon_pickup;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_movement_traits,
{
	unsigned char m_player_speed;
	unsigned char m_player_gravity;
	unsigned char m_vehicle_use;
	unsigned char __unknown4;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_appearance_traits,
{
	unsigned char m_active_camo;
	unsigned char m_waypoint;
	unsigned char m_aura;
	unsigned char m_forced_color;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_sensor_traits,
{
	unsigned short m_motion_tracker_mode;
	unsigned short m_motion_tracker_range;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1C, c_player_traits,
{
	c_player_health_traits m_health_traits;
	c_player_weapon_traits m_weapon_traits;
	c_player_movement_traits m_movement_traits;
	c_player_appearance_traits m_appearance_traits;
	c_player_sensor_traits m_sensor_traits;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_miscellaneous_options_definition,
{
	unsigned char m_flags;
	unsigned char m_time_limit;
	unsigned char m_number_of_rounds;
	unsigned char m_early_victory_win_count;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x28, c_game_engine_respawn_options_definition,
{
	unsigned char m_flags;
	unsigned char m_lives_per_round;
	unsigned char m_shared_team_lives;
	unsigned char m_respawn_time;
	unsigned char m_suicide_penalty;
	unsigned char m_betrayal_penalty;
	unsigned char m_unknown_penalty;
	unsigned char m_respawn_time_growth;
	unsigned char m_respawn_trait_duration;
	unsigned short __unknown9;
	unsigned char __unknownB;
	c_player_traits m_respawn_trait_profile;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_social_options_definition,
{
	unsigned short m_flags;
	unsigned short m_team_changing;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x7C, c_game_engine_map_override_options_definition,
{
	unsigned long m_flags;
	c_player_traits m_base_traits;
	unsigned short m_weapon_set;
	unsigned short m_vehicle_set;
	c_player_traits m_red_traits;
	c_player_traits m_blue_traits;
	c_player_traits m_yellow_traits;
	unsigned char m_red_traits_duration;
	unsigned char m_blue_traits_duration;
	unsigned char m_yellow_traits_duration;

	unsigned char __unknwon7B;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1D0, c_game_engine_base_variant,
{
	unsigned long __vftable;
	unsigned long __unknown4;

	char m_variant_backend_name[32];
	char m_metadata[0xF8];

	c_game_engine_miscellaneous_options_definition m_miscellaneous_options;
	c_game_engine_respawn_options_definition m_respawn_options;
	c_game_engine_social_options_definition m_social_options;
	c_game_engine_map_override_options_definition m_map_override_options;

	unsigned short m_flags;
	short m_team_scoring;
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_ctf_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_slayer_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_oddball_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_king_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x1F0, c_game_engine_sandbox_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x20];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x238, c_game_engine_vip_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x68];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_juggernaut_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x210, c_game_engine_territories_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x40];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x228, c_game_engine_assault_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x58];
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x250, c_game_engine_infection_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x80];
});

union c_game_engine_variant
{
	// data for size correctness
	char __data[0x260];

	c_game_engine_base_variant m_base_variant;
	c_game_engine_ctf_variant m_ctf_variant;
	c_game_engine_slayer_variant m_slayer_variant;
	c_game_engine_oddball_variant m_oddball_variant;
	c_game_engine_king_variant m_king_variant;
	c_game_engine_sandbox_variant m_sandbox_variant;
	c_game_engine_vip_variant m_vip_variant;
	c_game_engine_juggernaut_variant m_juggernaut_variant;
	c_game_engine_territories_variant m_territories_variant;
	c_game_engine_assault_variant m_assault_variant;
	c_game_engine_infection_variant m_infection_variant;
};

enum e_game_engine_variant : unsigned long
{
	_game_engine_base_variant = 0,
	_game_engine_ctf_variant,
	_game_engine_slayer_variant,
	_game_engine_oddball_variant,
	_game_engine_king_variant,
	_game_engine_sandbox_variant,
	_game_engine_vip_variant,
	_game_engine_juggernaut_variant,
	_game_engine_territories_variant,
	_game_engine_assault_variant,
	_game_engine_infection_variant,

	k_game_engine_variant_count,
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x264, c_game_variant,
{
	e_game_engine_variant m_game_engine_index;
	c_game_engine_variant m_variant;
});

#pragma pack(pop)