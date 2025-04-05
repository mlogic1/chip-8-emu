#pragma once

#include <stdio.h>
#include <string.h>
#include "IDisplay.h"

#define CHIP8_MEM_SIZE 4096
#define CHIP8_GPU_BUFFER 64 * 32
#define CHIP8_WORD unsigned short

struct Chip8
{
	unsigned char* _memory;
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

	bool _keyboard[16];

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
	
	IDisplay* _display;
};


/* how to seperate parameters
CHIP8_WORD expr = 0xDA12;
int x = (expr & 0x0F00) >> 8;
int y = (expr & 0x00F0) >> 4;
int z = expr & 0x000F;
*/

void EmuInit(Chip8* chip8);
void EmuLoadRom(Chip8* chip8, const char* data, const int dataSize);
void EmuDeInit(Chip8* chip8);
void EmuCycle(Chip8* chip8);
void EmuSetDisplay(Chip8* chip8, IDisplay* display);
void EmuKeyPress(Chip8* chip8, uint8_t keyIndex);
void EmuKeyRelease(Chip8* chip8, uint8_t keyIndex);
