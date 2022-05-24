#pragma once
#pragma pack(push, 1)

// names and comments from halo 3 tools

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_trait_shield_vitality,
{
	unsigned char m_damage_resistance;
	unsigned char m_shield_recharge_rate;
	unsigned char m_shield_vampirism;
	unsigned char m_headshot_immunity;
	unsigned char m_shield_multiplier;

	// elephant
	unsigned char pad[3];
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x8, c_player_trait_weapons,
{
	unsigned short m_initial_grenade_count;
	unsigned char m_initial_primary_weapon;
	unsigned char m_initial_secondary_weapon;
	unsigned char m_damage_modifier;
	unsigned char m_recharging_grenades;
	unsigned char m_infinite_ammo;
	unsigned char m_weapon_pickup;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_trait_movement,
{
	unsigned char m_speed_multiplier;
	unsigned char m_gravity_multiplier;
	unsigned char m_vehicle_usage;

	// shark
	unsigned char pad[1];
});

// Traits that affect the player's appearance
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_appearance_traits,
{
	unsigned char m_active_camo_setting;
	unsigned char m_waypoint_setting;
	unsigned char m_aura_setting;
	unsigned char m_forced_change_color_setting;
});

// Traits that affect the HUD motion sensor behavior
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_player_trait_sensors,
{
	unsigned short m_motion_tracker_settings;
	unsigned short m_motion_tracker_range;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1C, c_player_traits,
{
	c_player_trait_shield_vitality m_shield_traits;
	c_player_trait_weapons m_weapon_traits;
	c_player_trait_movement m_movement_traits;
	c_player_appearance_traits m_appearance_traits;
	c_player_trait_sensors m_sensor_traits;
});

enum e_game_engine_miscellaneous_options_flags
{
	_game_engine_miscellaneous_options_teams_enabled_flag,

	k_game_engine_miscellaneous_options_flags
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_miscellaneous_options,
{
	c_flags<e_game_engine_miscellaneous_options_flags, unsigned char, k_game_engine_miscellaneous_options_flags> m_flags;

	// minutes
	unsigned char m_round_time_limit;

	unsigned char m_number_of_rounds;
	unsigned char m_early_victory_win_count;
});

enum e_game_engine_respawn_options_flags
{
	_game_engine_respawn_options_inherit_respawn_time_flag,
	_game_engine_respawn_options_respawn_with_teammate_flag,
	_game_engine_respawn_options_respawn_at_location_flag,
	_game_engine_respawn_options_respawn_on_kills_flag,

	k_game_engine_respawn_options_flags
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x28, c_game_engine_respawn_options,
{
	c_flags<e_game_engine_respawn_options_flags, unsigned char, k_game_engine_respawn_options_flags> m_flags;

	unsigned char m_lives_per_round;
	unsigned char m_team_lives_per_round;

	// seconds
	unsigned char m_respawn_time;

	// seconds
	unsigned char m_suicide_penalty;

	// seconds
	unsigned char m_betrayal_penalty;

	// seconds
	unsigned char m_unknown_penalty;

	// seconds
	unsigned char m_respawn_growth;

	// seconds
	unsigned char m_respawn_player_traits_duration;

	// woman bound for glory, why you leaving me again?
	unsigned char pad[3];

	c_player_traits m_respawn_player_traits;
});

enum e_game_engine_social_options_flags
{
	_game_engine_social_options_observers_enabled_flag,
	_game_engine_social_options_team_changing_enabled_flag,
	_game_engine_social_options_team_changing_balancing_only_flag,
	_game_engine_social_options_friendly_fire_enabled_flag,
	_game_engine_social_options_betrayal_booting_enabled_flag,
	_game_engine_social_options_enemy_voice_enabled_flag,
	_game_engine_social_options_open_channel_voice_enabled_flag,
	_game_engine_social_options_dead_player_voice_enabled_flag,

	k_game_engine_social_options_flags
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_social_options,
{
	c_flags<e_game_engine_social_options_flags, unsigned short, k_game_engine_social_options_flags> m_flags;

	unsigned short m_team_changing;
});

enum e_game_engine_map_override_options_flags
{
	_game_engine_map_override_options_grenades_on_map_flag,
	_game_engine_map_override_options_indestructible_vehicles_flag,

	k_game_engine_map_override_options_flags
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x7C, c_game_engine_map_override_options,
{
	c_flags<e_game_engine_map_override_options_flags, unsigned long, k_game_engine_map_override_options_flags> m_flags;

	c_player_traits m_player_traits;

	unsigned short m_weapon_set;
	unsigned short m_vehicle_set;

	c_player_traits m_red_powerup_traits;
	c_player_traits m_blue_powerup_traits;
	c_player_traits m_yellow_powerup_traits;

	// seconds
	unsigned char m_red_powerup_traits_duration;

	// seconds
	unsigned char m_blue_powerup_traits_duration;

	// seconds
	unsigned char m_yellow_powerup_traits_duration;

	// gonna pack her bags and leave this house of pain
	unsigned char pad[1];
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1D0, c_game_engine_base_variant,
{
	unsigned long __vftable;
	unsigned long __unknown4;

	char m_variant_backend_name[32];
	char m_metadata[0xF8];

	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;

	unsigned short m_flags;
	short m_team_scoring;
});

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_ctf_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

enum e_slayer_team_scoring_settings
{
	_slayer_team_scoring_settings_sum,
	_slayer_team_scoring_settings_minimum,
	_slayer_team_scoring_settings_maximum,

	k_slayer_team_scoring_settings_count
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_slayer_variant, c_game_engine_base_variant,
{
	// `c_game_engine_base_variant::m_team_scoring` override
	TODO(c_enum<e_slayer_team_scoring_settings, short, k_slayer_team_scoring_settings_count> m_team_scoring)

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
	char m_unknown_bonus_points;

	c_player_traits m_leader_player_traits;

	char slayer_padding[2];
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

	union
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
});

#pragma pack(pop)