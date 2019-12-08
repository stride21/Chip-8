#pragma once

#include <iostream>
#include <array>
#include <vector>
#include <numeric>
#include <algorithm>
#include <string>
#include <fstream>
#include <cstdint>
#include <iterator>
#include <random>
#include <stdio.h>
#include <SDL.h>
#include <chrono>
#include <conio.h>
#include <windows.h>
#include <filesystem>




class chip8
{
public:
	std::array<uint8_t, 16> registers = { 0 };
	std::array<uint8_t, 4096> memory = {
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
	0xF0, 0x80, 0xF0, 0x80, 0x80 };// F
	uint16_t program_counter;
	uint16_t index = 0;
	std::array<uint16_t, 16> stack = { 0 };
	uint8_t stack_pointer = 0;
	uint8_t delayTimer;
	uint8_t soundTimer;
	std::array<bool, 16> keyboard = { 0 };
	std::array<uint32_t, 64 * 32> video = { 0 };
	uint32_t* current_pixel = &video[0];
	int instructionsPerCycle = 3; // default instruction count
	const int timeOfCycle = 16650; // time of each complete cycle equals 16.65 milliseconds
	bool increaseSpeed = false; // increase instruction count
	bool decreaseSpeed = false; // decrease instruction count
	bool Quit = false;
	uint16_t opcode;
	void init();
	void LoadROM(const char* filename);
	void OP_6xnn(); void OP_00E0(); void OP_2nnn(); void OP_7xnn(); void OP_1nnn();
	void OP_3xnn(); void OP_4xnn(); void OP_5xy0(); void OP_00EE(); void OP_Dxyn();
	void OP_8xy0(); void OP_8xy1(); void OP_8xy2(); void OP_8xy3(); void OP_8xy4();
	void OP_8xy5(); void OP_8xy6(); void OP_8xy7(); void OP_8xyE(); void OP_9xy0();
	void OP_Annn(); void OP_Bnnn(); void OP_Cxnn(); void OP_Ex9E(); void OP_Fx65();
	void OP_ExA1(); void OP_Fx07(); void OP_Fx0A(); void OP_Fx15(); void OP_Fx18();
	void OP_Fx1E(); void OP_Fx29(); void OP_Fx33(); void OP_Fx55();
};
