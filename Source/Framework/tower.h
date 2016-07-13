#ifndef __TOWER_H__
#define __TOWER_H__


#include "entity.h"
#include "animatedsprite.h"

#include <cmath>
#include <ctime>


enum TowerType
{
	RAIL,
	BEAM,
	ROCKET
};

class Tower : public Entity
{
public:
	
	Tower();
	~Tower();
	
	void SetData(int damage, float firingSpeed);
	bool ReadyToFire();
	void Process(float deltaTime);

	void SetType(TowerType t);
	TowerType GetType();
	void SetFiringSpeed(float speed);
	float GetFiringSpeed();
	void SetDamage(int amount);
	int GetDamage();
	int GetLevel();
	void SetLevel(int level);
	void Draw(BackBuffer& backBuffer, bool spawn);
	void SetAnimation(AnimatedSprite* anim);
	void SetSpawning();

protected:
private:

	//Member Data:
public:
protected:
	float m_firingSpeed;
	float m_timePassed;
	int m_damage;
	TowerType m_type;
	int m_level;
	AnimatedSprite* m_spawnAnim;
	bool m_spawning;
private:

};
#endif //__TOWER_H__

