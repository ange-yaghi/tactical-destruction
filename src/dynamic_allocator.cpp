#include <dynamic_allocator.h>

int MemoryBlockSortCompare(const MemoryBlock *A, const MemoryBlock *B) {

	if (A->m_start < B->m_start) return -1;
	if (A->m_start > B->m_start) return 1;
	return 0;
}