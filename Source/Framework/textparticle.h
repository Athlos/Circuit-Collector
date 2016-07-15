#pragma once

#include "particle.h"
#include <string>

class Label;

class TextParticle : public Particle
{
public:
	TextParticle();
	~TextParticle();

	void Process(float deltaTime);
	void Draw(BackBuffer& backbuffer);

	void SpawnParticle(int x, int y, std::string message);

	//Variables
protected:
	Label* m_text;
};

