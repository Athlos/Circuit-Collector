// 717310 C++ SDL Framework

// This include:
#include "backbuffer.h"

// Local includes:
#include "logmanager.h"
#include "texturemanager.h"
#include "sprite.h"
#include "texture.h"
#include "animatedsprite.h"


#include <stdio.h>

#include <stdlib.h>

#include <string>

#include <vector>

#include <time.h>


// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <cassert>
#include <cstring>


using namespace std;

BackBuffer::BackBuffer()
: m_pTextureManager(0)
, m_pWindow(0)
, m_pRenderer(0)
, m_width(0)
, m_height(0)
, m_clearRed(0xFF)
, m_clearGreen(0xFF)
, m_clearBlue(0xFF)
, m_paused(false)
, m_gameOver(false)
, m_controls(false)
{

}

BackBuffer::~BackBuffer()
{
	SDL_DestroyRenderer(m_pRenderer);
	m_pRenderer = 0;

	SDL_DestroyWindow(m_pWindow);
	m_pWindow = 0;

	//SDL_FreeSurface(m_surface);
	TTF_CloseFont(font);

	IMG_Quit();
	SDL_Quit();
}

void BackBuffer::SetGameState(bool state)
{
	m_gameOver = state;
}

AnimatedSprite*
BackBuffer::CreateAnimatedSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	AnimatedSprite* pSprite = new AnimatedSprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void BackBuffer::UpdatePaused(bool paused)
{
	m_paused = paused;

}

void BackBuffer::UpdateHealthUpgrade(int amount)
{
	m_upgradeCost = amount;
}

void BackBuffer::UpdateHealth(int amount)
{
	std::string message = "Health: " + std::to_string(amount);

	SDL_Color clrFg = { 220, 20, 60, 0 }; 

	m_surface = SDL_GetWindowSurface(m_pWindow);
	m_surface = TTF_RenderText_Solid(font, message.c_str(), clrFg);
	m_healthTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	SDL_FreeSurface(m_surface);
}

void BackBuffer::UpdateElectricity(int amount)
{
	std::string message = "Electricity: " + std::to_string(amount);

	SDL_Color clrFg = { 30, 144, 255, 0 };  

	m_surface = SDL_GetWindowSurface(m_pWindow);
	m_surface = TTF_RenderText_Solid(font, message.c_str(), clrFg);
	m_electricityTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	SDL_FreeSurface(m_surface);
}

void BackBuffer::ShowControls(bool show)
{
	m_controls = show;
}

void BackBuffer::SetWave(int amount)
{
	char electricity[30] = "Electricity: ";

	sprintf(electricity, "%d", amount);

	char other_string[64] = "Wave: ";

	strcat(other_string, electricity);

	SDL_Color clrFg = { 30, 144, 255, 0 };
	m_surface = TTF_RenderText_Solid(font, other_string, clrFg);
	m_waveTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	SDL_FreeSurface(m_surface);
}

void BackBuffer::ShowPrices(bool show) 
{
	m_prices = show;
}

void BackBuffer::DrawText()
{

	SDL_Rect dest;

	dest.x = 350;
	dest.y = 0;
	dest.w = 128;
	dest.h = 32;
	SDL_RenderCopy(m_pRenderer, m_waveTexture, 0, &dest);
	
	//display controls, or prompt to display controls
	if (!m_controls) 
	{
		dest.x = 600;
		dest.y = 20;
		dest.w = 192;
		dest.h = 32;
		SDL_RenderCopy(m_pRenderer, m_showControls, 0, &dest);
	}
	else 
	{
		dest.x = 670;
		dest.y = 20;
		dest.w = 128;
		dest.h = 24;
		SDL_RenderCopy(m_pRenderer, m_buyTowers, 0, &dest);

		dest.y = 60;
		SDL_RenderCopy(m_pRenderer, m_abilities, 0, &dest);

		dest.y = 100;
		SDL_RenderCopy(m_pRenderer, m_upgradeWall, 0, &dest);

		dest.y = 140;
		SDL_RenderCopy(m_pRenderer, m_repair, 0, &dest);

		dest.y = 180;
		SDL_RenderCopy(m_pRenderer, m_mouseUpgrade, 0, &dest);
		
		dest.x = 650;
		dest.w = 148;
		dest.y = 220;
		SDL_RenderCopy(m_pRenderer, m_sellTower, 0, &dest);

		dest.x = 0;
		dest.y = 20;
		SDL_RenderCopy(m_pRenderer, m_startWave, 0, &dest);

		dest.y = 60;
		SDL_RenderCopy(m_pRenderer, m_pauseGame, 0, &dest);

		dest.y = 100;
		SDL_RenderCopy(m_pRenderer, m_shoot, 0, &dest);

		dest.y = 140;
		SDL_RenderCopy(m_pRenderer, m_upgradeTower, 0, &dest);

		dest.y = 180;
		SDL_RenderCopy(m_pRenderer, m_slow, 0, &dest);
	}


	//display prices, or prompt to display prices
	if (!m_prices) 
	{
		dest.x = 0;
		dest.y = 470;
		dest.w = 192;
		dest.h = 24;
		SDL_RenderCopy(m_pRenderer, m_showPrices, 0, &dest);
	}
	else 
	{
		dest.x = 0;
		dest.y = 470;
		dest.w = 256;
		dest.h = 24;
		SDL_RenderCopy(m_pRenderer, m_towerPrice1, 0, &dest);

		dest.y = 440;
		SDL_RenderCopy(m_pRenderer, m_towerPrice2, 0, &dest);

		dest.y = 410;
		SDL_RenderCopy(m_pRenderer, m_slowCost, 0, &dest);

		dest.w = 192;
		dest.x = 600;
		dest.y = 470;
		SDL_RenderCopy(m_pRenderer, m_wallPrices, 0, &dest);

		dest.y = 440;
		SDL_RenderCopy(m_pRenderer, m_minePrices, 0, &dest);

		dest.y = 410;
		SDL_RenderCopy(m_pRenderer, m_repairPrices, 0, &dest);

		dest.y = 380;
		SDL_RenderCopy(m_pRenderer, m_mouseCost, 0, &dest);		
	}

	dest.x = 350;
	dest.y = 250;
	dest.w = 100;
	dest.h = 50;
	if (m_paused) 
	{
		SDL_RenderCopy(m_pRenderer, m_pausedTexture, 0, &dest);
	}

	dest.x = 200;
	dest.y = 250;
	dest.w = 400;
	dest.h = 200;
	if (m_gameOver) 
	{
		SDL_RenderCopy(m_pRenderer, m_gameOverTexture, 0, &dest);
		dest.y = 50;
		SDL_RenderCopy(m_pRenderer, m_quitGame, 0, &dest);	
	}
}




void BackBuffer::DrawAnimatedSprite(AnimatedSprite& sprite, int x, int width) 
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = width;
	dest.h = sprite.GetHeight();

	SDL_Rect* f = new SDL_Rect;
	//location of animation frame
	f->x = x;
	f->y = 0;
	f->w = width;
	f->h = sprite.GetHeight();
	
	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), f, &dest);
}

bool BackBuffer::Initialise(int width, int height)
{
	m_width = width;
	m_height = height;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0)
	{
		LogSDLError();
		return (false);
	}
	else
	{
		m_pWindow = SDL_CreateWindow("717130 Game Framework", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);

		if (m_pWindow == 0)
		{
			LogSDLError();
			return (false);
		}
		else
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED); 

			if (m_pRenderer == 0)
			{
				LogSDLError();
				return (false);
			}
			else
			{
				int imageResult = IMG_Init(IMG_INIT_PNG);

				if (imageResult == 0)
				{
					LogSDLError();
					return (false);
				}
			}
		}
	}

	m_pTextureManager = new TextureManager();
	assert(m_pTextureManager);
	m_pTextureManager->Initialise(m_pRenderer);

	m_surface = SDL_GetWindowSurface(m_pWindow);

	TTF_Init();
	font = TTF_OpenFont("assets\\currentfont.ttf", 12);
	m_font = TTF_OpenFont("assets\\currentfont.ttf", 12);
	
	SDL_Color clrFg = { 0, 0, 255, 0 };  // Blue ("Fg" is foreground)

	//set electricity texture
	m_surface = TTF_RenderText_Solid(font, "electricity", clrFg);
	m_electricityTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	//set pause texture and health texture
	m_healthTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	m_surface = TTF_RenderText_Solid(font, "Paused", clrFg);
	m_pausedTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	//set wave texture
	m_surface = TTF_RenderText_Solid(font, "Wave", clrFg);
	m_waveTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	//set game over texture
	clrFg = { 178, 34, 34, 0 };
	font = TTF_OpenFont("assets\\gameOver.fon", 24);
	m_surface = TTF_RenderText_Solid(font, "Game Over", clrFg);
	m_gameOverTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	//set control textures
	m_surface = TTF_RenderText_Solid(font, "Press Q for controls", clrFg);
	m_showControls = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "1 & 2 to buy towers", clrFg);
	m_buyTowers = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "R to place mines", clrFg);
	m_abilities = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "E to upgrade wall", clrFg);
	m_upgradeWall = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "Space to Pause", clrFg);
	m_pauseGame = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "S to start wave", clrFg);
	m_startWave = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "Click to shoot", clrFg);
	m_shoot = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "T to repair wall", clrFg);
	m_repair = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "Click to upgrade tower", clrFg);
	m_upgradeTower = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "A for Slow Motion", clrFg);
	m_slow = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "D to upgrade mouse", clrFg);
	m_mouseUpgrade = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "F over Tower to sell", clrFg);
	m_sellTower = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	//set price textures
	m_surface = TTF_RenderText_Solid(font, "Press W to see prices", clrFg);
	m_showPrices = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "Tower 1 is 50, same to upgrade", clrFg);
	m_towerPrice1 = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "Tower 2 is 100, same to upgrade", clrFg);
	m_towerPrice2 = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "10 for each mine", clrFg);
	m_minePrices = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "200 to upgrade wall", clrFg);
	m_wallPrices = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "1 to repair 10 hp", clrFg);
	m_repairPrices = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);

	m_surface = TTF_RenderText_Solid(font, "150 for Slow Motion", clrFg);
	m_slowCost = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	
	m_surface = TTF_RenderText_Solid(font, "100 for 10 mouse damage", clrFg);
	m_mouseCost = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	SDL_FreeSurface(m_surface);
	//Quit label
	m_surface = TTF_RenderText_Solid(font, "Press esc to quit", clrFg);
	m_quitGame = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	
	//Cursor settings
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_WarpMouseInWindow(m_pWindow, 400, 300);

	return (true);
}

void BackBuffer::SetMouse(int x, int y) {
	SDL_WarpMouseInWindow(m_pWindow, x, y);
}

void BackBuffer::Clear()
{
	SDL_SetRenderDrawColor(m_pRenderer, m_clearRed, m_clearGreen, m_clearBlue, 0xff);
	SDL_RenderClear(m_pRenderer);
}

void BackBuffer::Present()
{
	SDL_RenderPresent(m_pRenderer);
}

void BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b)
{
	SetDrawColour(r, g, b, 0xFF);
}

void BackBuffer::SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
}

void BackBuffer::DrawSprite(Sprite& sprite)
{
	SDL_Rect dest;

	dest.x = sprite.GetX();
	dest.y = sprite.GetY();
	dest.w = sprite.GetWidth();
	dest.h = sprite.GetHeight();

	SDL_RenderCopy(m_pRenderer, sprite.GetTexture()->GetTexture(), 0, &dest);
}

void BackBuffer::DrawRectangle(int x1, int y1, int x2, int y2)
{
	SDL_Rect fillRect;
	
	fillRect.x = x1;
	fillRect.y = y1;
	fillRect.w = x2 - x1;
	fillRect.h = y2 - y1;

	SDL_RenderFillRect(m_pRenderer, &fillRect);
}

void BackBuffer::DrawLine(int x1, int y1, int x2, int y2)
{
	SDL_RenderDrawLine(m_pRenderer, x1, y1, x2, y2);
}

void BackBuffer::LogSDLError()
{
	LogManager::GetInstance().Log(SDL_GetError());
}

Sprite* BackBuffer::CreateSprite(const char* pcFilename)
{
	assert(m_pTextureManager);

	Texture* pTexture = m_pTextureManager->GetTexture(pcFilename);

	Sprite* pSprite = new Sprite();
	if (!pSprite->Initialise(*pTexture))
	{
		LogManager::GetInstance().Log("Sprite Failed to Create!");
	}

	return (pSprite);
}

void BackBuffer::SetClearColour(unsigned char r, unsigned char g, unsigned char b)
{
	m_clearRed = r;
	m_clearGreen = g;
	m_clearBlue = b;
}

void BackBuffer::DrawText(SDL_Texture* textOnScreen, SDL_Rect bounds)
{
	//Basic render of the texture of the label
	SDL_RenderCopy(m_pRenderer, textOnScreen, 0, &bounds);
}

SDL_Texture* BackBuffer::CreateText(std::string text, SDL_Color colour)
{
	//Grab a surface
	m_surface = SDL_GetWindowSurface(m_pWindow);

	//create text and save into surface, then use surface to create a texture we can render
	m_surface = TTF_RenderText_Solid(m_font, text.c_str(), colour);
	SDL_Texture* tTexture = SDL_CreateTextureFromSurface(m_pRenderer, m_surface);
	//Clean up surface, we grab a new one each time - need to look into why I do that
	SDL_FreeSurface(m_surface);

	return tTexture;

}