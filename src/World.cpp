#include <world.h>

int GameObjectSortCompare(const GameObject *A, const GameObject *B) {
	if (A->m_systemID > B->m_systemID) return 1;
	if (A->m_systemID < B->m_systemID) return -1;

	return 0;
}

void World::Render() {
	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->RenderShadow();
	}

	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->Render();
	}
}

void World::Process() {
	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->ProcessAnimations();
	}

	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->Integrate();
	}

	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->CheckAnimations();
	}

	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->Process();
	}
}

void World::ProcessInput() {
	for (int i = 0; i < m_gameObjects.m_nObjects; i++) {
		m_gameObjects.m_array[i]->ProcessInput();
	}
}
