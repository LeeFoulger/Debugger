#pragma once

// build: 8510025

struct s_rasterizer_global_shader_globals
{
	bool initialized;
	void* pixel_shaders[11];
	void* vertex_shaders[11];
};
static_assert(sizeof(s_rasterizer_global_shader_globals) == 0xB8, "sizeof(s_rasterizer_global_shader_globals) != 0xB8");

void on_setup_global_shader_breakpoint(c_debugger& debugger, c_registers& registers)
{
	// matchmaking menu crash hack fix
	static c_remote_reference<s_rasterizer_global_shader_globals> g_rasterizer_global_shader_globals(debugger, registers.get_runtime_addr(0x3C41800));

	if (g_rasterizer_global_shader_globals().pixel_shaders[2] != NULL && g_rasterizer_global_shader_globals().vertex_shaders[2] != NULL)
		return;

	for (size_t i = 2; i < 10; i++)
	{
		if (g_rasterizer_global_shader_globals().pixel_shaders[i] == NULL)
		{
			g_rasterizer_global_shader_globals().pixel_shaders[i] = g_rasterizer_global_shader_globals().pixel_shaders[0];
		}
		if (g_rasterizer_global_shader_globals().vertex_shaders[i] == NULL)
		{
			g_rasterizer_global_shader_globals().vertex_shaders[i] = g_rasterizer_global_shader_globals().vertex_shaders[0];
		}
	}

	g_rasterizer_global_shader_globals = g_rasterizer_global_shader_globals();
	printf("");
}

void explicit_shader_fix_patch(c_debugger& debugger, LPMODULEINFO module_info)
{
	//debugger.add_breakpoint(0x00000001408DABAF - PE64_BASE, false, "lea rbx,[rbx*8]", L"c_rasterizer_global_shaders::setup_global_shader", on_setup_global_shader_breakpoint);

	static c_remote_reference<c_bytes<0x1000>> page(debugger);
	static c_remote_reference<c_bytes<32>> found_addr(debugger);
	static c_remote_reference<unsigned long> patch0(debugger);
	static c_remote_reference<unsigned long> patch1(debugger);

	unsigned char pattern[] = { 0xC1, 0xE8, 0x06, 0xA8, 0x01, 0x74, 0x0A, 0xBE, 0xFE, 0xFF, 0xFF, 0xFF, 0xE9, 0x39, 0xFB, 0xFF, 0xFF, 0xC1, 0xE9, 0x07, 0xB8, 0xFD, 0xFF, 0xFF, 0xFF, 0xF6, 0xC1, 0x01 };
	char mask[] = "x??xx???xxxx?????x??xxxxxxxx";

	size_t found_offset = 0;
	for (size_t page_offset = 0; page_offset < module_info->SizeOfImage; page_offset += sizeof(page()))
	{
		page.set_address((size_t)module_info->lpBaseOfDll + page_offset);

		unsigned char* addr = find_pattern(page().value, pattern, mask);
		if (addr)
		{
			found_offset = addr - page().value;
			break;
		}
	}
	if (!found_offset)
		return;

	static bool valid_instructions = false;
	found_addr.set_address(page.get_address() + found_offset);
	disassemble_x86(found_addr().value, [](const char* format, ...) -> int
	{
		if (valid_instructions)
			return 0;

		va_list args;
		va_start(args, format);

		static char buffer[128];

		memset(buffer, 0, 128);
		int result = vsprintf_s(buffer, 128, format, args);

		const char* valid_instruction_strings[]
		{
			"shr",
			"test",
			// skip - jz short Ah
			"mov esi,FFFFFFFEh",
			// skip - jmp FFFFFB49h
			"shr",
			"mov eax,FFFFFFFDh",
			"test"
		};

		static int valid_instruction_count = 0;
		for (int i = 0; i < NUMBER_OF(valid_instruction_strings); i++)
		{
			if (valid_instruction_count == i && strstr(buffer, valid_instruction_strings[i]) == buffer)
			{
				valid_instruction_count++;
				break;
			}
		}

		if (valid_instruction_count == NUMBER_OF(valid_instruction_strings))
		{
			//MessageBox(NULL, L"Valid Instructions Found!", L"Success", MB_OK);

			if (valid_instruction_count == 4)
			{
				patch0.set_address(found_addr.get_address() + 0x1);
				patch1.set_address(found_addr.get_address() + 0xE);
			}
			if (valid_instruction_count == 6)
			{
				patch0.set_address(found_addr.get_address() + 0x8);
				patch1.set_address(found_addr.get_address() + 0x15);
			}

			valid_instructions = true;
		}

		va_end(format);
		return result;
	});

	// set explicit shader index to 'copy w/scale'
	if (patch0.get_address() && patch0() == 0xFFFFFFFE) patch0 = 0xFFFFFFFF;
	if (patch1.get_address() && patch1() == 0xFFFFFFFD) patch1 = 0xFFFFFFFF;
}