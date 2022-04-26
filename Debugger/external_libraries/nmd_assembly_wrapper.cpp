#include <string_utils.h>

#define NMD_ASSEMBLY_IMPLEMENTATION
#include <nmd_assembly/nmd_assembly.h>

#ifdef _WIN64
const NMD_X86_MODE mode = NMD_X86_MODE_64;
#else
const NMD_X86_MODE mode = NMD_X86_MODE_32;
#endif

void disassemble_x86_impl(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...))
{
	if (!buffer || !buffer_end || !print_func)
		return;

	nmd_x86_instruction instruction;
	char formatted_instruction[128];

	for (size_t buffer_offset = 0; buffer_offset < size_t(buffer_end - buffer); buffer_offset += instruction.length)
	{
		if (!nmd_x86_decode(buffer + buffer_offset, buffer_end - (buffer + buffer_offset), &instruction, mode, NMD_X86_DECODER_FLAGS_MINIMAL))
			break;

		nmd_x86_format(&instruction, formatted_instruction, NMD_X86_INVALID_RUNTIME_ADDRESS, NMD_X86_FORMAT_FLAGS_DEFAULT);

		print_func("%s\n", formatted_instruction);
	}
}

bool instruction_startswith_impl(const unsigned char* buffer, const unsigned char* buffer_end, const char* search)
{
	if (!buffer || !buffer_end || !search)
		return false;

	nmd_x86_instruction instruction;
	char formatted_instruction[128];

	for (size_t buffer_offset = 0; buffer_offset < size_t(buffer_end - buffer); buffer_offset += instruction.length)
	{
		if (!nmd_x86_decode(buffer + buffer_offset, buffer_end - (buffer + buffer_offset), &instruction, mode, NMD_X86_DECODER_FLAGS_MINIMAL))
			break;

		nmd_x86_format(&instruction, formatted_instruction, NMD_X86_INVALID_RUNTIME_ADDRESS, NMD_X86_FORMAT_FLAGS_DEFAULT);

		return string_starts(formatted_instruction, search);
	}

	return false;
}