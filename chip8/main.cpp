#include "chip8.h"
#include "display.h"



int main(int argc, char* argv[])
{
	chip8 chip8;
	// basic cin for the name of the game. It checks if the file exists and if it does, it exits the while loop and closes cmd.
	// I plan on changing this in the future 
	std::cout << "Enter the filename of game you wish to play including its file extension. \n";
	std::string fileName;
	bool setName = false;
	while (setName == false)
	{
		std::cin >> fileName;
		if (std::filesystem::exists(fileName))
		{
			setName = true;
		}
		else
		{
			std::cout << "Either the filename is incorrect or it does not exist. Please try again.\n";
		}
	}
	CloseWindow(GetConsoleWindow());
	display display;
	int instructionCount = 1;
	unsigned __int64 cycleBegin = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	chip8.init();
	chip8.LoadROM(fileName.c_str());
	while (chip8.Quit != true)
	{
		// set the beginning time of new cycle after previous cycle has complete
		if (instructionCount == 1)
		{
			cycleBegin = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		}
		// current opcode
		chip8.opcode = (chip8.memory[chip8.program_counter] << 8) | chip8.memory[chip8.program_counter + 1];
		// increase program counter by 2 before calling a instruction
		chip8.program_counter += 2;
		// switch statements used to determine which opcode is being used and call the function for opcode 
		switch (chip8.opcode & 0xF00F)
		{
		case 0x8000:
			chip8.OP_8xy0();
			break;
		case 0x5000:
			chip8.OP_5xy0();
			break;
		case 0x8001:
			chip8.OP_8xy1();
			break;
		case 0x8002:
			chip8.OP_8xy2();
			break;
		case 0x8003:
			chip8.OP_8xy3();
			break;
		case 0x8004:
			chip8.OP_8xy4();
			break;
		case 0x8005:
			chip8.OP_8xy5();
			break;
		case 0x8006:
			chip8.OP_8xy6();
			break;
		case 0x8007:
			chip8.OP_8xy7();
			break;
		case 0x800E:
			chip8.OP_8xyE();
			break;
		case 0x9000:
			chip8.OP_9xy0();
			break;
		}

		switch (chip8.opcode & 0xF000)
		{
		case 0x1000:
			chip8.OP_1nnn();
			break;
		case 0x2000:
			chip8.OP_2nnn();
			break;
		case 0x3000:
			chip8.OP_3xnn();
			break;
		case 0x4000:
			chip8.OP_4xnn();
			break;
		case 0x6000:
			chip8.OP_6xnn();
			break;
		case 0x7000:
			chip8.OP_7xnn();
			break;
		case 0xA000:
			chip8.OP_Annn();
			break;
		case 0xB000:
			chip8.OP_Bnnn();
			break;
		case 0xC000:
			chip8.OP_Cxnn();
			break;
		case 0xD000:
			chip8.OP_Dxyn();
			break;
		}

		switch (chip8.opcode & 0xFFFF)
		{
		case 0x00E0:
			chip8.OP_00E0();
			break;
		case 0x00EE:
			chip8.OP_00EE();
			break;
		}

		switch (chip8.opcode & 0xF0FF)
		{
		case 0xE09E:
			chip8.OP_Ex9E();
			break;
		case 0xE0A1:
			chip8.OP_ExA1();
			break;
		case 0xF007:
			chip8.OP_Fx07();
			break;
		case 0xF00A:
			chip8.OP_Fx0A();
			break;
		case 0xF015:
			chip8.OP_Fx15();
			break;
		case 0xF018:
			chip8.OP_Fx18();
			break;
		case 0xF01E:
			chip8.OP_Fx1E();
			break;
		case 0xF029:
			chip8.OP_Fx29();
			break;
		case 0xF033:
			chip8.OP_Fx33();
			break;
		case 0xF055:
			chip8.OP_Fx55();
			break;
		case 0xF065:
			chip8.OP_Fx65();
			break;
		}
		unsigned __int64 cycleEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		unsigned __int64 instructionTime = (cycleEnd - cycleBegin);
		// if instruction is completed but the time it took was less then the amount needed for the instruction, wait.
		if (chip8.timeOfCycle / chip8.instructionsPerCycle > instructionTime)
		{
			while (chip8.timeOfCycle / chip8.instructionsPerCycle >= instructionTime)
			{
				cycleEnd = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				instructionTime = cycleEnd - cycleBegin;
			}
		}
		instructionCount++; // increment instruction count after completing instruction 
		//cycle has completed
		if (chip8.instructionsPerCycle == instructionCount)
		{
			SDL_Event event;
			SDL_PollEvent(&event);
			// if close is pressed, exit while loop and close program.
			if (event.type == SDL_QUIT)
			{
				break;
			}
			//store true if key is pressed
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
				{
					chip8.keyboard[5] = true;
					break;
				}
				case SDLK_1:
				{
					chip8.keyboard[1] = true;
					break;
				}
				case SDLK_2:
				{
					chip8.keyboard[2] = true;
					break;
				}
				case SDLK_3:
				{
					chip8.keyboard[3] = true;
					break;
				}
				case SDLK_4:
				{
					chip8.keyboard[12] = true;
					break;
				}
				case SDLK_q:
				{
					chip8.keyboard[4] = true;
					break;
				}
				case SDLK_e:
				{
					chip8.keyboard[6] = true;
					break;
				}
				case SDLK_r:
				{
					chip8.keyboard[13] = true;
					break;
				}
				case SDLK_a:
				{
					chip8.keyboard[7] = true;
					break;
				}
				case SDLK_s:
				{
					chip8.keyboard[8] = true;
					break;
				}
				case SDLK_d:
				{
					chip8.keyboard[9] = true;
					break;
				}
				case SDLK_f:
				{
					chip8.keyboard[14] = true;
					break;
				}
				case SDLK_z:
				{
					chip8.keyboard[10] = true;
					break;
				}
				case SDLK_x:
				{
					chip8.keyboard[0] = true;
					break;
				}
				case SDLK_c:
				{
					chip8.keyboard[11] = true;
					break;
				}
				case SDLK_v:
				{
					chip8.keyboard[15] = true;
					break;
				}
				case SDLK_ESCAPE:
				{
					chip8.Quit = true;
					break;
				}
				case SDLK_6:
				{
					if (chip8.instructionsPerCycle > 2)
					{
						--chip8.instructionsPerCycle; // decrease speed of emulation
					}
					break;
				}
				case SDLK_7:
				{

					if (chip8.instructionsPerCycle < 10)
					{
						++chip8.instructionsPerCycle; // increase speed of emulation
					}
					break;
				}
				}
			}
			//store false if key is no longer pressed
			if (event.type == SDL_KEYUP)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_w:
				{
					chip8.keyboard[5] = false;
					break;
				}
				case SDLK_1:
				{
					chip8.keyboard[1] = false;
					break;
				}
				case SDLK_2:
				{
					chip8.keyboard[2] = false;
					break;
				}
				case SDLK_3:
				{
					chip8.keyboard[3] = false;
					break;
				}
				case SDLK_4:
				{
					chip8.keyboard[12] = false;
					break;
				}
				case SDLK_q:
				{
					chip8.keyboard[4] = false;
					break;
				}
				case SDLK_e:
				{
					chip8.keyboard[6] = false;
					break;
				}
				case SDLK_r:
				{
					chip8.keyboard[13] = false;
					break;
				}
				case SDLK_a:
				{
					chip8.keyboard[7] = false;
					break;
				}
				case SDLK_s:
				{
					chip8.keyboard[8] = false;
					break;
				}
				case SDLK_d:
				{
					chip8.keyboard[9] = false;
					break;
				}
				case SDLK_f:
				{
					chip8.keyboard[14] = false;
					break;
				}
				case SDLK_z:
				{
					chip8.keyboard[10] = false;
					break;
				}
				case SDLK_x:
				{
					chip8.keyboard[0] = false;
					break;
				}
				case SDLK_c:
				{
					chip8.keyboard[11] = false;
					break;
				}
				case SDLK_v:
				{
					chip8.keyboard[15] = false;
					break;
				}
				}
			}
			//draw to display
			display.updateDisplay(&chip8.video);
			if (chip8.delayTimer > 0)
			{
				--chip8.delayTimer;
			}
			if (chip8.soundTimer > 0)
			{
				//std::cout << "\a"; currently disabled sound. \a was used for testing
				--chip8.soundTimer;
			}
			// reset instructionCount since a new cycle is beginning
			instructionCount = 1; 
			}
		
	}
	return 0;
};
