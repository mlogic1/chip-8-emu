#include <iostream>
#include "chip8.h"
#include "Util.h"
#include "EmuWindow.h"

#include "EmuWindow.h"
// bad pointer
EmuWindow* win;

void _00E0(Chip8* chip8)
{
	printf("TODO clear the screen\n");
	chip8->_PC += 2;
}

void _00EE(Chip8* chip8)
{
	--chip8->_sp;
	chip8->_PC = chip8->_pcCallStack[chip8->_sp];
	chip8->_PC += 2;
}

void _1NNN(Chip8* chip8, unsigned short NNN)
{
	chip8->_PC = NNN;
}

void _2NNN(Chip8* chip8, CHIP8_WORD NNN)
{
	chip8->_pcCallStack[chip8->_sp] = chip8->_PC;
	++chip8->_sp;
	chip8->_PC = NNN;
}

void _6XNN(Chip8* chip8, const short VX, const short NN)
{
	chip8->_V[VX] = NN;
	chip8->_PC += 2;
}

void _7XNN(Chip8* chip8, const unsigned short VX, const unsigned short NN)
{
	chip8->_V[VX] += NN;
	chip8->_PC += 2;
}

void _ANNN(Chip8* chip8, unsigned short NNN)
{
	chip8->_I = NNN;
	chip8->_PC += 2;
}

void _DXYN(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD VY, CHIP8_WORD N)
{
	// thank you Tobias ;)
	const CHIP8_WORD xCord = chip8->_V[VX] & 63;
	const CHIP8_WORD yCord = chip8->_V[VY] & 31;

	printf("Draw sprite with height of %d, at %d,%d\n", N, xCord, yCord);

	chip8->_V[0xF] = 0;

	for (int i = 0; i < N; ++i)
	{
		CHIP8_WORD pixel = chip8->_memory[chip8->_I + i];

		for (int j = 0; j < 8; ++j)
		{
			unsigned char& displayPixel = chip8->_graphics[i * 64 + j];
			if (pixel == '1' && displayPixel == '1')
			{
				displayPixel = '0';
				chip8->_V[0xF] = 1;
			}else if (pixel == '1' && displayPixel == '0'){
				displayPixel = '1';
			}
		}
	}

	chip8->_PC += 2;
	win->UpdateGraphics(chip8->_graphics); // bad
}

void _FX07(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_PC += 2;
}

void _FX15(Chip8* chip8, CHIP8_WORD VX)
{

	chip8->_PC += 2;
}

void _FX18(Chip8* chip8, CHIP8_WORD)
{
	chip8->_PC += 2;
}

void _FX29(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_I = chip8->_V[VX] * 5;
	chip8->_PC += 2;
}

void _FX33(Chip8* chip8, CHIP8_WORD VX)
{
	unsigned char val = chip8->_V[VX];
	chip8->_memory[chip8->_I] = val / 100;
	chip8->_memory[chip8->_I + 1] = (val / 10) % 10;
	chip8->_memory[chip8->_I + 2] = val % 10;

	chip8->_PC += 2;
}

void _FX65(Chip8* chip8, CHIP8_WORD VX)
{
	for (CHIP8_WORD i = 0x0; i <= VX; ++i)
		chip8->_V[i] = chip8->_memory[chip8->_I + i];
	chip8->_PC += 2;
}

void EmuInit(Chip8* chip8)
{
	printf("--- CHIP-8 ---\n");

	printf("Initializing main memory: %d bytes...", CHIP8_MEM_SIZE);
	chip8->_memory = (unsigned char*)calloc(CHIP8_MEM_SIZE, sizeof(unsigned char));
	for (int i = 0; i < 0x1000; ++i)
		chip8->_memory[i] = 0;
	printf("DONE\n");

	printf("Initializing graphics buffer: %d bytes...", CHIP8_GPU_BUFFER);
	chip8->_graphics = (unsigned char*)calloc(CHIP8_GPU_BUFFER, sizeof(unsigned char));
	for (int i = 0; i < CHIP8_GPU_BUFFER; ++i)
		chip8->_graphics[i] = 0;
	printf("DONE\n");

	printf("Initialzing registers...");
	memset(chip8->_V, 0, sizeof(chip8->_V));
	printf("DONE\n");

	printf("Initialzing program counter: 0x200...");
	chip8->_PC = 0x200;
	printf("DONE\n");

	printf("Initialzing index...");
	chip8->_I = 0;
	printf("DONE\n");

	printf("Initialzing stack pointer...");
	chip8->_sp = 0;
	printf("DONE\n");

	printf("Initialzing stack...");
	for (int i = 0; i < 0xF; ++i)
		chip8->_pcCallStack[i] = 0x00;
	printf("DONE\n");

}

void EmuLoadRom(Chip8* chip8, const char* data, const int dataSize)
{
	memcpy(chip8->_memory + 0x200, data, dataSize);
}

void EmuDeInit(Chip8* chip8)
{
	free(chip8->_memory);
	free(chip8->_graphics);
}

void EmuCycle(Chip8* chip8)
{
	// Fetch Opcode
	CHIP8_WORD opcode = chip8->_memory[chip8->_PC];
	opcode <<= 8;
	opcode |= chip8->_memory[chip8->_PC + 1];
	// _PC += 2;

	// Decode Opcode
	unsigned short decoded[4];
	decoded[0] = opcode & 0xF000;
	decoded[1] = opcode & 0x0F00;
	decoded[2] = opcode & 0x0F0F;
	decoded[3] = opcode & 0x00FF;

	// Execute Opcode
	printf("opcode %X\n", opcode);
	switch (decoded[0])
	{
		case 0x0:
		{
			if (decoded[3] == 0x00E0)
			{
				_00E0(chip8);
			}else if (decoded[3] == 0x00EE){
				_00EE(chip8);
			}else {
				printf("unknown opcode %X\n", opcode);
			}
			break;
		}

		case 0x00EE:
		{
			_00EE(chip8);
			break;
		}

		case 0x1000:
		{
			_1NNN(chip8, opcode & 0x0FFF);
			break;
		}

		case 0x2000:
		{
			_2NNN(chip8, opcode & 0x0FFF);
			break;
		}

		case 0x6000:
		{
			_6XNN(chip8, decoded[1] >> 8, decoded[3]);
			break;
		}

		case 0x7000:
		{
			CHIP8_WORD VX = decoded[1] >> 8;
			CHIP8_WORD NN = decoded[3];
			_7XNN(chip8, VX, NN);
			break;
		}

		case 0xA000:
		{
			_ANNN(chip8, opcode & 0x0FFF);
			break;
		}

		case 0xD000:
		{
			CHIP8_WORD x = decoded[1] >> 8;
			CHIP8_WORD y = (opcode & 0x00F0) >> 4;
			CHIP8_WORD N = opcode & 0x000F;
			_DXYN(chip8, x, y, N);
			break;
		}

		case 0xE09E:
		{
			printf("Bla");
			break;
		}

		case 0xE0A1:
		{
			printf("Bla");
			break;
		}

		case 0xF000:
		{
			if (decoded[3] == 0x0007){
				_FX07(chip8, decoded[3] >> 8);
			}
			else if (decoded[3] == 0x0015){
				_FX15(chip8, decoded[3] >> 8);
			}
			else if (decoded[3] == 0x0018){
				_FX18(chip8, decoded[3] >> 8);
			}
			else if (decoded[3] == 0x0029){
				_FX29(chip8, decoded[3] >> 8);
			}else if (decoded[3] == 0x0033){
				_FX33(chip8, decoded[1] >> 8);
			}else if (decoded[3] == 0x0065){
				_FX65(chip8, decoded[1] >> 8);
			}else
			{
				printf("unknown F opcode %X\n", opcode);
			}
			break;
		}

		default:
		{
			printf("unknown opcode %X\n", opcode);
			break;
		}
	}

	// Update timers
}
