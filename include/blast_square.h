#ifndef BLAST_SQUARE_H
#define BLAST_SQUARE_H

// Forward declarations
class Ball;

class BlastSquare {
public:
	BlastSquare();
	~BlastSquare();

	union {
		struct {
			Ball *Reference;

			Ball *TopRight;
			Ball *BottomRight;
			Ball *BottomLeft;
		};

		Ball *Balls[4];
	};

	bool IsValid() const;
	int CommonSquares(const BlastSquare *cmp) const;
};

#endif /* BLAST_SQUARE_H */
