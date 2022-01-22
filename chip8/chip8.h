#pragma once

#include <stdio.h>
#include <string.h>

#define CHIP8_MEM_SIZE 4096
#define CHIP8_GPU_BUFFER 64 * 32
#define CHIP8_WORD unsigned short

unsigned char* _memory;
unsigned short _opcode;
unsigned char _V[16];   // 16 8bit registers named V0 to VF

// special registers
unsigned short _I;  // index
unsigned short _PC; // program counter

unsigned char* _graphics;

// timers
unsigned char _delaytimer;
unsigned char _soundtimer;

unsigned short _pcCallStack[16];
unsigned short _sp;

unsigned char _keyboard[16];


// not mine
unsigned char chip8_fontset[80] =
{
   0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
   0x20, 0x60, 0x20, 0x20, 0x70, // 1
   0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
   0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
   0x90, 0x90, 0xF0, 0x10, 0x10, // 4
   0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
   0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
   0xF0, 0x10, 0x20, 0x40, 0x40, // 7
   0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
   0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
   0xF0, 0x90, 0xF0, 0x90, 0x90, // A
   0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
   0xF0, 0x80, 0x80, 0x80, 0xF0, // C
   0xE0, 0x90, 0x90, 0x90, 0xE0, // D
   0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
   0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


/* how to seperate parameters
CHIP8_WORD expr = 0xDA12;
int x = (expr & 0x0F00) >> 8;
int y = (expr & 0x00F0) >> 4;
int z = expr & 0x000F;
*/

void _00E0()
{
	printf("TODO clear the screen\n");
	_PC += 2;
}

void _00EE()
{
	--_sp;
	_PC = _pcCallStack[_sp];
	_PC += 2;
}

void _1NNN(unsigned short NNN)
{
	_PC = NNN;
}

void _2NNN(CHIP8_WORD NNN)
{
	_pcCallStack[_sp] = _PC;
	++_sp;
	_PC = NNN;
}

void _6XNN(const short VX, const short NN)
{
	_V[VX] = NN;
	_PC += 2;
}

void _7XNN(const unsigned short VX, const unsigned short NN)
{
	_V[VX] += NN;
	_PC += 2;
}

void _ANNN(unsigned short NNN)
{
	_I = NNN;
	_PC += 2;
}

void _DXYN(CHIP8_WORD VX, CHIP8_WORD VY, CHIP8_WORD N)
{
	// thank you Tobias ;)
	const CHIP8_WORD xCord = _V[VX] & 63;
	const CHIP8_WORD yCord = _V[VY] & 31;

	printf("Draw sprite with height of %d, at %d,%d\n", N, xCord, yCord);

	_V[0xF] = 0;

	for (int i = 0; i < N; ++i)
	{
		CHIP8_WORD pixel = _memory[_I + i];
		for (int j = 0; j < 8; ++j)
		{
			if (pixel != 0)
			{

			}
			if (_graphics[i * 64 + j] == '1' )
			{
				_graphics[i * 64 + j] == '0';
				_V[0xF] = 1;
			}
		}
	}

	_PC += 2;
}

void EmuInit()
{
	printf("--- CHIP-8 ---\n");

	printf("Initializing main memory: %d bytes...", CHIP8_MEM_SIZE);
	_memory = (unsigned char*)calloc(CHIP8_MEM_SIZE, sizeof(unsigned char));
	for (int i = 0; i < 0x1000; ++i)
		_memory[i] = 0;
	printf("DONE\n");

	printf("Initializing graphics buffer: %d bytes...", CHIP8_GPU_BUFFER);
	_graphics = (unsigned char*)calloc(CHIP8_GPU_BUFFER, sizeof(unsigned char));
	for (int i = 0; i < CHIP8_GPU_BUFFER; ++i)
		_graphics[i] = 0;
	printf("DONE\n");

	printf("Initialzing registers...");
	memset(_V, 0, sizeof(_V));
	printf("DONE\n");

	printf("Initialzing program counter: 0x200...");
	_PC = 0x200;
	printf("DONE\n");

	printf("Initialzing index...");
	_I = 0;
	printf("DONE\n");

	printf("Initialzing stack pointer...");
	_sp = 0;
	printf("DONE\n");

	printf("Initialzing stack...");
	for (int i = 0; i < 0xF; ++i)
		_pcCallStack[i] = 0x00;
	printf("DONE\n");

}

void EmuLoadRom(const char* data, const int dataSize)
{
	memcpy(_memory + 0x200, data, dataSize);
}

void EmuDeInit()
{
	free(_memory);
	free(_graphics);
}

void EmuCycle()
{
	// Fetch Opcode
	_opcode = _memory[_PC];
	_opcode <<= 8;
	_opcode |= _memory[_PC + 1];
	// _PC += 2;

	// Decode Opcode
	unsigned short decoded[4];
	decoded[0] = _opcode & 0xF000;
	decoded[1] = _opcode & 0x0F00;
	decoded[2] = _opcode & 0x0F0F;
	decoded[3] = _opcode & 0x00FF;

	// Execute Opcode
	printf("opcode %X\n", _opcode);
	switch (decoded[0])
	{
		case 0x0:
		{
			if (decoded[3] == 0x00E0)
			{
				_00E0();
			}else if (decoded[3] == 0x00EE){
				_00EE();
			}else {
				printf("unknown opcode %X\n", _opcode);
			}
			break;
		}

		case 0x00EE:
		{
			_00EE();
			break;
		}

		case 0x1000:
		{
			_1NNN(_opcode & 0x0FFF);
			break;
		}

		case 0x2000:
		{
			_2NNN(_opcode & 0x0FFF);
			break;
		}

		case 0x6000:
		{
			_6XNN(decoded[1] >> 8, decoded[3]);
			break;
		}

		case 0x7000:
		{
			CHIP8_WORD VX = decoded[1] >> 8;
			CHIP8_WORD NN = decoded[3];
			_7XNN(VX, NN);
			break;
		}

		case 0xA000:
		{
			_ANNN(_opcode & 0x0FFF);
			break;
		}

		case 0xD000:
		{
			CHIP8_WORD x = decoded[1] >> 8;
			CHIP8_WORD y = (_opcode & 0x00F0) >> 4;
			CHIP8_WORD N = _opcode & 0x000F;
			_DXYN(x, y, N);
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

		default:
		{
			printf("unknown opcode %X\n", decoded[0]);
			break;
		}
	}

	// Update timers
}
