#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>
#include <saved_games/saved_game_files.h>

#pragma pack(push, 1)

#pragma region enums

enum e_base_variant_flags
{
	_base_variant_flags_built_in = 0,

	k_base_variant_flags
};

enum e_game_engine_miscellaneous_option_flags
{
	_game_engine_miscellaneous_option_teams_enabled = 0,
	_game_engine_miscellaneous_option_round_reset_players,
	_game_engine_miscellaneous_option_round_reset_map,

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
	c_flags<e_game_engine_miscellaneous_option_flags, uchar, k_game_engine_miscellaneous_option_flags> m_flags;

	// default: 8
	// maximum: 60
	uchar m_round_time_limit; // minutes

	// default: 1
	// maximum: 15
	uchar m_number_of_rounds;

	// default: 2
	// maximum: 15
	uchar m_early_victory_win_count;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x28, c_game_engine_respawn_options,
{
	c_flags<e_game_engine_respawn_options_flags, uchar, k_game_engine_respawn_options_flags> m_flags;

	// default: 0
	// maximum: 50
	uchar m_lives_per_round;

	// default: 0
	// maximum: 100
	uchar m_team_lives_per_round;

	// default: 5
	// maximum: 240
	uchar m_respawn_time; // seconds

	// default: 10
	// maximum: 240
	uchar m_suicide_penalty; // seconds

	// default: 5
	// maximum: 240
	uchar m_betrayal_penalty; // seconds

	// default: 5
	// maximum: 240
	uchar m_unknown_penalty; // seconds, halo online specific

	// default: 0
	// maximum: 15
	uchar m_respawn_growth;

	// default: 5
	// maximum: 60
	uchar m_respawn_player_traits_duration;

	// woman bound for glory, why you leaving me again?
	t_padding<3> pad;

	c_player_traits m_respawn_player_traits;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x4, c_game_engine_social_options,
{
	c_flags<e_game_engine_social_options_flags, ushort, k_game_engine_social_options_flags> m_flags;

	// default: 0
	// maximum: 2
	ushort m_team_changing;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x7C, c_game_engine_map_override_options,
{
	c_flags<e_game_engine_map_override_options_flags, ulong, k_game_engine_map_override_options_flags> m_flags;

	c_player_traits m_player_traits;

	ushort m_weapon_set;
	ushort m_vehicle_set;

	c_player_traits m_red_powerup_traits;
	c_player_traits m_blue_powerup_traits;
	c_player_traits m_yellow_powerup_traits;

	// default: 5
	// maximum: 120
	uchar m_red_powerup_traits_duration; // seconds

	// default: 30
	// maximum: 120
	uchar m_blue_powerup_traits_duration; // seconds

	// default: 30
	// maximum: 120
	uchar m_yellow_powerup_traits_duration; // seconds

	// gonna pack her bags and leave this house of pain
	t_padding<1> pad;
});

struct c_game_engine_base_variant_vtbl;
DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1D0, c_game_engine_base_variant,
{
	union
	{
		size_t vftable_address;
		c_game_engine_base_variant_vtbl* vftable;
	};

	ulong __unknown4;

	char m_name[32];

	s_saved_game_item_metadata m_metadata;

	c_game_engine_miscellaneous_options m_miscellaneous_options;
	c_game_engine_respawn_options m_respawn_options;
	c_game_engine_social_options m_social_options;
	c_game_engine_map_override_options m_map_override_options;

	c_flags<e_base_variant_flags, ushort, k_base_variant_flags> m_flags;

	// default: 0
	// maximum: 2
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