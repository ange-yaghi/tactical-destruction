#include "World.h"

int GameObjectSortCompare(GameObject *A, GameObject *B)
{

	if (A->m_systemID > B->m_systemID) return 1;
	if (A->m_systemID < B->m_systemID) return -1;

	return 0;

}

void World::Render()
{

	int i=0;
	for(; i < m_gameObjects.m_nObjects; i++)
	{

		m_gameObjects.m_array[i]->RenderShadow();

	}

	i=0;
	for(; i < m_gameObjects.m_nObjects; i++)
	{

		m_gameObjects.m_array[i]->Render();

	}

}

void World::Process()
{

	int i=0;
	for(; i < m_gameObjects.m_nObjects; i++)
	{

		m_gameObjects.m_array[i]->Process();

	}

}

void World::ProcessInput()
{

	int i=0;
	for(; i < m_gameObjects.m_nObjects; i++)
	{

		m_gameObjects.m_array[i]->ProcessInput();

	}

}