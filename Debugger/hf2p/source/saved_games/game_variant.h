#pragma once

#include <game/game_engine.h>

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
