#ifndef __GAME_H__
#define __GAME_H__

// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;
class Label;
class ParticleEmitter;
class Player;
class Tower;
class GameMap;
class EnemySpawner;
class ProjectileSpawner;

#include "fmod.hpp";

#include <vector>;

struct TowerFoundation
{
	float x;
	float y;

	bool isFree;
};

class Game
{
	//Member Methods:
public:
	static Game& GetInstance();
	static void DestroyInstance();
	~Game();

	bool Initialise();
	bool DoGameLoop();
	void Quit();

	void Pause(bool pause);
	bool IsPaused();

	Player* GetPlayer();
	void SpawnEnemy();

	void Debug_HurtClosest();
	void Debug_HurtMostHealth();
	void Debug_HurtLeastHealth();
	void Debug_SpawnTower();
	
protected:
	void Process(float deltaTime);

	// Draw methods
	void Draw(BackBuffer& backBuffer);
	void DrawTowers(BackBuffer& backBuffer);

private:
	Game(const Game& game);
	Game& operator=(const Game& game);
	
	Game();

	//Member Data:
public:

protected:
	static Game* sm_pInstance;
	BackBuffer* m_pBackBuffer;
	InputHandler* m_pInputHandler;
	bool m_looping;

	// Simulation Counters:
	float m_elapsedSeconds;
	float m_lag;
	float m_executionTime;
	int m_lastTime;
	int m_frameCount;
	int m_FPS;
	int m_numUpdates;
	bool m_drawDebugInfo;
	
	bool m_paused;

	//UI ELEMENTS
	const static int SCREEN_WIDTH = 1280;
	const static int SCREEN_HEIGHT = 720;

	Label* m_debug_fps;

	//GAME ENTITIES
	ParticleEmitter* m_particles; // Particle emitter
	Player* m_player; // Player

	std::vector<Tower*> m_towers; // Towers in the game
	EnemySpawner* m_enemySpawner;
	ProjectileSpawner* m_projectileSpawner;

	TowerFoundation m_availablePositions[8];

	//AUDIO
	FMOD::System *system;
	bool combatMusic;
	FMOD_RESULT result;
	FMOD::Channel* channel;
	FMOD::Channel* m_musicChannel;

private:

};

#endif // __GAME_H__
