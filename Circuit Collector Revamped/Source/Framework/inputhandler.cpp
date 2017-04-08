// 717310 C++ SDL Framework

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"
#include "player.h"

// Library includes:
#include <cassert>

InputHandler::InputHandler()
: m_pGameController(0)
, counter(0)
{
}

InputHandler::~InputHandler()
{
}

bool InputHandler::Initialise()
{
	return (true);
}

void InputHandler::ProcessInput(Game& game)
{
	// Receive Input Events below...
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		
		if (e.type == SDL_MOUSEBUTTONDOWN) // Mouse pressed
		{
			
		}
		
		switch (e.type)
		{
			/* Look for a keypress */
		case SDL_KEYDOWN:
			/* Check the SDLKey values and move change the coords */
			switch (e.key.keysym.sym)
			{
				case SDLK_ESCAPE: // Escape pressed
				{
					game.Quit();
				}
					break;
				case SDLK_1:
				{
					game.GetPlayer()->updateCurrency(10);
				}
					break;
				case SDLK_2:
				{
					game.GetPlayer()->updateCurrentHealth(10);
				}
					break;
				case SDLK_3:
				{
					game.GetPlayer()->updateMaximumHealth(10);
				}
					break;
				case SDLK_4:
				{
					game.GetPlayer()->updateHealthMultiplier(0.1f);
				}
					break;
				default:
					break;
			}
		}
	}
}
