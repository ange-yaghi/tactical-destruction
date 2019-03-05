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

template<typename TYPE, unsigned int SIZE>
class Container
{

public:
	Container() 
	{
	
		m_nObjects = 0;

		m_sortDirection = 1;
		m_sortingMethod = 0;

		if (SIZE) 
		{
			m_array = new TYPE *[SIZE];
			Clear();
		}
		else m_array = 0;

		m_maxSize = SIZE;

		m_dynamicallyAllocated = true;

	}

	~Container() { Destroy(m_dynamicallyAllocated); }

	TYPE **m_array;

	int m_nObjects;
	int m_sortDirection;
	int m_maxSize;
	int (*m_sortingMethod)(TYPE *A, TYPE *B);

	bool m_dynamicallyAllocated;

	bool AdjustSize(int unit=32)
	{

		bool adjusted = false;
		while ( (m_maxSize - m_nObjects) < unit)
		{

			m_maxSize += unit;
			adjusted = true;

		}

		if (adjusted) Reallocate();

		return adjusted;

	}

	void Allocate(int size)
	{

		m_maxSize = size;
		Reallocate();

	}

	void Reallocate()
	{

		TYPE **newArray = new TYPE *[m_maxSize];

		int i=0;
		for (; i < m_nObjects; i++)
		{

			newArray[i] = m_array[i];

		}

		delete [] m_array;
		m_array = newArray;

	}

	void SetAscending(bool ascending)
	{

		if (ascending) m_sortDirection = 1;
		else m_sortDirection = -1;

	}

	void Clear(bool destroy = false)
	{

		unsigned int i=0;
		unsigned int count;
		if (destroy) count = m_nObjects;
		else count = SIZE;

		for (; i < count; i++) 
		{
			if (destroy) delete m_array[i];
			m_array[i] = 0;
		}

		m_nObjects = 0;

	}

	void Destroy(bool destroy=true)
	{

		Clear(destroy);
		delete [] m_array;

	}

	void Swap(int index1, int index2)
	{

		TYPE *temp = m_array[index1];
		m_array[index1] = m_array[index2];
		m_array[index2] = temp;

	}

	TYPE *AddObject()
	{

		return AddObject(new TYPE);

	}

	TYPE *AddObject(TYPE *object)
	{

		if (!m_sortingMethod || m_nObjects == 0)
		{

			m_array[m_nObjects] = object;
			m_nObjects++;

		}

		else
		{

			// Check Bounds
			int index;

			index = FindIndex(object);

			int i=m_nObjects-1;
			for (; i >= index; i--) m_array[i+1] = m_array[i];
			m_array[index] = object;

			m_nObjects++;

		}

		return object;

	}

	void SetSortMethod(int (*compare)(TYPE *A, TYPE *B))
	{

		m_sortingMethod = compare;

	}

	int FindIndex(TYPE *object)
	{

		int bottom = 0;
		int top = m_nObjects - 1;
		int middle = bottom + (top-bottom) / 2;

		while (bottom <= top)
		{

			middle = bottom + (top-bottom) / 2;
			int compare = m_sortingMethod(object, m_array[middle]) * m_sortDirection;
			int compare2;
			if (middle + 1 < m_nObjects) compare2 = m_sortingMethod(object, m_array[middle + 1]) * m_sortDirection;
			else compare2 = -1;

			if (compare >= 0 && compare2 <= 0) 
				return middle + 1;

			else if (compare > 0) bottom = middle + 1;
			else top = middle - 1;

		}

		return middle;

	}

	int FindItemIndex(TYPE *ref)
	{

		int bottom = 0;
		int top = m_nObjects - 1;
		int middle;

		while (bottom <= top)
		{

			middle = bottom + (top-bottom) / 2;
			int compare = m_sortingMethod(ref, m_array[middle]) * m_sortDirection;

			if (compare == 0) return middle;
			else if (compare > 0) bottom = middle + 1;
			else top = middle - 1;

		}

		return -1;

	}

	TYPE *FindItem(TYPE *ref)
	{

		int bottom = 0;
		int top = m_nObjects - 1;
		int middle;

		while (bottom <= top)
		{

			middle = bottom + (top-bottom) / 2;
			int compare = m_sortingMethod(ref, m_array[middle]) * m_sortDirection;

			if (compare == 0) return m_array[middle];
			else if (compare > 0) bottom = middle + 1;
			else top = middle - 1;

		}

		return 0;

	}

	void DeleteObject(int index, bool destroy=true)
	{
		
		if (index < 0 || index >= m_nObjects) return;
		if (destroy) delete m_array[index];

		int i=index;
		m_array[index] = 0;

		for (; i < m_nObjects-1; i++) m_array[i] = m_array[i + 1];

		m_array[m_nObjects - 1] = 0; 

		m_nObjects--;

	}

	void DeleteObject(TYPE *ref, bool destroy=true)
	{

		DeleteObject(FindItemIndex(ref), destroy);

	}

	void Sort(int (*compare)(TYPE *A, TYPE *B))
	{

		SortRange(compare, 0, m_nObjects-1);

	}

	void Sort()
	{

		Sort(m_sortingMethod);

	}

	int Partition(int (*compare)(TYPE *A, TYPE *B), int left, int right, int pivot)
	{

		TYPE *pivotValue = m_array[pivot];
		Swap(pivot, right);

		int storeIndex = left;
		int i=left;
		for (; i < right; i++)
		{

			int comparison = compare(m_array[i], pivotValue) * m_sortDirection;
			if (comparison < 0)
			{

				Swap(i, storeIndex);
				storeIndex++;

			}

		}

		Swap(storeIndex, right);
		return storeIndex;

	}

	void SortRange(int (*compare)(TYPE *A, TYPE *B), int low, int high)
	{

		if (low < high)
		{

			int pivotIndex = high + (low - high) / 2;
			int pivotNewIndex = Partition(compare, low, high, pivotIndex);

			SortRange(compare, low, pivotNewIndex - 1);
			SortRange(compare, pivotNewIndex + 1, high);

		}

	}

};

#endif /* LIST_ALLOCATOR_H */