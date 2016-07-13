// 717310 C++ SDL Framework
#ifndef __GAME_H__
#define __GAME_H__



// Forward Declarations
class BackBuffer;
class InputHandler;
class Sprite;

#include "enemy.h";
#include "mine.h";
#include "explosion.h";
#include "fmod.hpp";
#include "tower.h"
#include "wall.h"
#include "reticle.h"
#include "particleemitter.h"
#include "bullet.h"

#include <vector>;

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

	void SpawnEnemies(int amount);

	void SpawnMine(int x, int y);
	void SpawnExplosion(int x, int y);

	bool SpawnTower(int x, int y, int type);
	void Pause(bool pause);
	bool IsPaused();
	void ShootEnemy(int x, int y);
	void StartWave();
	void FireTower(Tower* t);
	void UpgradeWall();
	void UpdateControls();
	void UpdatePrices();
	void RepairWall();
	void UpgradeTower(Tower* t);
	Tower* TowerClicked(int x, int y);
	void SlowMotion();
	void UpgradeReticle();
	void IncreaseWave();
	void GiveElectricity();
	bool SellTower(int x, int y);
	
protected:
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

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


	
	int m_mineCounter;
	bool m_paused;
	bool m_inWave;
	int m_electricity;
	int m_waveNumber;
	bool m_showControls;
	bool m_showPrices;
	float m_slowedCounter;

	

	//GAME ENTITIES
	std::vector<Enemy*> m_enemies;
	std::vector<Mine*> m_mines;
	std::vector<Explosion*> m_explosions;
	std::vector<Tower*> m_towers;
	std::vector<Bullet*> m_towerBullets;
	Wall m_wall;
	Reticle m_reticle;
	ParticleEmitter m_particles;


	//AUDIO
	FMOD::System *system;
	FMOD::Sound* m_bulletFiredSound;
	FMOD::Sound* m_mineLayed;
	FMOD::Sound* m_noMines;
	FMOD::Sound* m_towerPlacedRail;
	FMOD::Sound* m_combatSoundtrack;
	FMOD::Sound* m_shoot;
	FMOD::Sound* m_error;
	FMOD::Sound* m_towerPlacedAuto;
	bool combatMusic;

	FMOD::Sound* m_enemyExplodedSound;
	FMOD_RESULT result;
	FMOD::Channel* channel;
	FMOD::Channel* m_musicChannel;

	

private:

};

#endif // __GAME_H__
