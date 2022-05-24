#pragma once

enum e_game_mode : unsigned long
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
	char __data[0xE620];

	void game_mode(e_game_mode const game_mode)
	{
		*reinterpret_cast<e_game_mode*>(__data) = game_mode;
	}

	void scenario_path(c_string<char, MAX_PATH> const scenario_path)
	{
		csstrncpy(__data + 0x24, MAX_PATH, scenario_path.value, MAX_PATH);
	}

	c_game_variant& game_variant()
	{
		return *reinterpret_cast<c_game_variant*>(__data + 0x32C);
	}
};
