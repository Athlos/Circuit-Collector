#include "gamemap.h"
#include "maptile.h"
#include "sprite.h"
#include "backbuffer.h"

GameMap::GameMap()
{
	m_mapHeight = 10;
	m_mapWidth = 10;

}


GameMap::~GameMap()
{
}

void GameMap::Process(float deltaTime)
{
	Entity::Process(deltaTime);
}

void GameMap::Draw(BackBuffer& backBuffer)
{
	for (int mx = 0; mx < m_mapWidth; ++mx)
	{
		for (int my = 0; my < m_mapHeight; ++my)
		{
			m_mapTiles.at(mx).at(my)->Draw(backBuffer);
		}
	}
}

void GameMap::Initialise(BackBuffer& backBuffer)
{
	Sprite* dirt = backBuffer.CreateSprite("assets\\dirt.png");
	for (int mx = 0; mx < m_mapWidth; ++mx)
	{
		for (int my = 0; my < m_mapHeight; ++my)
		{
			MapTile* tile = new MapTile();
			tile->Initialise(dirt);
			m_mapTiles.at(mx).push_back(tile);
		}
	}
}