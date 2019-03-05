#ifndef WORLD_H
#define WORLD_H

#include "GameObject.h"
#include "Ball.h"
#include <dynamic_allocator.h>
#include <container.h>
#include "GUI.h"

#define MAX_OBJECTS 2048

#define OBJECT_ADD(type, functionName)							\
	type *functionName()										\
	{															\
		type *object = m_gameObjectAllocator.Allocate<type>();	\
		object->m_systemID = m_currentID++;						\
		m_gameObjects.AddObject((GameObject *)object);			\
		object->m_world = this;									\
		object->Register();										\
		return object;											\
	}															\

int GameObjectSortCompare(const GameObject *A, const GameObject *B);

class World
{

public:

	World() 
	{ 

		m_gameObjects.m_dynamicallyAllocated = false;

		m_gameObjects.SetSortMethod(&GameObjectSortCompare); 
		m_gameObjectAllocator.m_showOutput = false;
		m_currentID = 0; 
	
	}

	~World() {}

	OBJECT_ADD(PlayerObject, AddPlayer);
	OBJECT_ADD(Ball, AddBall);
	OBJECT_ADD(GUI, AddGUI);
	OBJECT_ADD(Button, AddButton);
	OBJECT_ADD(SimpleLabel, AddSimpleLabel);

	void DeleteObject(GameObject *target)
	{

		GameObject REF;
		REF.m_systemID = target->m_systemID;
		REF.SetType(target->GetType());
		target->OnDestroy();

		assert(m_gameObjects.FindItem(&REF));

		m_gameObjects.DeleteObject((GameObject *)(&REF), false);
		m_gameObjectAllocator.Deallocate(target);

		//int i=0;
		//for(; i < m_gameObjects.m_nObjects; i++)
		//{

		//	std::cout << m_gameObjects.m_array[i]->m_systemID << "\n";

		//}

	}

	void Render();
	void Process();
	void ProcessInput();

protected:

	Container<GameObject, 2048> m_gameObjects;
	DynamicAllocator<10 * MB> m_gameObjectAllocator;

	int m_currentID;

};

#endif
