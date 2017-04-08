#ifndef __RETICLE_H__
#define __RETICLE_H__

class AnimatedSprite;
class Sprite;
class BackBuffer;
class Texture;
class Entity;

#include "entity.h"
#include "animatedsprite.h"

#include <cassert>
#include <vector>

class Label;
class ParticleEmitter;


class Reticle : public Entity
{
public:
	Reticle();
	~Reticle();
	void SetDamage(int amount);
	int GetDamage();
	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	void SetAnimation(AnimatedSprite* anim);
	void Draw(BackBuffer& backBuffer, bool hit);
	bool IsFiring();
	void SetFiring();
protected:
private:

	//Member Data:
public:
protected:
	int m_damage;
	AnimatedSprite* m_damageAnim;
private:

};
#endif //__RETICLE_H__