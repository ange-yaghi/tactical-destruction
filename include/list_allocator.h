#ifndef LIST_ALLOCATOR_H
#define LIST_ALLOCATOR_H

#include <memory.h>

template<typename TYPE, int MAX_SIZE>
class ListAllocator {
public:
	ListAllocator() { m_nObjects = 0; }
	~ListAllocator() {}

	// Adds a new object to the list
	void AddObject(TYPE *object) { m_array[m_nObjects++] = *object; }

	// Deletes the object at index from the list
	void Deallocate(int index) { m_array[index] = m_array[--m_nObjects]; }

	int GetNumObjects() { return m_nObjects; }
	TYPE *GetArray() { return m_array; }

protected:
	TYPE m_array[MAX_SIZE];
	int m_nObjects;
};

#endif /* LIST_ALLOCATOR_H */
