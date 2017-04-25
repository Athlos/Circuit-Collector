#ifndef __MAPTILE_H__
#define __MAPTILE_H__

#include "entity.h"

class MapTile : public Entity
{
public:
	MapTile();
	~MapTile();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);

	//Member data
public:
	int m_x;
	int m_y;
protected:
private:

};

#endif //__MAPTILE_H__