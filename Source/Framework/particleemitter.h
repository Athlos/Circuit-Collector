#ifndef __PARTICLEEMITTER_H__
#define __PARTICLEEMITTER_H__

#include "particle.h"
#include "entity.h"
#include "alienexplosionparticle.h"
#include "playerbullettrailparticle.h"
#include <vector>



class ParticleEmitter : public Entity
{
public:
	ParticleEmitter();
	~ParticleEmitter();
	void SpawnNewParticles(int x, int y, int amount, BackBuffer* backbuffer, ParticleType T);
	bool Initialise(Sprite* sprite);

	void SetParticleTimer(float timer);

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
protected:
private:

	//Member Data:
public:
protected:
	std::vector<Particle*> m_particles;
	//how often particles spawn
	float m_particleTimer;
private:

};
#endif //__PARTICLEEMITTER_H__