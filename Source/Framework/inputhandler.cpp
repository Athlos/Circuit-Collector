// 717310 C++ SDL Framework

// This include:
#include "inputhandler.h"

// Local includes:
#include "game.h"


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

bool 
InputHandler::Initialise()
{
	return (true);
}

void 
InputHandler::ProcessInput(Game& game)
{
	// Receive Input Events below...
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			game.Quit();
		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			game.ShootEnemy(e.button.x, e.button.y);
			if (game.TowerClicked(e.button.x, e.button.y) != NULL) 
			{
				game.UpgradeTower(game.TowerClicked(e.button.x, e.button.y));
			}
		}
		
		
		switch (e.type)
		{
			/* Look for a keypress */
		case SDL_KEYDOWN:
			/* Check the SDLKey values and move change the coords */
			switch (e.key.keysym.sym)
			{
			case SDLK_1:
				if (counter == 10)
					break;
				if(game.SpawnTower(80*counter, 530, 1))
					counter++;
				break;
			case SDLK_2:
				if (counter == 10)
					break;
				if (game.SpawnTower(80 * counter, 530, 2))
					counter++;
				break;
			case SDLK_SPACE:
				if (game.IsPaused()) 
				{
				game.Pause(false);
				}
				else
				{
				game.Pause(true);
				}
				break;
			case SDLK_r:
				game.SpawnMine();
				break;
			case SDLK_s:
				game.SpawnEnemies(10);
				break;
			case SDLK_e:
				game.UpgradeWall();
				break;
			case SDLK_q:
				game.UpdateControls();
				break;
			case SDLK_w:
				game.UpdatePrices();
				break;
			case SDLK_t:
				game.RepairWall();
				break;
			case SDLK_a:
				game.SlowMotion();
				break;
			case SDLK_d:
				game.UpgradeReticle();
				break;
			case SDLK_f:
				int tx, ty;
				SDL_GetMouseState(&tx, &ty);
				if (game.SellTower(tx, ty))
				{
					counter--;
				}
				break;
			case SDLK_F1:
				game.UpdateElectricity(1000);
				break;
			case SDLK_F2:
				game.IncreaseWave();
				break;
			case SDLK_ESCAPE:
				game.Quit();
				break;
			default:
				break;
			}
		}
	}
}
