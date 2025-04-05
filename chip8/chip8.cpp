#include <iostream>
#include "chip8.h"
#include "Util.h"

void _00E0(Chip8* chip8)
{
	for (int i = 0; i < CHIP8_GPU_BUFFER; ++i)
		chip8->_graphics[i] = 0;
	chip8->_PC += 2;
	if (chip8->_display)
	{
		chip8->_display->DrawInstruction(chip8->_graphics);
	}
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

void _3XNN(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD NN)
{
	if (chip8->_V[VX] == NN)
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
}

void _4XNN(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD NN)
{
	if (chip8->_V[VX] != NN)
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
}

void _5XY0(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD VY)
{
	if (chip8->_V[VX] == chip8->_V[VY])
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
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

void _8XY0(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD VY)
{
	chip8->_V[VX] = chip8->_V[VY];
	chip8->_PC += 2;
}

void _8XY1(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD VY)
{
	chip8->_V[VX] |= chip8->_V[VY];
	chip8->_PC += 2;
}

void _8XYE(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_V[0xF] = (chip8->_V[VX] >> 7) & 0x1;
	chip8->_V[VX] <<= 1;
	chip8->_PC += 2;
}

void _9XY0(Chip8* chip8, CHIP8_WORD VX, CHIP8_WORD VY)
{
	if (chip8->_V[VX] != chip8->_V[VY])
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
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

	chip8->_V[0xF] = 0;	// Reset collision flag

	for (int i = 0; i < N; ++i)
	{
		CHIP8_WORD pixel = chip8->_memory[chip8->_I + i];

		for (int j = 0; j < 8; ++j)
		{
			if ((pixel & (0x80 >> j)) != 0) // Check if the j-th bit of the sprite is set
			{
				int x = (xCord + j) % 64; // Wrap X-coordinate
				int y = (yCord + i) % 32; // Wrap Y-coordinate
				unsigned char& displayPixel = chip8->_graphics[y * 64 + x];

				if (displayPixel == 1)
				{
					chip8->_V[0xF] = 1; // Collision detected
				}
				displayPixel ^= 1; // Toggle pixel using XOR
			}
		}
	}

	chip8->_PC += 2;
	if (chip8->_display)
	{
		chip8->_display->DrawInstruction(chip8->_graphics);
	}
}

void _EX9E(Chip8* chip8, CHIP8_WORD VX)
{
	if (chip8->_keyboard[chip8->_V[VX]] == 1)
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
}

void _EXA1(Chip8* chip8, CHIP8_WORD VX)
{
	if (chip8->_keyboard[chip8->_V[VX]] == 0)
	{
		chip8->_PC += 4;
	}
	else
	{
		chip8->_PC += 2;
	}
}

void _FX0A(Chip8* chip8, CHIP8_WORD VX)
{
	bool keyPressed = false;
	for (int i = 0; i < 16; ++i)
	{
		if (chip8->_keyboard[i]){
			keyPressed = true;
			chip8->_V[VX] = i;
		}
	}

	if (keyPressed){
	 chip8->_PC += 2;
	}
}

void _FX07(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_V[VX] = chip8->_delaytimer;
	chip8->_PC += 2;
}

void _FX15(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_delaytimer = chip8->_V[VX];
	chip8->_PC += 2;
}

void _FX18(Chip8* chip8, CHIP8_WORD)
{
	std::cout << "fx18 not implemented" << std::endl;
	chip8->_PC += 2;
}

void _FX1E(Chip8* chip8, CHIP8_WORD VX)
{
	chip8->_I += chip8->_V[VX];

	chip8->_V[0xF] = (chip8->_I > 0x0FFF);
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

void _FX55(Chip8* chip8, CHIP8_WORD VX)
{
	for (uint8_t i = 0; i <= VX; ++i)
	{
		chip8->_memory[chip8->_I + i] = chip8->_V[i];
	}
	chip8->_PC += 2;
}

void _FX65(Chip8* chip8, CHIP8_WORD VX)
{
	for (CHIP8_WORD i = 0x0; i <= VX; ++i)
		chip8->_V[i] = chip8->_memory[chip8->_I + i];
	chip8->_PC += 2;
}

void _Timers(Chip8* chip8)
{
	if ( chip8->_delaytimer > 0)
	{
		chip8->_delaytimer -= 1;
	}
}

void EmuInit(Chip8* chip8)
{
	printf("--- CHIP-8 ---\n");

	chip8->_display = NULL;
	for (int i = 0; i < 16; ++i)
	{
		chip8->_keyboard[i] = false;
	}


	printf("Initializing main memory: %d bytes...", CHIP8_MEM_SIZE);
	chip8->_memory = (unsigned char*)calloc(CHIP8_MEM_SIZE, sizeof(unsigned char));
	for (int i = 0; i < 0x1000; ++i)
		chip8->_memory[i] = 0;
	printf("\tDONE\n");

	printf("Initializing graphics buffer: %d bytes...", CHIP8_GPU_BUFFER);
	chip8->_graphics = (unsigned char*)calloc(CHIP8_GPU_BUFFER, sizeof(unsigned char));
	for (int i = 0; i < CHIP8_GPU_BUFFER; ++i)
		chip8->_graphics[i] = 0;
	printf("\tDONE\n");

	printf("Initialzing registers...");
	memset(chip8->_V, 0, sizeof(chip8->_V));
	printf("\tDONE\n");

	printf("Initialzing program counter: 0x200...");
	chip8->_PC = 0x200;
	printf("\tDONE\n");

	printf("Initialzing index...");
	chip8->_I = 0;
	printf("\tDONE\n");

	printf("Initialzing stack pointer...");
	chip8->_sp = 0;
	printf("\tDONE\n");

	printf("Initialzing stack...");
	for (int i = 0; i < 0xF; ++i)
		chip8->_pcCallStack[i] = 0x00;
	printf("\tDONE\n");

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

		case 0x3000:
		{
			_3XNN(chip8, decoded[1] >> 8, decoded[3]);
			break;
		}

		case 0x4000:
		{
			_4XNN(chip8, decoded[1] >> 8, decoded[3]);
			break;
		}

		case 0x5000:
		{
			CHIP8_WORD lastByte = opcode & 0x000F;
			CHIP8_WORD VX = decoded[1] >> 8;
			CHIP8_WORD VY = (opcode & 0x00F0) >> 4;
			if (lastByte == 0x0000)
			{
				_5XY0(chip8, VX, VY);
			}
			else
			{
				printf("unknown opcode %X\n", opcode);
			}
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

		case 0x8000:
		{
			CHIP8_WORD lastByte = opcode & 0x000F;
			CHIP8_WORD VX = decoded[1] >> 8;
			CHIP8_WORD VY = (opcode & 0x00F0) >> 4;
			if (lastByte == 0x0000){
				_8XY0(chip8, VX, VY);
			}
			else if (lastByte == 0x0001){
				_8XY1(chip8, VX, VY);
			}
			else if (lastByte == 0x000E){
				_8XYE(chip8, VX);
			}
			else{
				printf("unknown opcode %X\n", opcode);
			}

			break;
		}

		case 0x9000:
		{
			CHIP8_WORD lastByte = opcode & 0x000F;
			CHIP8_WORD VX = decoded[1] >> 8;
			CHIP8_WORD VY = (opcode & 0x00F0) >> 4;

			if (lastByte == 0x0000)
			{
				_9XY0(chip8, VX, VY);
			}
			else
			{
				printf("unknown opcode %X\n", opcode);
			}
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

		case 0xE000:
		{
			if (decoded[3] == 0x009E){
				_EX9E(chip8, decoded[1] >> 8);
			}else if (decoded[3] == 0x00A1){
				_EXA1(chip8, decoded[1] >> 8);
			}
			else{
				printf("unknown opcode %X\n", opcode);
			}
			break;
		}

		case 0xF000:
		{
			if (decoded[3] == 0x000A)
			{
				_FX0A(chip8, decoded[1] >> 8);
			}
			else if (decoded[3] == 0x0007){
				_FX07(chip8, decoded[3] >> 8);
			}
			else if (decoded[3] == 0x0015){
				_FX15(chip8, decoded[1] >> 8);
			}
			else if (decoded[3] == 0x0018){
				_FX18(chip8, decoded[1] >> 8);
			}
			else if (decoded[3] == 0x001E){
				_FX1E(chip8, decoded[1] >> 8);
			}
			else if (decoded[3] == 0x0029){
				_FX29(chip8, decoded[1] >> 8);
			}else if (decoded[3] == 0x0033){
				_FX33(chip8, decoded[1] >> 8);
			}else if (decoded[3] == 0x0055){
				_FX55(chip8, decoded[1] >> 8);
			}else if (decoded[3] == 0x0065){
				_FX65(chip8, decoded[1] >> 8);
			}
			else
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

	_Timers(chip8); // TODO: timers need to be updated at a pace of 60hz. Currently this is being called at 700 hz
}

void EmuSetDisplay(Chip8* chip8, IDisplay* display)
{
	chip8->_display = display;
}

void EmuKeyPress(Chip8* chip8, uint8_t keyIndex)
{
	chip8->_keyboard[keyIndex] = true;
}

void EmuKeyRelease(Chip8* chip8, uint8_t keyIndex)
{
	chip8->_keyboard[keyIndex] = false;
}
