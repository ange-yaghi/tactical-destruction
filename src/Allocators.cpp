#include "Allocators.h"

int MemoryBlockSortCompare(MemoryBlock *A, MemoryBlock *B)
{

	if (A->m_start < B->m_start) return -1;
	if (A->m_start > B->m_start) return 1;
	return 0;

}