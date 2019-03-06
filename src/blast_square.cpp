#include <blast_square.h>

#include <ball.h>

BlastSquare::BlastSquare() {}

BlastSquare::~BlastSquare() {}

bool BlastSquare::IsValid() const {
	return (
		Reference != nullptr 
		&& TopRight != nullptr
		&& BottomRight != nullptr
		&& BottomLeft != nullptr);
}

int BlastSquare::CommonSquares(const BlastSquare *cmp) const {
	if (!cmp->IsValid()) return 0;
	if (cmp->Reference->m_color != Reference->m_color) return 0;
	if (cmp == this) return 0;

	int count = 0;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (Balls[i] == cmp->Balls[j]) {
				count++;
				break;
			}
		}
	}

	return count;
}
