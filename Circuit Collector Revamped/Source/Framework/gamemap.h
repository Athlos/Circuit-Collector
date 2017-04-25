#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__

#include "entity.h"

#include <vector>

class MapTile;

// Class
class GameMap : public Entity
{
	// Member methods:
public:
	GameMap();
	~GameMap();

	void Process(float deltaTime);
	void Draw(BackBuffer& backBuffer);
	
	void Initialise(BackBuffer& backBuffer);

	// Member data:
public:
	int m_mapHeight;
	int m_mapWidth;
private:
	std::vector<std::vector<MapTile*>> m_mapTiles;
};

#endif // __GAMEMAP_H__