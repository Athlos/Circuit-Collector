// 717310 C++ SDL Framework
#ifndef __BACKBUFFER_H__
#define __BACKBUFFER_H__

#include <string>
#include "SDL_ttf.h"
#include <SDL.h>   // All SDL App's need this
using namespace std;

// Forward Declarations:
class TextureManager;
class Sprite;
class AnimatedSprite;
struct SDL_Window;
struct SDL_Renderer;



class BackBuffer
{
	//Member Methods:
public:
	BackBuffer();
	~BackBuffer();

	bool Initialise(int width, int height);

	void Clear();
	void Present();
	void UpdateElectricity(int amount);
	void UpdateHealth(int amount);
	void UpdatePaused(bool paused);

	void SetClearColour(unsigned char r, unsigned char g, unsigned char b);

	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b);
	void SetDrawColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	void DrawSprite(Sprite& sprite);
	void DrawRectangle(int x1, int y1, int x2, int y2);
	void DrawLine(int x1, int y1, int x2, int y2);

	void DrawText();

	void DrawText(SDL_Texture* textOnScreen, SDL_Rect bounds);
	SDL_Texture* CreateText(std::string text, SDL_Color colour);

	Sprite* CreateSprite(const char* pcFilename);
	void DrawAnimatedSprite(AnimatedSprite& sprite, int x, int width);
	AnimatedSprite* CreateAnimatedSprite(const char* pcFilename);
	void SetGameState(bool state);
	void SetWave(int wave);
	void UpdateHealthUpgrade(int amount);
	void ShowControls(bool show);
	void ShowPrices(bool show);
	void SetMouse(int x, int y);
	
protected:
	void LogSDLError();

private:
	BackBuffer(const BackBuffer& backBuffer);
	BackBuffer& operator=(const BackBuffer& backBuffer);

	//Member Data:
public:

protected:
	TextureManager* m_pTextureManager;

	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Surface* m_surface;
	TTF_Font* font;
	SDL_Texture* m_electricityTexture;
	SDL_Texture* m_healthTexture;
	SDL_Texture* m_pausedTexture;
	SDL_Texture* m_gameOverTexture;
	SDL_Texture* m_waveTexture;
	

	//CONTROLS TEXTURES
	SDL_Texture* m_showControls;
	SDL_Texture* m_buyTowers;
	SDL_Texture* m_abilities;
	SDL_Texture* m_upgradeWall;
	SDL_Texture* m_startWave;
	SDL_Texture* m_pauseGame;
	SDL_Texture* m_shoot;
	SDL_Texture* m_repair;
	SDL_Texture* m_upgradeTower;
	SDL_Texture* m_slow;
	SDL_Texture* m_mouseUpgrade;
	SDL_Texture* m_quitGame;
	SDL_Texture* m_sellTower;

	//SHOW PRICES TEXTURES
	SDL_Texture* m_showPrices;
	SDL_Texture* m_towerPrice1;
	SDL_Texture* m_towerPrice2;
	SDL_Texture* m_minePrices;
	SDL_Texture* m_wallPrices;
	SDL_Texture* m_repairPrices;
	SDL_Texture* m_slowCost;
	SDL_Texture* m_mouseCost;

	bool m_paused;
	bool m_gameOver;
	int m_wave;
	int m_upgradeCost;
	bool m_controls;
	bool m_prices;

	int m_width;
	int m_height;

	unsigned char m_clearRed;
	unsigned char m_clearGreen;
	unsigned char m_clearBlue;

	TTF_Font* m_font;
	
private:

};

#endif // __BACKBUFFER_H__
