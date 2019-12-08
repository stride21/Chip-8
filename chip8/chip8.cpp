#include "chip8.h"

	void chip8::init() {
		program_counter = 0x200; // initialize program counter to the start of program in memory
		std::fill(video.begin(), video.end(), 0); //fill video array with 0s
	}

	void chip8::LoadROM(const char* filename)
	{
		const long pos = 0x200;
		std::ifstream input(filename, std::ios::binary);
		std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
		for (unsigned long i = 0; i < buffer.size(); i++)
		{
			memory[pos + i] = buffer[i];
		}
	}

	void chip8::OP_1nnn()
	{
		uint16_t address = opcode & 0x0FFF;
		program_counter = address;
	}

	void chip8::OP_2nnn()
	{
		++stack_pointer;
		uint16_t address = opcode & 0x0FFF;
		stack[stack_pointer] = program_counter;
		program_counter = address;
	}

	void chip8::OP_00EE()
	{
		program_counter = stack[stack_pointer];
		--stack_pointer;
	}

	void chip8::OP_00E0()
	{
		std::fill(video.begin(), video.end(), 0);
	}

	void chip8::OP_3xnn()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t address = opcode & 0x00FF;
		uint8_t temp = registers[regX];

		if (temp == address)
		{
			program_counter += 2;
		}
	}

	void chip8::OP_4xnn()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t address = opcode & 0x00FF;
		uint8_t temp = registers[regX];
		if (temp != address)
		{
			program_counter += 2;
		}
	}

	void chip8::OP_5xy0()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;
		if (registers[regX] == registers[regY])
		{
			program_counter += 2;
		}
	}

	void chip8::OP_6xnn()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t temp = opcode & 0x00FF;
		registers[regX] = temp;
	}

	void chip8::OP_7xnn()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t value = opcode & 0x00FF;
		registers[regX] += value;
	}

	void chip8::OP_8xy0()
	{
		uint8_t regY = (opcode & 0x00F0) >> 4;
		uint8_t regX = (opcode & 0x0F00) >> 8;
		registers[regX] = registers[regY];
	}

	void chip8::OP_8xy1()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;
		registers[regX] |= registers[regY];
	}

	void chip8::OP_8xy2()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;
		registers[regX] &= registers[regY];
		
	}

	void chip8::OP_8xy3()
	{
		uint8_t regX = (opcode & 0x0F00u) >> 8;
		uint8_t regY = (opcode & 0x00F0u) >> 4;
		registers[regX] ^= registers[regY];
	}

	void chip8::OP_8xy4()
	{
		
		uint8_t regX = (opcode & 0x0F00u) >> 8;
		uint8_t regY = (opcode & 0x00F0u) >> 4;
		uint16_t temp = registers[regX] + registers[regY];
		if (temp > 255)
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		registers[regX] = temp & 0xFF;
		

	}

	void chip8::OP_8xy5()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;

		if (registers[regX] > registers[regY])
		{
			registers[0xF] = 0x1;
		}
		else
		{
			registers[0xF] = 0x0;
		}
		uint8_t temp = registers[regX] - registers[regY];
		registers[regX] = temp;
	}

	void chip8::OP_8xy6()
	{    
		uint8_t regX = (opcode & 0x0F00) >> 8;
		registers[0xFu] = (registers[regX] & 0x01);
		registers[regX] /= 2; 
		
	}

	void chip8::OP_8xy7()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;

		if (registers[regY] > registers[regX])
		{
			registers[0xF] = 1;
		}
		else
		{
			registers[0xF] = 0;
		}

		uint8_t temp = registers[regY] - registers[regX];
		registers[regX] = temp;

	}

	void chip8::OP_8xyE()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t MSB = (registers[regX] & 0x80) >> 7;
		registers[0xF] = MSB;
		registers[regX] *= 2;
	}	  

	void chip8::OP_9xy0()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;
		if (registers[regX] != registers[regY])
		{
			program_counter += 2;
		}
	}

	void chip8::OP_Annn()
	{
		uint16_t address = opcode & 0x0FFF;
		index = address;
	}

	void chip8::OP_Bnnn()
	{
		uint16_t address = opcode & 0x0FFF;
		program_counter = address + registers[0];
	}

	void chip8::OP_Cxnn()
	{
		std::random_device rand;
		std::mt19937 rng(rand());
		std::uniform_int_distribution<std::mt19937::result_type> range(0, 255);
		unsigned short rand_num = range(rng);
		uint8_t value = opcode & 0x00FF;
		uint8_t regX = (opcode & 0x0F00) >> 8;
		registers[regX] = rand_num & value;
	}

	void chip8::OP_Fx07()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		registers[regX] = delayTimer;
	}

	void chip8::OP_Fx0A()
	{
		bool keypress = false;
		uint8_t regX = (opcode & 0x0F00) >> 8;
		SDL_Event event;
		while (keypress == false)
		{
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_w:
					{
						registers[regX] = 0x5;
						keyboard[5] = true;
						keypress = true;
						break;
					}
					case SDLK_1:
					{
						registers[regX] = 0x1;
						keyboard[1] = true;
						keypress = true;
						break;
					}
					case SDLK_2:
					{
						registers[regX] = 0x2;
						keyboard[2] = true;
						keypress = true;
						break;
					}
					case SDLK_3:
					{
						registers[regX] = 0x3;
						keyboard[3] = true;
						keypress = true;
						break;
					}
					case SDLK_4:
					{
						registers[regX] = 0xC;
						keyboard[12] = true;
						keypress = true;
						break;
					}
					case SDLK_q:
					{
						registers[regX] = 0x4;
						keyboard[4] = true;
						keypress = true;
						break;
					}
					case SDLK_e:
					{
						registers[regX] = 0x6;
						keyboard[6] = true;
						keypress = true;
						break;
					}
					case SDLK_r:
					{
						registers[regX] = 0xD; // 
						keyboard[13] = true;
						keypress = true;
						break;
					}
					case SDLK_a:
					{
						registers[regX] = 0x7;
						keyboard[7] = true;
						keypress = true;
						break;
					}
					case SDLK_s:
					{
						registers[regX] = 0x8;
						keyboard[8] = true;
						keypress = true;
						break;
					}
					case SDLK_d:
					{
						registers[regX] = 0x9;
						keyboard[9] = true;
						keypress = true;
						break;
					}
					case SDLK_f:
					{
						registers[regX] = 0xE;
						keyboard[14] = true;
						keypress = true;
						break;
					}
					case SDLK_z:
					{
						registers[regX] = 0xA;
						keyboard[10] = true;
						keypress = true;
						break;
					}
					case SDLK_x:
					{
						registers[regX] = 0x0;
						keyboard[0] = true;
						keypress = true;
						break;
					}
					case SDLK_c:
					{
						registers[regX] = 0xB;
						keyboard[11] = true;
						keypress = true;
						break;
					}
					case SDLK_v:
					{
						registers[regX] = 0xF;
						keyboard[15] = true;
						keypress = true;
						break;
					}
				}
			}
		}
	}
	}

	void chip8::OP_Ex9E()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
			if (keyboard[registers[regX]] == true)
				{	
					program_counter += 2;
				}	
    }

	void chip8::OP_ExA1()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		if (keyboard[registers[regX]] == false)
		{
			program_counter += 2;
		}
	}


	void chip8::OP_Fx18()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		soundTimer = registers[regX];
	}


	void chip8::OP_Fx1E()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		index += registers[regX];
	}

	void chip8::OP_Fx55()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		for (long i = 0; i <= regX; i++) {
			memory[index + i] = registers[i];
		}
	}

	void chip8::OP_Fx65()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		for (long i = 0; i <= regX; i++) 
		{
			registers[i] = memory[index + i];
		}
	}

	void chip8::OP_Fx15()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		delayTimer = registers[regX];
	}

	void chip8::OP_Fx29()
	{
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t temp = registers[regX];
		index = 0x00 + temp * 5; 
	}

	void chip8::OP_Fx33()
	{
		uint8_t regX = (opcode & 0x0F00u) >> 8;
		uint8_t ones = registers[regX] % 10;
		uint8_t temp = registers[regX] / 10;
		uint8_t tens = temp % 10;
		uint8_t hundreds =  temp / 10;
		memory[index + 2] = ones;
		memory[index + 1] = tens;
		memory[index + 0] = hundreds;
	}

	void chip8::OP_Dxyn()
	{
		registers[0xF] = 0;
		uint8_t regX = (opcode & 0x0F00) >> 8;
		uint8_t regY = (opcode & 0x00F0) >> 4;
		uint8_t texture_height = opcode & 0x000F;
		uint16_t row = 0;
		uint8_t pixel = 0;
		while (row != texture_height)
		{
			uint8_t column = 7;
			uint8_t sprite_byte = memory[index + row];
			// for loop shifts the bits of each sprite byte to get the pixel value and decrements the column value so its 
			for (int i = 0; i < 8; ++i)
			{
        // wrap back to the top in the same column if it y value goes off screen
				unsigned short int yCoordinate = (registers[regY] + row) % 32 ; 
        // wrap back to the left in the same row if it y value goes off screen
				unsigned short int xCoordinate = (registers[regX] + column) % 64 ;
				uint8_t temp = sprite_byte >> i; 
				pixel = temp & 0x01; // get pixel value
				 // multiply y coordinate by the texture width so that it draws on the correct row and add x coordinate so its on correct column
				current_pixel = &video[xCoordinate + 64 * yCoordinate];
				// register[F] = 1 if a pixel is unset
				if (*current_pixel != 0 && pixel == 1)
				{
					*current_pixel ^= 0x99FFFFFF; // cyan is the color of the pixels
					registers[0xF] = 0x01;
				}
				// sets pixel
				else if (*current_pixel == 0 && pixel == 1)
				{
					*current_pixel ^= 0x99FFFFFF;
				}

				column -= 1;
			}

			row += 1;
		}
	}
