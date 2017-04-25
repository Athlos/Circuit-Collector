// This include:
#include "logmanager.h"

// Library includes:
#include <Windows.h>
#include <SDL.h>

// Static Members:
LogManager* LogManager::sm_pInstance = 0;

LogManager& 
LogManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new LogManager();
	}

	return (*sm_pInstance);
}

LogManager::LogManager()
{
	
}

LogManager::~LogManager()
{
	
}

void LogManager::DestroyInstance()
{
	delete sm_pInstance;
	sm_pInstance = 0;
}

void LogManager::Log(const char* pcMessage)
{
	OutputDebugStringA(pcMessage);
	OutputDebugStringA("\n"); 

	//SDL_Log(pcMessage);
}

