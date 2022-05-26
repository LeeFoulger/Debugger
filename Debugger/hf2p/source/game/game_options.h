#pragma once

#include <saved_games/game_variant.h>

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
};

enum e_game_simulation
{
	_game_simulation_none = 0,
	_game_simulation_local,
	_game_simulation_sync_client,
	_game_simulation_sync_server,
	_game_simulation_dist_client,
	_game_simulation_dist_server,

	k_game_simulation_count
};

enum e_game_playback
{
	_game_playback_none = 0,
	_game_playback_local,
	_game_playback_sync_client,

	k_game_playback_count
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
	k_default_language = _language_english
};

#pragma endregion

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x30, s_player_configuration_from_client,
{
	wchar_t player_name[16];
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
	uchar __unknown1;
	short input_user_index;
	long controller_index;
	uchar machine_identifier[0x10];
	qword player_identifier;
	s_player_configuration player_configuration;
});

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0x1A048, game_options,
{
	c_enum<e_game_mode, long, _game_mode_multiplayer, k_game_mode_count> game_mode;
	c_enum<e_game_simulation, char, _game_simulation_local, k_game_simulation_count> game_simulation;
	char game_network_type;
	short game_tick_rate;
	qword game_instance;
	long random_seed;
	c_enum<e_language, long, k_default_language, _language_english> language;
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

	uchar __unnknown12F;

	c_enum<e_game_playback, short, _game_playback_local, k_game_playback_count> game_playback;
	bool record_saved_film;

	uchar __unnknown133;

	long playback_start_ticks;
	long playback_length_in_ticks;
	short campaign_difficulty;
	short insertion_point;
	short metagame_scoring_option;
	bool campaign_metagame_enabled;
	bool campaign_survival_mode;
	bool campaign_allow_persistent_storage;
	bool campaign_customization_enabled;
	uchar campaign_armaments[0x78];

	uchar __unnknown1BE[2];

	uchar campaign_game_progression[0x80];
	ulong game_active_primary_skulls;
	ulong game_active_secondary_skulls;
	uchar hub_armaments[120];

	uchar __unnknown2C0[0x10];
	uchar __unnknown2D0[0x5C];

	char game_variant[sizeof(c_game_variant)];
	char map_variant[0xE090];

	uchar machine_options[0x128];
	uchar player_options[16][sizeof(game_player_options)];

	c_game_variant& get_game_variant()
	{
		return *reinterpret_cast<c_game_variant*>(game_variant);
	}

	game_player_options& get_player_options(long player_index)
	{
		return *reinterpret_cast<game_player_options*>(player_options[player_index]);
	}
});

#pragma pack(pop)