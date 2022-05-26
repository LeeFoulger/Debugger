#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_game_engine_miscellaneous_option_flags
{
	_game_engine_miscellaneous_option_teams_enabled = 0,

	k_game_engine_miscellaneous_option_flags
};

enum e_game_engine_respawn_options_flags
{
	_game_engine_respawn_options_inherit_respawn_time = 0,
	_game_engine_respawn_options_respawn_with_teammate,
	_game_engine_respawn_options_respawn_at_location,
	_game_engine_respawn_options_respawn_on_kills,

	k_game_engine_respawn_options_flags
};

enum e_game_engine_social_options_flags
{
	_game_engine_social_options_observers_enabled = 0,
	_game_engine_social_options_team_changing_enabled,
	_game_engine_social_options_team_changing_balancing_only,
	_game_engine_social_options_friendly_fire_enabled,
	_game_engine_social_options_betrayal_booting_enabled,
	_game_engine_social_options_enemy_voice_enabled,
	_game_engine_social_options_open_channel_voice_enabled,
	_game_engine_social_options_dead_player_voice_enabled,

	k_game_engine_social_options_flags
};

enum e_game_engine_map_override_options_flags
{
	_game_engine_map_override_options_grenades_on_map = 0,
	_game_engine_map_override_options_indestructible_vehicles,

	k_game_engine_map_override_options_flags
};

#pragma endregion

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_miscellaneous_options,
{
	c_flags<e_game_engine_miscellaneous_option_flags, unsigned char, k_game_engine_miscellaneous_option_flags> m_flags;

	// minutes
	unsigned char m_round_time_limit;

	unsigned char m_number_of_rounds;
	unsigned char m_early_victory_win_count;
});

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
	unsigned char m_unknown_penalty; // halo online specific

	// seconds
	unsigned char m_respawn_growth;

	// seconds
	unsigned char m_respawn_player_traits_duration;

	// woman bound for glory, why you leaving me again?
	unsigned char pad[3];

	c_player_traits m_respawn_player_traits;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_social_options,
{
	c_flags<e_game_engine_social_options_flags, unsigned short, k_game_engine_social_options_flags> m_flags;

	unsigned short m_team_changing;
});

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

struct c_game_engine_base_variant_vtbl;
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1D0, c_game_engine_base_variant,
{
	union
	{
		size_t vftable_address;
		c_game_engine_base_variant_vtbl* vftable;
	};

	unsigned long __unknown4;

	char m_variant_backend_name[32];
	char m_metadata[0xF8];

	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;

	unsigned short m_flags;
	short m_team_scoring_method;
});

struct c_game_engine_base_variant_vtbl
{
	long(*get_game_engine_name_string_id)();
	long(*get_game_engine_default_description_string_id)();
	void(*initialize)(c_game_engine_base_variant*);
	void(*validate)(c_game_engine_base_variant*);
	void(*encode)(c_game_engine_base_variant*, class c_bitstream*);
	void(*decode)(c_game_engine_base_variant*, class c_bitstream*);
	bool(*can_add_to_recent_list)();
	long(*get_score_to_win_round)(c_game_engine_base_variant*);
	long(*get_score_unknown)(c_game_engine_base_variant*); // halo online specific
	bool(*can_be_cast_to)(c_game_engine_base_variant*, e_game_engine_variant, void const**);
	void(*custom_team_score_stats)(long, long, long);
};

#pragma pack(pop)