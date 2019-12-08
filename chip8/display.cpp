#include "display.h"

display::display()
{
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	texture = SDL_CreateTexture(rend, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, textureWidth, textureHeight);
}

display::~display()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void display::updateDisplay(void const* buffer)
{
	SDL_UpdateTexture(texture, nullptr, buffer, textureWidth * 4);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, texture, nullptr, nullptr);
	SDL_RenderPresent(rend);
}
