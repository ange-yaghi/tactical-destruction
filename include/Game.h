#ifndef GAME_H
#define GAME_H

#include <world.h>
#include <ball.h>
#include <blast_square.h>

#define ASSET_PATH "../../assets/"

int BallSortCompare(const Ball *A, const Ball *B);
int BallRowSortCompare(const Ball *A, const Ball *B);

class Game {
public:
	struct Connection {
		int Connections[3];
	};

public:
	Game();
	~Game();

	void OnBallSelected(Ball *ball);
	bool IsBallSelected(int ID) const;

	void LoadImages();

	void CreateBalls();

	Image m_whiteBall;
	Image m_blackBall;
	Image m_redBall;
	Image m_shadow;
	Animation m_selectionAnimation;
	Animation m_hoverAnimation;
	Animation m_explosion;

	Image *m_connectionImages;
	Image *m_lockedImages;

	// GUI

	Image m_sidePanel;

	void Process();

	Ball *GetBall(int x, int y) const;
	bool IsValidHover(const Ball *ball) const;

	void DeleteBall(Ball *ball);
	void MoveBallDown(Ball *ball);

	void DetonateAllSquares();
	bool DetonateSquare();

	void GenerateBlastSquares();
	void RefineBlastSquares();

	bool InBlast(const Ball *ball) const {
		if (ball == nullptr) return false;
		else {
			bool found = m_blastArea.FindItem(ball) != NULL;
			return found;
		}
	}

	bool IsSquare(Ball *ball, BlastSquare *square) const;
	int CommonCells(const BlastSquare *square) const;

	Vector2 GetNominal(int row, int column) const;
	bool IsSettled() const;
	bool IsExploding() const;

	bool EliminateSquare(int *nBlack, int *nWhite);

	int m_gridStartX;
	int m_gridStartY;

	int m_gridHeight;
	int m_gridWidth;

	void ClearGameBoard();

	bool m_creatingNewGameBoard;

	int m_nextDetonate;

	bool m_detonating;
	bool m_gameOver;
	bool m_quit;
	double m_finalScore;
	double m_possibleScore;
	double m_scorePerObject;
	int m_nMoves;
	double m_totalTime;

	Connection m_connectionMap[10];

	int GetScore() const;

protected:
	Ball *m_ball1;
	Ball *m_ball2;

	Ball **m_balls;

	Container<Ball, 0> m_blastArea;
	Container<BlastSquare, 0> m_blastSquares;

public:
	World m_world;
};

#endif /* WORLD_H */
