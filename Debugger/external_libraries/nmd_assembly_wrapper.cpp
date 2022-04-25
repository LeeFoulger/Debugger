
#define NMD_ASSEMBLY_IMPLEMENTATION
#include <nmd_assembly/nmd_assembly.h>

void dissassemble_x86_32(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...))
{
	if (!buffer || !buffer_end || !print_func)
		return;

	nmd_x86_instruction instruction;
	char formatted_instruction[128];

	for (size_t buffer_offset = 0; buffer_offset < size_t(buffer_end - buffer); buffer_offset += instruction.length)
	{
		if (!nmd_x86_decode(buffer + buffer_offset, buffer_end - (buffer + buffer_offset), &instruction, NMD_X86_MODE_32, NMD_X86_DECODER_FLAGS_MINIMAL))
			break;

		nmd_x86_format(&instruction, formatted_instruction, NMD_X86_INVALID_RUNTIME_ADDRESS, NMD_X86_FORMAT_FLAGS_DEFAULT);

		print_func("%s\n", formatted_instruction);
	}
}

void dissassemble_x86_64(const unsigned char* buffer, const unsigned char* buffer_end, int(print_func)(const char*, ...))
{
	if (!buffer || !buffer_end || !print_func)
		return;

	nmd_x86_instruction instruction;
	char formatted_instruction[128];

	for (size_t buffer_offset = 0; buffer_offset < size_t(buffer_end - buffer); buffer_offset += instruction.length)
	{
		if (!nmd_x86_decode(buffer + buffer_offset, buffer_end - (buffer + buffer_offset), &instruction, NMD_X86_MODE_64, NMD_X86_DECODER_FLAGS_MINIMAL))
			break;

		nmd_x86_format(&instruction, formatted_instruction, NMD_X86_INVALID_RUNTIME_ADDRESS, NMD_X86_FORMAT_FLAGS_DEFAULT);

		print_func("%s\n", formatted_instruction);
	}
}
