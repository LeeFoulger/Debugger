#pragma once

#include <saved_games/game_variant.h>

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

struct c_game_variant;
struct game_options
{
	c_enum<e_game_mode, long, k_game_mode_count> game_mode;

	char __data[0xE620 - 4];

	void scenario_path(c_string<char, MAX_PATH> const scenario_path)
	{
		csstrncpy(&__data[0x24 - 4], MAX_PATH, scenario_path.value, MAX_PATH);
	}

	c_game_variant& game_variant()
	{
		return *reinterpret_cast<c_game_variant*>(&__data[0x32C - 4]);
	}
};
