#ifndef __BULLET_H__
#define __BULLET_H__

#include "entity.h"
#include "particle.h"

class Bullet : public Entity
{
public:
	Bullet();
	~Bullet();
	void SetDamage(int amount);
	int GetDamage();
	void Process(float deltaTime);
	bool IsOutOfBounds();
	void SetParticleType(ParticleType t);
	ParticleType GetParticleType();
protected:
private:

	//Member Data:
public:
protected:
	int m_damage;
	ParticleType m_particles;
private:

};
#endif //__BULLET_H__

