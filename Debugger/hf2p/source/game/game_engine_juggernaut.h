#pragma once

#include <cseries/cseries.h>
#include <game/game_engine_traits.h>

#pragma pack(push, 1)

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x200, c_game_engine_juggernaut_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x30];
});

#pragma pack(pop)