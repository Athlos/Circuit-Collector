#ifndef __ENEMYSPAWNER_H__
#define __ENEMYSPAWNER_H__

#include "entity.h"
#include "enemy.h"

// Library includes:
#include <SDL.h>
#include <SDL_image.h>
#include <vector>

class Enemy;

class EnemySpawner : public Entity
{
public:
	EnemySpawner();
	~EnemySpawner();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SpawnEnemy(EnemyType type, BackBuffer& backBuffer);
	void SetSpawnArea(SDL_Rect area);

	void KillEnemy();

	Enemy* GetClosestEnemy();
	Enemy* GetMostHealthEnemy();
	Enemy* GetLeastHealthEnemy();

	std::vector<Enemy*> GetEnemies();

protected:
private:
	Position GetSpawnPosition();
	void UpdateTargetEnemies(Enemy* enemy);

	//Member data
public:
protected:
private:
	std::vector<Enemy*> m_spawnedEnemies;
	std::vector<Position> m_spawnPositions;
	SDL_Rect m_spawnArea;

	Enemy* m_closest;
	Enemy* m_mostHealth;
	Enemy* m_leastHealth;
};

#endif // __ENEMYSPAWNER_H__