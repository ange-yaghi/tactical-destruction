#ifndef CONTAINER_H
#define CONTAINER_H

template<typename TYPE, unsigned int SIZE>
class Container {
public:
	Container() {
		m_nObjects = 0;

		m_sortDirection = 1;
		m_sortingMethod = 0;

		if (SIZE) {
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
	int(*m_sortingMethod)(const TYPE *A, const TYPE *B);

	bool m_dynamicallyAllocated;

	bool AdjustSize(int unit = 32) {
		bool adjusted = false;
		while ((m_maxSize - m_nObjects) < unit) {
			m_maxSize += unit;
			adjusted = true;
		}

		if (adjusted) Reallocate();

		return adjusted;
	}

	void Allocate(int size) {
		m_maxSize = size;
		Reallocate();
	}

	void Reallocate() {
		TYPE **newArray = new TYPE *[m_maxSize];

		int i = 0;
		for (; i < m_nObjects; i++) {
			newArray[i] = m_array[i];
		}

		delete[] m_array;
		m_array = newArray;
	}

	void SetAscending(bool ascending) {
		if (ascending) m_sortDirection = 1;
		else m_sortDirection = -1;
	}

	void Clear(bool destroy = false) {
		unsigned int count = destroy ? m_nObjects : SIZE;

		if (destroy) {
			for (unsigned int i = 0; i < count; i++) {
				if (destroy) delete m_array[i];
			}
		}

		for (unsigned int i = 0; i < count; i++) {
			m_array[i] = nullptr;
		}

		m_nObjects = 0;
	}

	void Destroy(bool destroy = true) {
		Clear(destroy);
		delete[] m_array;
	}

	void Swap(int index1, int index2) {
		TYPE *temp = m_array[index1];
		m_array[index1] = m_array[index2];
		m_array[index2] = temp;
	}

	TYPE *AddObject() {
		return AddObject(new TYPE);
	}

	TYPE *AddObject(TYPE *object) {
		if (!m_sortingMethod || m_nObjects == 0) {
			m_array[m_nObjects] = object;
			m_nObjects++;
		}
		else {
			// Check Bounds
			int index;

			index = FindIndex(object);

			for (int i = m_nObjects - 1; i >= index; i--) m_array[i + 1] = m_array[i];
			m_array[index] = object;

			m_nObjects++;
		}

		return object;
	}

	void SetSortMethod(int(*compare)(const TYPE *A, const TYPE *B)) {
		m_sortingMethod = compare;
	}

	int FindIndex(const TYPE *object) const {
		int bottom = 0;
		int top = m_nObjects - 1;
		int middle = bottom + (top - bottom) / 2;

		while (bottom <= top) {
			middle = bottom + (top - bottom) / 2;
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

	int FindItemIndex(const TYPE *ref) const {
		int bottom = 0;
		int top = m_nObjects - 1;
		int middle;

		while (bottom <= top) {
			middle = bottom + (top - bottom) / 2;
			int compare = m_sortingMethod(ref, m_array[middle]) * m_sortDirection;

			if (compare == 0) return middle;
			else if (compare > 0) bottom = middle + 1;
			else top = middle - 1;
		}

		return -1;
	}

	TYPE *FindItem(const TYPE *ref) const {
		int bottom = 0;
		int top = m_nObjects - 1;
		int middle;

		while (bottom <= top) {
			middle = bottom + (top - bottom) / 2;
			int compare = m_sortingMethod(ref, m_array[middle]) * m_sortDirection;

			if (compare == 0) return m_array[middle];
			else if (compare > 0) bottom = middle + 1;
			else top = middle - 1;
		}

		return 0;
	}

	void DeleteObject(int index, bool destroy = true) {
		if (index < 0 || index >= m_nObjects) return;
		if (destroy) delete m_array[index];

		int i = index;
		m_array[index] = 0;

		for (; i < m_nObjects - 1; i++) m_array[i] = m_array[i + 1];

		m_array[m_nObjects - 1] = 0;

		m_nObjects--;
	}

	void DeleteObject(TYPE *ref, bool destroy = true) {
		DeleteObject(FindItemIndex(ref), destroy);
	}

	void Sort(int(*compare)(const TYPE *A, const TYPE *B)) {
		SortRange(compare, 0, m_nObjects - 1);
	}

	void Sort() {
		Sort(m_sortingMethod);
	}

	int Partition(int(*compare)(const TYPE *A, const TYPE *B), int left, int right, int pivot) {
		TYPE *pivotValue = m_array[pivot];
		Swap(pivot, right);

		int storeIndex = left;
		for (int i = left; i < right; i++) {
			int comparison = compare(m_array[i], pivotValue) * m_sortDirection;
			if (comparison < 0) {
				Swap(i, storeIndex);
				storeIndex++;
			}
		}

		Swap(storeIndex, right);
		return storeIndex;
	}

	void SortRange(int(*compare)(const TYPE *A, const TYPE *B), int low, int high) {
		if (low < high) {
			int pivotIndex = high + (low - high) / 2;
			int pivotNewIndex = Partition(compare, low, high, pivotIndex);

			SortRange(compare, low, pivotNewIndex - 1);
			SortRange(compare, pivotNewIndex + 1, high);
		}
	}
};

#endif /* CONTAINER_H */
