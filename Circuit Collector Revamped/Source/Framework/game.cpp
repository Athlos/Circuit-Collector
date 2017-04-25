// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "fmod.hpp"
#include "label.h"
#include "particleemitter.h"
#include "player.h"
#include "tower.h"
#include "gamemap.h"
#include "enemyspawner.h"
#include "enemy.h"
#include "projectilespawner.h"
#include "projectile.h"

// Library includes:
#include <cassert>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <vector>

// Static Members:
Game* Game::sm_pInstance = 0;

Game& Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void Game::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

Game::Game()
: m_pBackBuffer(0)
, m_pInputHandler(0)
, m_looping(true)
, m_executionTime(0)
, m_elapsedSeconds(0)
, m_frameCount(0)
, m_FPS(0)
, m_numUpdates(0)
, m_lastTime(0)
, m_lag(0)
, m_paused(0)
{
	// Seed rand
	srand(time(0));
}

Game::~Game()
{
	std::vector<Tower*>::iterator towerIter = m_towers.begin();

	while (towerIter != m_towers.end())
	{
		if (*towerIter != 0)
		{
			delete *towerIter;
			towerIter = m_towers.erase(towerIter);
		}
		else
		{
			++towerIter;
		}
	}

	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	delete m_particles;
	m_particles = 0;

	delete m_pInputHandler;
	m_pInputHandler = 0;

	delete m_player;
	m_player = 0;

	delete m_enemySpawner;
	m_enemySpawner = 0;

	delete m_projectileSpawner;
	m_projectileSpawner = 0;

	delete m_debug_fps;
	m_debug_fps = 0;

	system->release();
}

/*
Initialises renderer, input, audio, player and other variables needed to begin
*/
bool Game::Initialise()
{
	//SET UP GRAPHICS AND INPUT

	// Load backbuffer
	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	// Load input
	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	//SET UP ENTITIES AND VARIABLES
	//Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\wall.png");

	m_particles = new ParticleEmitter();
	m_enemySpawner = new EnemySpawner();
	m_projectileSpawner = new ProjectileSpawner();

	SDL_Rect spawnArea;
	spawnArea.x = 0;
	spawnArea.w = SCREEN_WIDTH - 64;
	spawnArea.y = -128;
	spawnArea.h = 128;

	m_enemySpawner->SetSpawnArea(spawnArea);

	float baseWidth = (float)SCREEN_WIDTH / 8.0f;

	for (int i = 0; i < 8; ++i)
	{
		int x1 = (baseWidth * 0.1f) + (baseWidth * i);

		int y1 = SCREEN_HEIGHT - ((baseWidth * 1.1f) * 0.75);
		
		m_availablePositions[i].x = x1;
		m_availablePositions[i].y = y1;
	}

	//SET UP AUDIO
	system = NULL;

	result = FMOD::System_Create(&system); // Create the main system object.
	result = system->init(32, FMOD_INIT_NORMAL, 0); // Initialize FMOD.

	channel = 0;
	combatMusic = 0;
	m_musicChannel = 0;

	//result = system->createSound("assets\\error.wav", FMOD_DEFAULT, 0, &m_error);

	//result = m_towerPlacedAuto->setMode(FMOD_LOOP_OFF);

	//SET UP UI
	m_player = new Player();

	m_debug_fps = new Label("");
	m_debug_fps->SetBounds(0, 0, 48, 32);
	m_debug_fps->SetColour(0, 0, 0, 50);

	// Update timings
	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f; // calculate step size

	// Check input
	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (m_looping)
	{
		// Set up time values
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize); // Process

			m_lag -= stepSize;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer); // Render
	}

	SDL_Delay(1);

	return (m_looping);
}

void Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;

		std::string fpsString = std::to_string(m_FPS);
		m_debug_fps->SetText(fpsString);
	}

	// Check if game is paused
	if (m_paused)
	{
		return;
	}

	// Update the game world simulation:

	for each (Tower*  tower in m_towers)
	{
		tower->Process(deltaTime);

		if (tower->IsReadyToShoot())
		{
			m_projectileSpawner->SpawnProjectile(tower);
			tower->ResetCooldown();
		}
	}

	m_enemySpawner->Process(deltaTime);
	m_projectileSpawner->Process(deltaTime);

	m_particles->Process(deltaTime); // Process particles

	system->update(); // Update system
}

void Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	//Draw map
	m_enemySpawner->Draw(backBuffer);

	//Draw items in all vectors
	DrawTowers(backBuffer);

	//Draw wall
	backBuffer.SetDrawColour(0, 0, 0);
	backBuffer.DrawRectangle(0, SCREEN_HEIGHT - (SCREEN_WIDTH / 8.0f), SCREEN_WIDTH, (SCREEN_HEIGHT - (SCREEN_WIDTH / 8.0f) - 64));
	
	m_projectileSpawner->Draw(backBuffer);
	m_particles->Draw(backBuffer);

	//DRAW UI ELEMENTS
	m_player->drawPlayerUI(backBuffer);
	m_debug_fps->Draw(backBuffer);

	backBuffer.Present();
}

void Game::DrawTowers(BackBuffer& backBuffer)
{
	float baseWidth = (float)SCREEN_WIDTH / 8.0f;

	for (int i = 0; i < 8; ++i)
	{
		backBuffer.SetDrawColour(150, 150, 150);

		int x1 = (baseWidth * 0.1f) + (baseWidth * i);
		int x2 = x1 + (baseWidth * 0.8f);

		int y1 = SCREEN_HEIGHT - ((baseWidth * 1.1f) * 0.75);
		int y2 = y1 + (baseWidth * 0.75);

		backBuffer.DrawRectangle(x1, y1, x2, y2);
		backBuffer.SetDrawColour(0, 0, 0);
	}

	for each (Tower* tower in m_towers)
	{
		tower->Draw(backBuffer);
	}
}

void Game::Quit()
{
	m_looping = false;
}

void Game::Pause(bool pause)
{
	m_paused = pause;
	m_musicChannel->setPaused(pause);
}

bool Game::IsPaused()
{
	return (m_paused);
}

Player* Game::GetPlayer()
{
	return m_player;
}

void Game::SpawnEnemy()
{
	m_enemySpawner->SpawnEnemy(EnemyType::TANK, *m_pBackBuffer);
}

void Game::Debug_HurtClosest()
{
	if (m_enemySpawner->GetClosestEnemy() != 0)
	{
		m_enemySpawner->GetClosestEnemy()->TakeDamage(10);
	}
}

void Game::Debug_HurtMostHealth()
{
	if (m_enemySpawner->GetMostHealthEnemy() != 0)
	{
		m_enemySpawner->GetMostHealthEnemy()->TakeDamage(10);
	}
}

void Game::Debug_HurtLeastHealth()
{
	if (m_enemySpawner->GetLeastHealthEnemy() != 0)
	{
		m_enemySpawner->GetLeastHealthEnemy()->TakeDamage(10);
	}
}

void Game::Debug_SpawnTower()
{
	if (m_towers.size() < 8)
	{
		Tower* newTower = new Tower();

		newTower->Initialise(m_pBackBuffer->CreateSprite("assets\\railturret.png"));

		for (int i = 0; i < 8; ++i)
		{
			if (m_availablePositions[i].isFree)
			{
				m_availablePositions[i].isFree = false;

				newTower->SetPosition(m_availablePositions[i].x, m_availablePositions[i].y);
				break;
			}
		}

		Projectile* towerProjectile = new Projectile();

		towerProjectile->Initialise(m_pBackBuffer->CreateSprite("assets\\bullet.png"));

		newTower->InitialiseProjectile(towerProjectile);

		m_towers.push_back(newTower);
	}
}