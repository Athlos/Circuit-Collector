// 717310 C++ SDL Framework

// This include:
#include "texturemanager.h"

// Local includes:
#include "texture.h"
#include "logmanager.h"

// Library includes:
#include <SDL.h>

TextureManager::TextureManager()
: m_pRenderer(0)
{

}

TextureManager::~TextureManager()
{
	std::map<std::string, Texture*>::iterator textureIter = m_pLoadedTextures.begin();

	while (textureIter != m_pLoadedTextures.end())
	{
		if (textureIter->second != 0)
		{
			delete textureIter->second;
			textureIter = m_pLoadedTextures.erase(textureIter);
		}
		else
		{
			++textureIter;
		}
	}

	//m_pLoadedTextures.erase(m_pLoadedTextures.begin(), m_pLoadedTextures.end());
}

bool 
TextureManager::Initialise(SDL_Renderer* pRenderer)
{
	m_pRenderer = pRenderer;

	return (true);
}

Texture*
TextureManager::GetTexture(const char* pcFilename)
{
	Texture* pTexture = 0;

	if (m_pLoadedTextures.find(pcFilename) == m_pLoadedTextures.end())
	{
		// Not already loaded... so load...
		pTexture = new Texture();
		if (!pTexture->Initialise(pcFilename, m_pRenderer))
		{
			LogManager::GetInstance().Log("Texture Failed to Init!");
		}
		
		m_pLoadedTextures[pcFilename] = pTexture;
	}
	else
	{
		// Is already loaded...
		pTexture = m_pLoadedTextures[pcFilename];
	}

	return (pTexture);
}

