// 717310 C++ SDL Framework

// This includes:
#include "game.h"

// Local includes:
#include "backbuffer.h"
#include "inputhandler.h"
#include "logmanager.h"
#include "sprite.h"
#include "animatedsprite.h"
#include "fmod.hpp"
#include "tower.h"
#include "mine.h"


// Library includes:
#include <cassert>
#include <SDL.h>
#include <stdio.h>
#include <cmath>
#include <ctime>
#include <vector>
// Static Members:
Game* Game::sm_pInstance = 0;


Game&
Game::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Game();
	}

	assert(sm_pInstance);

	return (*sm_pInstance);
}

void
Game::DestroyInstance()
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
, m_electricity(0)
, m_inWave(0)
, m_waveNumber(1)
, m_showControls(true)
, m_showPrices(true)
{

}

Game::~Game()
{
	delete m_pBackBuffer;
	m_pBackBuffer = 0;

	for (int i = 0; i < m_mines.size(); i++)
	{
		delete m_mines[i];
		m_mines[i] = 0;
	}
	for (int i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
		m_enemies[i] = 0;
	}

	system->release();
}

bool Game::SellTower(int x, int y) 
{
	if (TowerClicked(x, y) == NULL) 
	{
		return false;
	}

	std::vector<Tower*>::iterator enemyIter = m_towers.begin();
	bool s = false;
	while (enemyIter != m_towers.end()) 
	{
		Tower* current = *enemyIter;
		if (current == TowerClicked(x, y))
		{
			if (current->GetType() == RAIL) 
			{
				m_electricity += 25 * (current->GetLevel()+1);
			}
			else
			{
				m_electricity += 50 * (current->GetLevel() + 1);
			}

			enemyIter = m_towers.erase(enemyIter);
			delete current;
			s = true;
		}
		else
		{
			if (s)
			{
				current->SetPosition(current->GetPositionX()-80, current->GetPositionY());
			}
			enemyIter++;
		}
	}
	return true;
}

void Game::UpgradeWall() 
{
	if (m_electricity < 200) 
	{
		return;
	}
	m_wall.SetMaxHealth(m_wall.GetMaxHealth() + 1000);
	m_wall.SetCurrentHealth(m_wall.GetCurrentHealth() + 1000);
	m_electricity -= 200;
}

void Game::RepairWall()
{
	float repairCost = ((static_cast<float>(m_wall.GetMaxHealth()) - static_cast<float>(m_wall.GetCurrentHealth())) / 10);

	if (m_electricity >= repairCost)
	{
		m_wall.SetCurrentHealth(m_wall.GetMaxHealth());
		m_electricity -= repairCost;
	}
	else
	{
		m_wall.SetCurrentHealth(m_wall.GetCurrentHealth() + (m_electricity * 10));
		m_electricity = 0;
	}
}


void Game::IncreaseWave()
{
	m_waveNumber++;
	m_pBackBuffer->SetWave(m_waveNumber);
}

void Game::GiveElectricity()
{
	m_electricity += 1000;
}

void Game::UpgradeReticle() 
{
	if (m_electricity < 100) 
	{
		return;
	}
	m_electricity -= 100;
	m_reticle.SetDamage(m_reticle.GetDamage() + 10);
}

bool
Game::Initialise()
{
	//SET UP GRAPHICS AND INPUT
	const int width = 800;
	const int height = 600;

	m_pBackBuffer = new BackBuffer();
	if (!m_pBackBuffer->Initialise(width, height))
	{
		LogManager::GetInstance().Log("BackBuffer Init Fail!");
		return (false);
	}

	m_pInputHandler = new InputHandler();
	if (!m_pInputHandler->Initialise())
	{
		LogManager::GetInstance().Log("InputHandler Init Fail!");
		return (false);
	}

	
	srand(time(0));

	//SET UP ENTITIES AND VARIABLES
	m_mineCounter = 10;

	Sprite* pPlayerSprite = m_pBackBuffer->CreateSprite("assets\\wall.png");

	m_wall.Initialise(pPlayerSprite);
	m_wall.SetPosition(0, 500);
	m_wall.SetCurrentHealth(5000);
	m_wall.SetMaxHealth(5000);

	m_electricity = 500;
	m_pBackBuffer->SetWave(m_waveNumber);
	m_pBackBuffer->UpdateHealthUpgrade(200);
	m_pBackBuffer->UpdateHealth(m_wall.GetCurrentHealth());
	Sprite* reticleSprite = m_pBackBuffer->CreateSprite("assets\\reticle.png");

	m_reticle.Initialise(reticleSprite);
	m_reticle.SetDamage(30);

	
	UpdateControls();
	UpdatePrices();

	AnimatedSprite* reticleAnim = m_pBackBuffer->CreateAnimatedSprite("assets\\damageIndicator.png");
	m_reticle.SetAnimation(reticleAnim);

	//SET UP AUDIO
	system = NULL;

	result = FMOD::System_Create(&system);      // Create the main system object.
	result = system->init(32, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.

	channel = 0;
	combatMusic = 0;
	m_musicChannel = 0;

	result = system->createSound("assets\\error.wav", FMOD_DEFAULT, 0, &m_error);
	result = system->createSound("assets\\towerplaced.wav", FMOD_DEFAULT, 0, &m_towerPlacedAuto);
	result = system->createSound("assets\\shoot.wav", FMOD_DEFAULT, 0, &m_shoot);
	result = system->createSound("assets\\towerplaced.wav", FMOD_DEFAULT, 0, &m_towerPlacedRail);
	result = system->createSound("assets\\Rhinoceros.wav", FMOD_DEFAULT, 0, &m_combatSoundtrack);
	result = system->createSound("assets\\minelayed.wav", FMOD_DEFAULT, 0, &m_mineLayed);
	result = system->createSound("assets\\click.wav", FMOD_DEFAULT, 0, &m_noMines);
	result = system->createSound("assets\\Explosion.wav", FMOD_DEFAULT, 0, &m_enemyExplodedSound);

	result = m_towerPlacedAuto->setMode(FMOD_LOOP_OFF);
	result = m_error->setMode(FMOD_LOOP_OFF);
	result = m_shoot->setMode(FMOD_LOOP_OFF);
	result = m_towerPlacedRail->setMode(FMOD_LOOP_OFF);
	result = m_mineLayed->setMode(FMOD_LOOP_OFF);
	result = m_noMines->setMode(FMOD_LOOP_OFF);

	m_lastTime = SDL_GetTicks();
	m_lag = 0.0f;

	return (true);
}

bool
Game::DoGameLoop()
{
	const float stepSize = 1.0f / 60.0f;

	assert(m_pInputHandler);
	m_pInputHandler->ProcessInput(*this);

	if (m_looping)
	{
		int current = SDL_GetTicks();
		float deltaTime = (current - m_lastTime) / 1000.0f;
		m_lastTime = current;

		m_executionTime += deltaTime;

		m_lag += deltaTime;

		while (m_lag >= stepSize)
		{
			Process(stepSize);

			m_lag -= stepSize;

			++m_numUpdates;
		}

		Draw(*m_pBackBuffer);
	}

	SDL_Delay(1);

	return (m_looping);
}

void
Game::Process(float deltaTime)
{
	// Count total simulation time elapsed:
	m_elapsedSeconds += deltaTime;

	// Frame Counter:
	if (m_elapsedSeconds > 1)
	{
		m_elapsedSeconds -= 1;
		m_FPS = m_frameCount;
		m_frameCount = 0;

		//Update counter labels
		char electricity[30] = "FPS : ";

		sprintf(electricity, "%d", m_FPS);

		char other_string[16] = "FPS : ";

		strcat(other_string, electricity);
		LogManager::GetInstance().Log(other_string);

		//DEAL DAMAGE TO WALL
		if (!m_paused) 
		{
			for each (Enemy* e in m_enemies)
			{
				if (e->GetPositionY() == m_wall.GetPositionY() - 64)
				{
					m_particles.SpawnNewParticles(e->GetPositionX()+32, m_wall.GetPositionY(), 3, m_pBackBuffer, BRICKPARTICLE);
					if (m_slowedCounter > 0) 
					{
						m_wall.TakeDamage(e->GetDamage()/10);
					}
					else 
					{
						m_wall.TakeDamage(e->GetDamage());
					}
				}
			}
		}
	}
	m_reticle.Process(deltaTime);
	m_pBackBuffer->UpdateElectricity(m_electricity);
	m_pBackBuffer->UpdateHealth(m_wall.GetCurrentHealth());

	if (m_wall.IsDead()) 
	{
		m_pBackBuffer->SetGameState(true);
		return;
	}

	if (m_paused) 
	{
		return;
	}
	m_pBackBuffer->UpdatePaused(m_paused);

	// Update the game world simulation:
	m_slowedCounter -= deltaTime;
	if (m_slowedCounter < 0) 
	{
		m_slowedCounter = 0;
	}

	if (m_slowedCounter > 0) 
	{
		deltaTime = deltaTime / 10;
	}

	for each (Enemy* e in m_enemies)
	{
		e->Process(deltaTime);
	}

	for each (Mine* b in m_mines)
	{
		b->Process(deltaTime);
	}

	if (m_towerBullets.size() >= 100)
	{
		LogManager::GetInstance().Log("lots of bullets");
	}

	std::vector<Bullet*>::iterator iter = m_towerBullets.begin();

	while (iter != m_towerBullets.end())
	{
		Bullet* current = *iter;
		if (current->IsDead() || static_cast<int>(current->GetPositionY()) <= 0)
		{
			iter = m_towerBullets.erase(iter);
			delete current;
		}
		else 
		{
			current->Process(deltaTime);
			if (static_cast<int>(current->GetPositionY()) <= 0)
			{
				LogManager::GetInstance().Log("bullet at zero");
			}
			m_particles.SpawnNewParticles(current->GetPositionX() + 4, current->GetPositionY() + 8, 1, m_pBackBuffer, current->GetParticleType());
			iter++;
		}
	}

	std::vector<Enemy*>::iterator enemyIter = m_enemies.begin();

	while (enemyIter != m_enemies.end()) 
	{
		Enemy* current = *enemyIter;
		if (current->IsDead()) 
		{
			m_electricity += current->GetReward()*m_waveNumber;
			enemyIter = m_enemies.erase(enemyIter);
			delete current;
		}
		else 
		{
			current->Process(deltaTime);
			enemyIter++;
		}
	}

	//CHECK MINE COLLISION
	for (int i = 0; i < m_mines.size(); i++) 
	{
		for (int a = 0; a < m_enemies.size(); a++) 
		{
			if (!m_enemies.at(a)->IsDead() && m_enemies.at(a)->IsCollidingWith(m_mines.at(i)->GetPositionX(), m_mines.at(i)->GetPositionY(), 32))
			{
				m_enemies.at(a)->TakeDamage(m_mines.at(i)->GetDamage());
				m_mines.at(i)->SetDead(true);
				SpawnExplosion(m_enemies.at(a)->GetPositionX(), m_enemies.at(a)->GetPositionY());
				result = system->playSound(m_enemyExplodedSound, 0, false, &channel);
			}
		}
	}

	for (int i = 0; i < m_towerBullets.size(); i++)
	{
		for (int a = 0; a < m_enemies.size(); a++) 
		{
			if (m_enemies.at(a)->IsCollidingWith(m_towerBullets.at(i)->GetPositionX(), m_towerBullets.at(i)->GetPositionY(), 16))
			{
				m_enemies.at(a)->TakeDamage(m_towerBullets.at(i)->GetDamage());
				m_towerBullets.at(i)->SetDead(true);
				result = system->playSound(m_enemyExplodedSound, 0, false, &channel);
				m_particles.SpawnNewParticles(m_enemies.at(a)->GetPositionX() + 32, m_enemies.at(a)->GetPositionY() + 16, 5, m_pBackBuffer, ALIENEXPLOSION);
			}
		}
	}

	for (int i = 0; i < m_mines.size(); i++) 
	{
		if (m_mines.at(i)->IsDead()) 
		{
			m_mines.erase(m_mines.begin() + i);
		}
	}

	for each (Tower* e in m_towers) 
	{
		e->Process(deltaTime);
	}

	for each (Explosion* b in m_explosions) 
	{
		b->Process(deltaTime);
	}

	for (int i = 0; i < m_explosions.size(); i++) 
	{
		if (m_explosions.at(i)->IsDead())
		{
			m_explosions.erase(m_explosions.begin() + i);
		}
	}
	if (!m_wall.IsDead())
	{
		m_wall.Process(deltaTime);
	}

	for each (Tower* t in m_towers)
	{
		if (t->ReadyToFire()) 
		{
			FireTower(t);
		}
	}

	m_particles.Process(deltaTime);

	system->update();
}

void Game::SlowMotion() 
{
	if (m_electricity < 150) 
	{
		return;
	}
	m_slowedCounter += 10.0f;
	m_electricity -= 150;
}

void
Game::Draw(BackBuffer& backBuffer)
{
	++m_frameCount;

	backBuffer.Clear();

	m_particles.Draw(backBuffer);

	for each (Enemy* e in m_enemies)
	{
		e->Draw(*m_pBackBuffer);

	}

	for each (Mine* b in m_mines)
	{
		b->Draw(*m_pBackBuffer);
	}

	for each (Tower* e in m_towers) 
	{
		e->Draw(backBuffer, true);
	}

	for each (Explosion* e in m_explosions) 
	{
		e->Draw(*m_pBackBuffer);
	}

	for each (Bullet* e in m_towerBullets) 
	{
		e->Draw(*m_pBackBuffer);
	}



	if (!m_wall.IsDead())
	{
		m_wall.Draw(*m_pBackBuffer);
	}
		
	backBuffer.DrawText("Electricity: ", 0, 0);

	m_reticle.Draw(backBuffer, true);

	backBuffer.Present();
}

Tower* Game::TowerClicked(int x, int y) 
{
	for each(Tower* t in m_towers)
	{
		if (t->IsClickedOn(m_reticle.GetPositionX()+16, m_reticle.GetPositionY()+16))
		{
			return t;
		}
	}
	return NULL;
}

void Game::UpgradeTower(Tower* t) 
{

	if (t->GetType() == BEAM) 
	{
		if (m_electricity < 100 || t->GetLevel() >= 10)
		{
			return;
		}
		t->SetDamage(t->GetDamage() + 50);
		t->SetFiringSpeed(t->GetFiringSpeed() - 0.03);
		m_electricity -= 100;
		t->SetLevel(t->GetLevel()+1);
	}

	if (t->GetType() == RAIL)
	{
		if (m_electricity < 50 || t->GetLevel() >= 5) {
			return;
		}
		t->SetDamage(t->GetDamage() + 25);
		t->SetFiringSpeed(t->GetFiringSpeed() - 0.1);
		m_electricity -= 50;
		t->SetLevel(t->GetLevel() + 1);
	}

	t->SetSpawning();
}

void
Game::Quit()
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


void Game::StartWave()
{
	SpawnEnemies(m_waveNumber * 7);
	m_waveNumber++;
}

void
Game::SpawnEnemies(int amount)
{
	if (m_enemies.size() > 0) 
	{
		return;
	}
	amount = amount * m_waveNumber;
	m_pBackBuffer->SetWave(m_waveNumber);
	while (amount > 0)
	{

		int randomizer = (rand() % 3) + 1;
		int x = (amount % 10) * 80;
		int y = -70;
		if (m_enemies.size() > 10)
		{
			y = -140;
		}

		if (randomizer == 1) 
		{
			Enemy* e = new Enemy();
			Sprite* enemySprite = m_pBackBuffer->CreateSprite("assets\\Enemy1.png");
			e->Initialise(enemySprite);
			e->SetPosition(x, y);
			e->SetData(200 * m_waveNumber, 20, 50 * m_waveNumber);
			e->SetReward(5);
			m_enemies.push_back(e);
		}
		else if (randomizer == 2) 
		{
			Enemy* e = new Enemy();
			Sprite* enemySprite = m_pBackBuffer->CreateSprite("assets\\Enemy2.png");
			e->Initialise(enemySprite);
			e->SetPosition(x, y);
			e->SetData(150 * m_waveNumber, 50, 30 * m_waveNumber);
			e->SetReward(2);
			m_enemies.push_back(e);
		}
		else 
		{
			Enemy* e = new Enemy();
			Sprite* enemySprite = m_pBackBuffer->CreateSprite("assets\\Enemy3.png");
			e->Initialise(enemySprite);
			e->SetPosition(x, y);
			e->SetData(100 * m_waveNumber, 100, 15 * m_waveNumber);
			e->SetReward(1);
			m_enemies.push_back(e);
		}
		amount--;
	}

	if (!combatMusic)
	{
		combatMusic = true;
		result = system->playSound(m_combatSoundtrack, 0, false, &m_musicChannel);
	}
	m_waveNumber++;
}

void Game::ShootEnemy(int x, int y)
{
	if (m_paused)
		return;
	result = system->playSound(m_shoot, 0, false, &channel);
	for each(Enemy* e in m_enemies) 
	{
		if (e->IsClickedOn(m_reticle.GetPositionX() + 16, m_reticle.GetPositionY() + 16)) 
		{
			e->TakeDamage(m_reticle.GetDamage());
			m_reticle.SetFiring();
			if (e->IsDead())
				SpawnExplosion(x + 16, y + 16);
		}
	}
}

void Game::UpdateControls()
{
	m_showControls = !m_showControls;
	m_pBackBuffer->ShowControls(m_showControls);
}

void Game::UpdatePrices()
{
	m_showPrices = !m_showPrices;
	m_pBackBuffer->ShowPrices(m_showPrices);
}

void Game::SpawnExplosion(int x, int y)
{
	Explosion* e = new Explosion();
	AnimatedSprite* explosionSprite = m_pBackBuffer->CreateAnimatedSprite("assets\\explosion.png");
	e->Initialise(explosionSprite);
	e->SetPosition(x - 24, y - 24);
	m_explosions.push_back(e);
}

void Game::FireTower(Tower* t) 
{
	if (m_enemies.size() == 0) {
		return;
	}

	Bullet* b = new Bullet();
	Sprite* bulletSprite;
	if (t->GetType() == BEAM) 
	{
		bulletSprite = m_pBackBuffer->CreateSprite("assets\\beambullet.png");
	}
	else 
	{
		bulletSprite = m_pBackBuffer->CreateSprite("assets\\playerbullet.png");
	}

	b->Initialise(bulletSprite);
	b->SetPosition(t->GetPositionX() + 24, t->GetPositionY() - 8);
	b->SetVerticalVelocity(-200);
	b->SetDamage(t->GetDamage());
	b->SetParticleType(PLAYERBULLETTRAIL);
	if (t->GetType() == BEAM)
	{
		b->SetVerticalVelocity(-700);
		b->SetParticleType(BEAMTRAIL);
	}
	m_towerBullets.push_back(b);
}

bool Game::SpawnTower(int x, int y, int type)
{
	Tower* one = new Tower();
	Sprite* oneSprite = m_pBackBuffer->CreateSprite("assets\\railturret.png");
	one->Initialise(oneSprite);

	Tower* two = new Tower();
	Sprite* twoSprite = m_pBackBuffer->CreateSprite("assets\\autoturret.png");
	two->Initialise(twoSprite);

	AnimatedSprite* oneAnim = m_pBackBuffer->CreateAnimatedSprite("assets\\railspawnanim.png");
	AnimatedSprite* twoAnim = m_pBackBuffer->CreateAnimatedSprite("assets\\beamspawnanim.png");
	
	switch (type)
	{
	case 1:
		if (m_electricity < 50)
		{
			result = system->playSound(m_error, 0, false, &channel);
			return false;
		}

		one->SetPosition(x, y);
		result = system->playSound(m_towerPlacedRail, 0, false, &channel);
		one->SetType(RAIL);
		one->SetFiringSpeed(1.0);
		one->SetDamage(75);
		one->SetAnimation(oneAnim);
		one->SetSpawning();
		m_towers.push_back(one);

		m_electricity -= 50;
		break;
	case 2:
		if (m_electricity < 100)
		{
			result = system->playSound(m_error, 0, false, &channel);
			return false;
		}

		two->SetPosition(x, y);
		result = system->playSound(m_towerPlacedAuto, 0, false, &channel);
		two->SetType(BEAM);
		one->SetDamage(125);
		two->SetFiringSpeed(0.5);
		two->SetAnimation(twoAnim);
		two->SetSpawning();
		m_towers.push_back(two);
		m_electricity -= 100;
		break;
	default:
		break;
	}
	return true;
}

void
Game::SpawnMine(int x, int y)
{
	if (m_electricity < 10)
	{
		result = system->playSound(m_noMines, 0, false, &channel);
		return;
	}
	m_electricity -= 10;
	Mine* e = new Mine();
	Sprite* enemySprite = m_pBackBuffer->CreateSprite("assets\\mine.png");
	e->Initialise(enemySprite);
	e->SetPosition(x, y);
	e->SetDamage(150);
	result = system->playSound(m_mineLayed, 0, false, &channel);
	m_mines.push_back(e);
	m_mineCounter--;
}
