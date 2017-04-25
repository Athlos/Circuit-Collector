#include "enemyspawner.h"
#include "enemy.h"
#include "backbuffer.h"
#include "logmanager.h"

EnemySpawner::EnemySpawner()
{
	m_closest = 0;
	m_mostHealth = 0;
	m_leastHealth = 0;
}

EnemySpawner::~EnemySpawner()
{
	std::vector<Enemy*>::iterator enemyIter = m_spawnedEnemies.begin();

	while (enemyIter != m_spawnedEnemies.end())
	{
		if (*enemyIter != 0)
		{
			delete *enemyIter;
			enemyIter = m_spawnedEnemies.erase(enemyIter);
		}
		else
		{
			++enemyIter;
		}
	}
}

void EnemySpawner::Process(float deltaTime)
{
	std::vector<Enemy*>::iterator enemyIter = m_spawnedEnemies.begin();

	while (enemyIter != m_spawnedEnemies.end())
	{
		Enemy* current = *enemyIter;
		if (current->IsDead())
		{
			if (current == m_closest)
			{
				m_closest = 0;
			}

			if (current == m_mostHealth)
			{
				m_mostHealth = 0;
			}

			if (current == m_leastHealth)
			{
				m_leastHealth = 0;
			}

			enemyIter = m_spawnedEnemies.erase(enemyIter);
			delete current;
		}
		else
		{
			current->Process(deltaTime);
			UpdateTargetEnemies(current);
			enemyIter++;
		}
	}
}

void EnemySpawner::Draw(BackBuffer& backBuffer)
{
	for each (Enemy* enemy in m_spawnedEnemies)
	{
		enemy->Draw(backBuffer);
	}
}

void EnemySpawner::SpawnEnemy(EnemyType type, BackBuffer& backBuffer)
{
	Enemy* newEnemy = new Enemy();
	Sprite* enemySprite;

	switch (type)
	{
	case EnemyType::TANK:
		enemySprite = backBuffer.CreateSprite("assets\\EnemyTank.png");
		newEnemy->Initialise(enemySprite);
		break;
	case EnemyType::MAX_TYPE:
		break;
	default:
		enemySprite = backBuffer.CreateSprite("assets\\Reticle.png");
		newEnemy->Initialise(enemySprite);
		break;
	}

	newEnemy->SetData(20, 25, 10);
	newEnemy->SetReward(5);

	Position pos = GetSpawnPosition();

	newEnemy->SetPosition(pos.x, pos.y);

	if (m_spawnedEnemies.size() == 0)
	{
		m_closest = newEnemy;
		m_mostHealth = newEnemy;
		m_leastHealth = newEnemy;
	}

	m_spawnedEnemies.push_back(newEnemy);
}

void EnemySpawner::SetSpawnArea(SDL_Rect area)
{
	m_spawnArea = area;
}

void EnemySpawner::KillEnemy()
{
	if (m_spawnedEnemies.size() > 0)
	{
		m_spawnedEnemies.at(rand() % m_spawnedEnemies.size())->SetDead(true);
	}
}

Position EnemySpawner::GetSpawnPosition()
{
	Position result;

	result.x = (rand() % ((m_spawnArea.x + m_spawnArea.w) - m_spawnArea.x) + m_spawnArea.x);
	result.y = (rand() % ((m_spawnArea.y + m_spawnArea.h) - m_spawnArea.y) + m_spawnArea.y) - 64; // TODO change this to be dynamic

	//std::string message = ("Enemy Spawn at" + std::to_string(result.x) + ", " + std::to_string(result.y));

	//LogManager::GetInstance().Log(message.c_str());

	LogManager::GetInstance().Log("HELLO");

	return result;
}

void EnemySpawner::UpdateTargetEnemies(Enemy* enemy)
{
	if (m_closest == 0 || m_closest->GetPositionY() < enemy->GetPositionY() || m_closest->IsDead())
	{
		m_closest = enemy;
	}

	if (m_mostHealth == 0 || m_mostHealth->GetHealth() < enemy->GetHealth() || m_mostHealth->IsDead())
	{
		m_mostHealth = enemy;
	}

	if (m_leastHealth == 0 || m_leastHealth->GetHealth() > enemy->GetHealth() || m_leastHealth->IsDead())
	{
		m_leastHealth = enemy;
	}
}

Enemy* EnemySpawner::GetClosestEnemy()
{
	return m_closest;
}

Enemy* EnemySpawner::GetMostHealthEnemy()
{
	return m_mostHealth;
}

Enemy* EnemySpawner::GetLeastHealthEnemy()
{
	return m_leastHealth;
}

std::vector<Enemy*> EnemySpawner::GetEnemies()
{
	return m_spawnedEnemies;
}