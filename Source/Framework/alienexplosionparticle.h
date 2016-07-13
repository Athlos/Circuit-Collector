#ifndef __ALIENEXPLOSIONPARTICLE_H__
#define __ALIENEXPLOSIONPARTICLE_H__

#include "particle.h"
#include "backbuffer.h"
#include <cmath>
#include <ctime>


class AlienExplosionParticle : public Particle
{
public:
	AlienExplosionParticle();
	~AlienExplosionParticle();
	
	bool Initialise(Sprite* texture);

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);
	void SpawnParticle(int x, int y, BackBuffer* backbuffer, ParticleType t);
protected:
private:

	//Member Data:
public:
protected:
private:

};
#endif //__ALIENEXPLOSIONPARTICLE_H__

