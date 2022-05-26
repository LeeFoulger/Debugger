#pragma once

DECLARE_STRUCT_WITH_SIZE_ASSERT1(0xF8, s_saved_game_item_metadata,
{
	qword unique_id;
	wchar_t name[16];
	char description[128];
	char author[16];
	ulong file_type;
	bool author_is_xuid_online;
	t_padding<3> pad0;
	qword author_id;
	qword size_in_bytes;
	qword date;
	ulong length_seconds;
	ulong campaign_id;
	ulong map_id;
	ulong game_engine_type;
	ulong campaign_difficulty;
	ushort hopper_id;
	t_padding<2> pad;
	qword game_id;
});
