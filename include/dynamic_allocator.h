#ifndef DYNAMIC_ALLOCATOR_H
#define DYNAMIC_ALLOCATOR_H

typedef unsigned long long POINTER;

struct MemoryBlock {
	MemoryBlock() { m_nObjects = 0; }
	~MemoryBlock() {}

	POINTER m_start;
	POINTER m_end;

	POINTER GetSize() { return (POINTER)(m_end - m_start); }

	bool m_inUse;

	unsigned int m_nObjects;
};

int MemoryBlockSortCompare(const MemoryBlock *A, const MemoryBlock *B);

template<POINTER MAX_SIZE>
class DynamicAllocator {
public:
	DynamicAllocator() { m_storage = NULL; m_memoryBlocks.SetSortMethod(&MemoryBlockSortCompare); Initialize(); m_showOutput = false; }
	~DynamicAllocator() {}

	void Initialize() {
		if (m_storage) free(m_storage);
		m_memoryBlocks.m_nObjects = 0;

		m_storage = malloc(MAX_SIZE);

		AddMemoryBlock((POINTER)m_storage, (POINTER)m_storage + MAX_SIZE);
	}

	template<typename TYPE>
	TYPE *Allocate(unsigned int n = 1) {
		//if (m_showOutput) DrawBlocks();

		if (n == 0) return 0;

		unsigned long long minDif = ULLONG_MAX;
		int index = -1;

		for (int i = 0; i < m_memoryBlocks.m_nObjects; i++) {

			if (m_memoryBlocks.m_array[i].m_inUse) continue;

			long long dif = m_memoryBlocks.m_array[i].GetSize() - sizeof(TYPE) * n;

			if (dif >= 0) {
				if (dif < minDif) { index = i; minDif = dif; }
				if (!dif) break;
			}
		}

		POINTER location = m_memoryBlocks.m_array[index].m_start;
		MemoryBlock *newBlock = ShrinkBlock(index, sizeof(TYPE) * n);
		newBlock->m_nObjects = n;

		TYPE *output = new((void *)(location)) TYPE;
		for (int i = 1; i < n; i++) {
			new((void *)(location + sizeof(TYPE) * i)) TYPE;
		}

		//if (m_showOutput) 
		//{

		//	std::cout << "New Block: " << newBlock->m_start - (POINTER)m_storage << " - " << newBlock->m_end - (POINTER)m_storage << "\n";

		//	std::cout << "Allocation: " << "Data Size = " << sizeof(TYPE) 
		//		<< " Number = " << n << " Address = " << (location - (POINTER)m_storage)
		//		<< " Total Size = " << sizeof(TYPE) * n << "\n";

		//	DrawBlocks();

		//	system("PAUSE");

		//}

		return output;
	}

	void DrawBlocks() {
#ifdef _DEBUG
		std::cout << "**********************************************************************\n";

		int i = 0;
		for (; i < m_memoryBlocks.m_nObjects; i++)
		{

			if (m_memoryBlocks.m_array[i].m_inUse) std::cout << "Used Block: " << m_memoryBlocks.m_array[i].m_start - (POINTER)m_storage << " - " << m_memoryBlocks.m_array[i].m_end - (POINTER)m_storage <<
				" < " << m_memoryBlocks.m_array[i].m_nObjects << " > " << "\n";

			else
			{

				std::cout << "Free Block: " << m_memoryBlocks.m_array[i].m_start - (POINTER)m_storage << " - " << m_memoryBlocks.m_array[i].m_end - (POINTER)m_storage <<
					" < " << m_memoryBlocks.m_array[i].m_nObjects << " > " << "\n";

			}

		}

		std::cout << "**********************************************************************\n";
#endif
	}

	template<typename TYPE>
	void Deallocate(TYPE *memory) {
		MemoryBlock block;
		block.m_inUse = true;
		block.m_start = (POINTER)memory;

		MemoryBlock *delBlock = m_memoryBlocks.FindItem(&block);
		int n = delBlock->m_nObjects;
		for (int i = 0; i < delBlock->m_nObjects; i++) {
			(memory + i)->~TYPE();
		}

		m_memoryBlocks.DeleteObject(&block);

		block.m_inUse = false;
		AddMemoryBlock(block.m_start, block.m_start + sizeof(TYPE) * n, true);

		//if (m_showOutput)
		//{

		//	std::cout << "Free\n";
		//	DrawBlocks();

		//}
	}

	MemoryBlock* ShrinkBlock(int index, POINTER amount) {
		if (amount == m_memoryBlocks.m_array[index].GetSize()) {
			MemoryBlock block = m_memoryBlocks.m_array[index];
			block.m_inUse = true;

			m_memoryBlocks.DeleteObject(index);
			return m_memoryBlocks.AddObject(&block);
		}
		else {
			POINTER start = m_memoryBlocks.m_array[index].m_start;
			m_memoryBlocks.m_array[index].m_start += amount;
			return AddMemoryBlock(start, start + amount, false);
		}
	}

	bool m_showOutput;

protected:
	MemoryBlock *AddMemoryBlock(POINTER start, POINTER end, bool free = true) {
		MemoryBlock block;
		block.m_start = start;
		block.m_end = end;
		block.m_inUse = !free;

		if (!free) return m_memoryBlocks.AddObject(&block);
		else {
			MemoryBlock *newBlock = m_memoryBlocks.AddObject(&block);
			int index = m_memoryBlocks.FindItemIndex(newBlock);

			MemoryBlock *left = newBlock, *right = newBlock;

			if (index > 0 && !m_memoryBlocks.m_array[index - 1].m_inUse) left = &m_memoryBlocks.m_array[index - 1];
			if (index < m_memoryBlocks.m_nObjects - 1 && !m_memoryBlocks.m_array[index + 1].m_inUse) right = &m_memoryBlocks.m_array[index + 1];
			left->m_end = right->m_end;

			if (right != newBlock && left != newBlock) {
				m_memoryBlocks.DeleteObject(right);
				m_memoryBlocks.DeleteObject(newBlock);
			}
			else if (right != left) {
				m_memoryBlocks.DeleteObject(right);
			}

			return left;
		}
	}

	StaticContainer<MemoryBlock, 65536> m_memoryBlocks;

	void *m_storage;
};

#endif /* DYNAMIC_ALLOCATOR_H */
