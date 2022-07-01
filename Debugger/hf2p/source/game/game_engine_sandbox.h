#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

#pragma region enums

enum e_sandbox_variant_flags
{
	_sandbox_variant_flags_open_channel_voice = 0,

	k_sandbox_variant_flags
};

enum e_sandbox_editing_mode
{
	_sandbox_flags_all_players = 0,
	_sandbox_flags_only_leader,

	k_sandbox_editing_mode,
	k_sandbox_editing_mode_default = _sandbox_flags_all_players
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

	k_sandbox_respawn_times,
	k_sandbox_respawn_times_default = _sandbox_respawn_time_5_seconds
};

#pragma endregion

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x260, c_game_engine_sandbox_variant, c_game_engine_base_variant,
{
	c_flags<e_sandbox_variant_flags, uchar, k_sandbox_variant_flags> m_variant_flags;
	c_enum<e_sandbox_editing_mode, char, k_sandbox_editing_mode_default, k_sandbox_editing_mode> m_edit_mode;
	c_enum<e_sandbox_respawn_time, short, k_sandbox_respawn_times_default, k_sandbox_respawn_times> m_respawn_time;
	c_player_traits m_all_player_traits;

	t_padding<0x70> unused;
});

#pragma pack(pop)