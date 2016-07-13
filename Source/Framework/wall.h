#ifndef __WALL_H__
#define __WALL_H__

class AnimatedSprite;
class Sprite;
class BackBuffer;
class Texture;
class Entity;

#include "entity.h"


class Wall : public Entity
{
public:
	Wall();
	~Wall();
	bool Wall::IsCollidingWith(Entity& e);
	void SetCurrentHealth(int health);
	int GetCurrentHealth();
	void SetMaxHealth(int health);
	int GetMaxHealth();
	void TakeDamage(int amount);
protected:
private:

	//Member Data:
public:
protected:
	int m_currentHealth;
	int m_maxHealth;
private:

};
#endif //__WALL_H__

