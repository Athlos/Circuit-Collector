#ifndef __PROJECTILESPAWNER_H__
#define __PROJECTILESPAWNER_H__

#include "entity.h"

#include <vector>

class Projectile;
class Tower;

class ProjectileSpawner : public Entity
{
public:
	ProjectileSpawner();
	~ProjectileSpawner();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	void SpawnProjectile(Tower* tower);

	std::vector<Projectile*> GetProjectiles();

	//Member data
public:
protected:
private:
	std::vector<Projectile*> m_projectiles;
};

#endif // __PROJECTILESPAWNER_H__