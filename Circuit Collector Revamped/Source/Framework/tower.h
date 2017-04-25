#ifndef __TOWER_H__
#define __TOWER_H__

#include "entity.h"

class Projectile;
class Enemy;
class Particle;

class Tower : public Entity
{
	//Member methods:
public:
	Tower();
	~Tower();

	void InitialiseProjectile(Projectile* projectile);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	bool IsReadyToShoot();
	Projectile* GetProjectile();

	void ResetCooldown();

protected:
private:

	//Member data:
public:
protected:
private:
	Projectile* m_projectile;
	Enemy* m_target;
	float m_secondsPerShot;

	float m_secondsElapsedSinceShot;
};
#endif //__TOWER_H__
