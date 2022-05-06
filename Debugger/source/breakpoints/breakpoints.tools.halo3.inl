#pragma once

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
