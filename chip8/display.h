#pragma once
#include <stdio.h>
#include <SDL.h>



class display
{
private:
	SDL_Window* window{};
	SDL_Renderer* rend{};
	SDL_Texture* texture{};

public:
	const int windowWidth = 640;
	const int windowHeight = 320;
	const int textureWidth = 64;
	const int textureHeight = 32;
	void updateDisplay(void const* buffer);
	display();
	~display();
};
