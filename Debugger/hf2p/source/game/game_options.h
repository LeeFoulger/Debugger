#pragma once

#include <saved_games/game_variant.h>
#include <saved_games/map_variant.h>

#pragma pack(push, 1)

#pragma region enums

enum e_game_mode
{
	_game_mode_none = 0,
	_game_mode_campaign,
	_game_mode_multiplayer,
	_game_mode_mapeditor,
	_game_mode_savefilm,
	_game_mode_survival,

	k_game_mode_count,
	k_game_mode_default = _game_mode_multiplayer
};

enum e_game_simulation
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_sync_client,
	_game_simulation_sync_server,
	_game_simulation_dist_client,
	_game_simulation_dist_server,

	k_game_simulation_count,
	k_game_simulation_default = _game_simulation_local
};

enum e_game_playback
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_sync_client,

	k_game_playback_count,
	k_game_playback_default = _game_playback_local
};

enum e_language
{
	_language_none = -1,
	_language_english,
	_language_japanese,
	_language_german,
	_language_french,
	_language_spanish,
	_language_mexican,
	_language_italian,
	_language_korean,
	_language_chinese_traditional,
	_language_chinese_simplified,
	_language_portuguese,
	_language_russian,

	k_language_count,
	k_language_default = _language_english
};

enum e_campaign_difficulty_level
{
	_campaign_difficulty_level_easy = 0,
	_campaign_difficulty_level_normal,
	_campaign_difficulty_level_heroic,
	_campaign_difficulty_level_legendary,

	k_campaign_difficulty_level_count,
	k_campaign_difficulty_level_default = _campaign_difficulty_level_normal
};

#pragma endregion

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x80, s_hub_progression,
{
	uchar armaments[0x78];
	ulong hub_return_to_insertion_point;
	bool valid;
	t_padding<3> pad;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x128, game_machine_options,
{
	uchar __data[0x128];
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x30, s_player_configuration_from_client,
{
	wchar_t desired_name[16];
	char player_team;
	char player_assigned_team;
	char active_armor_loadout;
	char active_weapon_loadout;
	ulong hopper_access_flags;
	ulong cheating_flags;
	ulong user_experience_flags;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0xB40, s_player_configuration_from_host,
{
	uchar player_identifier[0x8];
	wchar_t player_name[16];
	long player_team;
	long player_assigned_team;
	uchar player_appearance[0x660];
	uchar saber_player_appearance[0x494];
	uchar saber_player_customization[0x1C];
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0xB70, s_player_configuration,
{
	s_player_configuration_from_client player_from_client;
	s_player_configuration_from_host player_from_host;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0xB90, game_player_options,
{
	bool valid;
	bool left_game;
	short user_index;
	long controller_index;
	uchar machine_identifier[0x10];
	qword identifier;
	s_player_configuration player_data;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1A048, game_options,
{
	c_enum<e_game_mode, long, k_game_mode_default, k_game_mode_count> game_mode;
	c_enum<e_game_simulation, char, k_game_simulation_default, k_game_simulation_count> game_simulation;
	char game_network_type;
	short game_tick_rate;
	qword game_instance;
	long random_seed;
	c_enum<e_language, long, k_language_default, _language_english> language;
	long determinism_version;
	long campaign_id;
	long map_id;
	c_string<char, MAX_PATH> scenario_path;
	short zoneset_group_index;
	bool load_level_only;
	bool dump_machine_index;
	bool dump_object_log;
	bool dump_random_seeds;
	bool playtest_mode;
	t_padding<1> pad0;
	c_enum<e_game_playback, short, k_game_playback_default, k_game_playback_count> game_playback;
	bool record_saved_film;
	t_padding<1> pad1;
	long playback_start_ticks;
	long playback_length_in_ticks;
	c_enum<e_campaign_difficulty_level, short, k_campaign_difficulty_level_default, k_campaign_difficulty_level_count> campaign_difficulty;
	short insertion_point;
	short metagame_scoring_option;
	bool campaign_metagame_enabled;
	bool campaign_survival_enabled;
	bool campaign_allow_persistent_storage;
	bool campaign_customization_enabled;
	uchar campaign_armaments[0x78];
	t_padding<2> pad2;
	uchar campaign_game_progression[0x80];
	ulong game_active_primary_skulls;
	ulong game_active_secondary_skulls;
	s_hub_progression hub_progression;
	t_padding<0x64> pad;
	c_game_variant game_variant;
	c_map_variant map_variant;
	game_machine_options machine_options;
	game_player_options player_options[16];
});

#pragma pack(pop)