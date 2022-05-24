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
	_game_engine_miscellaneous_options_teams_enabled,

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
	_game_engine_respawn_options_inherit_respawn_time,
	_game_engine_respawn_options_respawn_with_teammate,
	_game_engine_respawn_options_respawn_at_location,
	_game_engine_respawn_options_respawn_on_kills,

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
	_game_engine_social_options_observers_enabled,
	_game_engine_social_options_team_changing_enabled,
	_game_engine_social_options_team_changing_balancing_only,
	_game_engine_social_options_friendly_fire_enabled,
	_game_engine_social_options_betrayal_booting_enabled,
	_game_engine_social_options_enemy_voice_enabled,
	_game_engine_social_options_open_channel_voice_enabled,
	_game_engine_social_options_dead_player_voice_enabled,

	k_game_engine_social_options_flags
};

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_social_options,
{
	c_flags<e_game_engine_social_options_flags, unsigned short, k_game_engine_social_options_flags> m_flags;

	unsigned short m_team_changing;
});

enum e_game_engine_map_override_options_flags
{
	_game_engine_map_override_options_grenades_on_map,
	_game_engine_map_override_options_indestructible_vehicles,

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

enum e_ctf_variant_flags
{
	_ctf_variant_flag_at_home_to_score_flag,

	k_ctf_variant_flags
};

enum e_ctf_variant_home_flag_waypoint_settings
{
	_ctf_variant_home_flag_waypoint_settings_never,
	_ctf_variant_home_flag_waypoint_settings_when_not_carried_by_enemy,
	_ctf_variant_home_flag_waypoint_settings_away_from_home,
	_ctf_variant_home_flag_waypoint_settings_always,

	k_ctf_variant_home_flag_waypoint_settings
};

enum e_ctf_variant_game_type_settings
{
	_ctf_variant_game_type_settings_multi_flag,
	_ctf_variant_game_type_settings_single_flag,
	_ctf_variant_game_type_settings_neutral_flag,

	k_ctf_variant_game_type_settings
};

enum e_ctf_variant_respawn_settings
{
	_ctf_variant_respawn_settings_normal,
	_ctf_variant_respawn_settings_on_friendly_capture,
	_ctf_variant_respawn_settings_on_enemy_capture,
	_ctf_variant_respawn_settings_on_any_capture,

	k_ctf_variant_respawn_settings
};

enum e_ctf_variant_touch_return_settings
{
	_ctf_variant_touch_return_settings_off = -1,
	_ctf_variant_touch_return_settings_instant = 0,
	_ctf_variant_touch_return_settings_5_seconds = 5,
	_ctf_variant_touch_return_settings_10_seconds = 10,
	_ctf_variant_touch_return_settings_15_seconds = 15,
	_ctf_variant_touch_return_settings_20_seconds = 20,
	_ctf_variant_touch_return_settings_30_seconds = 30,
	_ctf_variant_touch_return_settings_45_seconds = 45,
	_ctf_variant_touch_return_settings_1_minute = 60,

	k_ctf_variant_touch_return_settings
};

enum e_ctf_variant_sudden_death_time
{
	_ctf_variant_sudden_death_time_infinite = -1,
	_ctf_variant_sudden_death_time_off = 0,
	_ctf_variant_sudden_death_time_15_seconds = 15,
	_ctf_variant_sudden_death_time_1_minute = 60,
	_ctf_variant_sudden_death_time_2_minute = 120,
	_ctf_variant_sudden_death_time_5_minute = 300,

	k_ctf_variant_sudden_death_times
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_ctf_variant, c_game_engine_base_variant,
{
	c_flags<e_ctf_variant_flags, unsigned char, k_ctf_variant_flags> m_flags;

	c_enum<e_ctf_variant_home_flag_waypoint_settings, char, k_ctf_variant_home_flag_waypoint_settings> m_home_flag_waypoint;
	c_enum<e_ctf_variant_game_type_settings, char, k_ctf_variant_game_type_settings> m_game_type;
	c_enum<e_ctf_variant_respawn_settings, char, k_ctf_variant_respawn_settings> m_capture_respawn;
	c_enum<e_ctf_variant_touch_return_settings, short, k_ctf_variant_touch_return_settings> m_touch_return_time;
	c_enum<e_ctf_variant_sudden_death_time, short, k_ctf_variant_sudden_death_times> m_sudden_death_time;

	short m_score_to_win;
	short m_flag_reset_time;

	short unknown;

	c_player_traits carrier_traits;

	char pad[6];
});

enum e_slayer_team_scoring_settings
{
	_slayer_team_scoring_settings_sum,
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

enum e_oddball_variant_team_scoring_settings
{
	_oddball_variant_team_scoring_settings_sum,
	_oddball_variant_team_scoring_settings_minimum,
	_oddball_variant_team_scoring_settings_maximum,

	k_oddball_variant_team_scoring_settings
};

enum e_oddball_variant_flags
{
	_oddball_variant_flags_auto_ball_pickup,
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

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_king_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

enum e_sandbox_flags
{
	_sandbox_flags_open_channel_voice,

	k_sandbox_flags
};

enum e_sandbox_editing_mode
{
	_sandbox_flags_all_players,
	_sandbox_flags_only_leader,

	k_sandbox_editing_mode
};

enum e_sandbox_respawn_time
{
	_sandbox_respawn_time_instant = 0,
	_sandbox_respawn_time_3_seconds = 3,
	_sandbox_respawn_time_4_seconds = 4,
	_sandbox_respawn_time_5_seconds = 5,
	_sandbox_respawn_time_6_seconds = 6,
	_sandbox_respawn_time_7_seconds = 7,
	_sandbox_respawn_time_8_seconds = 8,
	_sandbox_respawn_time_9_seconds = 9,
	_sandbox_respawn_time_10_seconds = 10,
	_sandbox_respawn_time_15_seconds = 15,
	_sandbox_respawn_time_30_seconds = 30,
	_sandbox_respawn_time_60_seconds = 60,

	k_sandbox_respawn_times
};

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x1F0, c_game_engine_sandbox_variant, c_game_engine_base_variant,
{
	c_flags<e_sandbox_flags, unsigned char, k_sandbox_flags> m_flags;
	c_enum<e_sandbox_editing_mode, char, k_sandbox_editing_mode> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_all_player_traits;
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