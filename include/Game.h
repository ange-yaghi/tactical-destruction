#ifndef GAME_H
#define GAME_H

#include "World.h"
#include "Ball.h"

//#define GAME_BOARD_WIDTH 10
//#define GAME_BOARD_HEIGHT 9

#define ASSET_PATH "../../assets/"

int BallSortCompare(Ball *A, Ball *B);

int BallRowSortCompare(Ball *A, Ball *B);

class BlastSquare
{

public:

	BlastSquare() {}
	~BlastSquare() {}

	// Top Left

	union
	{
	
		struct
		{

			Ball *Reference;

			Ball *TopRight;
			Ball *BottomRight;
			Ball *BottomLeft;

		};

		Ball *Balls[4];

	};

	bool IsValid()
	{

		return (Reference && TopRight && BottomRight && BottomLeft);

	}

	int CommonSquares(BlastSquare *cmp)
	{

		if (!cmp->IsValid()) return 0;
		if (cmp->Reference->m_color != Reference->m_color) return 0;
		if (cmp == this) return 0;

		int count=0;

		for(int i=0; i < 4; i++)
		{

			for(int j=0; j < 4; j++)
			{

				if (Balls[i] == cmp->Balls[j]) 
				{

					count++;
					break;

				}

			}

		}

		return count;

	}

};

struct Connection
{

	int Connections[3];

};

class Game
{

public:

	Game()
	{

		m_gridStartX = 60;
		m_gridStartY = 60;

		m_ball1 = m_ball2 = 0;

		m_blastArea.SetSortMethod(&BallRowSortCompare);
		m_blastArea.m_dynamicallyAllocated = false;

		m_creatingNewGameBoard = false;

		m_nextDetonate = 0;

		m_gameOver = false;
		m_detonating = false;
		m_quit = false;
		m_finalScore = 0;
		m_possibleScore = 0;
		m_nMoves = 0;
		m_totalTime = 0;
		m_scorePerObject = 10;

	}

	~Game() {}

	void OnBallSelected(Ball *ball);
	bool IsBallSelected(int ID);

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

	Ball *GetBall(int x, int y);
	bool IsValidHover(Ball *ball);

	void DetonateBall(Ball *ball);
	void MoveBallDown(Ball *ball);

	void DetonateAllSquares();
	bool DetonateSquare();
	bool ExpandBlast();

	void GenerateBlastSquares();
	void RefineBlastSquares();

	bool InBlast(Ball *ball)
	{

		if (!ball) return false;
		else 
		{

			bool find = m_blastArea.FindItem(ball) != NULL;
			return find;

		}

	}

	bool IsSquare(Ball *ball, BlastSquare *square);
	int CommonCells(BlastSquare *square);

	Vector2 GetNominal(int row, int column);
	bool IsSettled();
	bool IsExploding();

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

	int GetScore();

protected:

	Ball *m_ball1;
	Ball *m_ball2;

	Ball **m_balls;

	Container<Ball, 0> m_blastArea;
	Container<BlastSquare, 0> m_blastSquares;

public:

	World m_world;

};

#endif