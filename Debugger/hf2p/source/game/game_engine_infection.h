#pragma once

#include <cseries/cseries.h>

#pragma pack(push, 1)

DECLARE_INHERITED_STRUCT_WITH_SIZE_ASSERT1(0x250, c_game_engine_infection_variant, c_game_engine_base_variant,
{
	TODO("map out this data")
	char __data[0x80];
});

#pragma pack(pop)