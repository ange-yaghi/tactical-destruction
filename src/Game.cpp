#include "Game.h"
#include "InputManager.h"
#include "DrawingManager.h"
#include "Timing.h"
#include <math.h>
#include <fstream>

int BallSortCompare(const Ball *A, const Ball *B) {
	if (A->m_systemID < B->m_systemID) return -1;
	if (A->m_systemID > B->m_systemID) return 1;
	return 0;
}

int BallRowSortCompare(const Ball *A, const Ball *B) {
	if (A->m_row > B->m_row) return 1;
	if (A->m_row < B->m_row) return -1;

	if (A->m_column > B->m_column) return 1;
	if (A->m_column < B->m_column) return -1;

	//if (A->m_systemID > B->m_systemID) return 1;
	//if (A->m_systemID < B->m_systemID) return -1;

	return 0;
}

void Game::OnBallSelected(Ball *ball)
{


	// TEMP
	//DetonateBall(ball);
	//return;
	// END TEMP

	if (!m_ball1 && ball->m_color != Ball::RED) 
	{

		m_ball1 = ball;
		return;

	}

	else if (m_ball1 && m_ball1->m_systemID == ball->m_systemID) 
	{
		
		m_ball1 = 0;
		return;

	}

	if (!m_ball2)
	{

		if (ball->m_color != Ball::RED)
		{

			m_ball2 = ball;

			Ball::COLOR temp = m_ball1->m_color;
			int tempConnection = m_ball1->m_detonationConnection;

			m_ball1->m_color = m_ball2->m_color;
			m_ball2->m_color = temp;

			m_ball1->m_detonationConnection = m_ball2->m_detonationConnection;
			m_ball2->m_detonationConnection = tempConnection;

			m_ball2 = 0;
			m_ball1 = 0;

			m_nMoves++;

		}

		return;

	}

	else if (m_ball2->m_systemID == ball->m_systemID)
	{

		m_ball2 = 0;
		return;

	}

}

bool Game::IsBallSelected(int ID)
{

	return ((m_ball2 && m_ball2->m_systemID == ID) || (m_ball1 && m_ball1->m_systemID == ID));

}

bool Game::IsValidHover(Ball *ball)
{

	if (m_creatingNewGameBoard || !IsSettled() || IsExploding() || ball->m_color == Ball::RED) return false;

	if (!m_ball1) return true;

	if (abs(ball->m_column - m_ball1->m_column) <= 1)
	{

		if (abs(ball->m_row - m_ball1->m_row) <= 1)
		{

			return (ball->m_column == m_ball1->m_column || ball->m_row == m_ball1->m_row);

		}

	}

	return false;

}

void Game::DetonateBall(Ball *ball)
{

	int x, y;

	x = ball->m_column;
	y = ball->m_row;

	m_blastArea.DeleteObject(ball, false);
	m_balls[y * m_gridWidth +x] = 0;
	m_world.DeleteObject(ball);

	// Move the whole column down

	int i=y-1;

	for(; i >= 0; i--)
	{

		MoveBallDown(GetBall(i, ball->m_column));

	}

	if (!m_gameOver && !m_creatingNewGameBoard)
	{

		m_finalScore++;

	}

}

void Game::MoveBallDown(Ball *ball)
{

	if (!ball) return;

	m_balls[(ball->m_row + 1) * m_gridWidth + ball->m_column] = m_balls[ball->m_row * m_gridWidth + ball->m_column];
	m_balls[ball->m_row * m_gridWidth + ball->m_column] = 0;

	ball->m_row++;

	// Temp location placement

	//ball->m_location.y += 75;

}

void Game::LoadImages()
{

	std::ifstream configurationFile("configuration.txt");

	configurationFile >> m_gridWidth;
	configurationFile >> m_gridHeight;

	configurationFile.close();

	m_selectionAnimation.LoadAnimation(ASSET_PATH "Ball Animation/SelectionRing", "tga", 0, 10);
	m_selectionAnimation.SetPlayMode(Animation::PM_REVERSE);
	m_selectionAnimation.SetTotalLength(0.25);

	m_hoverAnimation.LoadAnimation(ASSET_PATH "Ball Animation/HoverRing", "tga", 0, 10);
	m_hoverAnimation.SetPlayMode(Animation::PM_REVERSE);
	m_hoverAnimation.SetTotalLength(0.25);

	m_explosion.LoadAnimation(ASSET_PATH "Ball Animation/ExplosionAnimation", "tga", 0, 30);
	m_explosion.SetPlayMode(Animation::PM_REVERSE);
	m_explosion.SetTotalLength(0.5);

	m_blackBall.LoadImage(ASSET_PATH "Ball Animation/BlackBall.tga");
	m_whiteBall.LoadImage(ASSET_PATH "Ball Animation/WhiteBall.tga");
	m_redBall.LoadImage(ASSET_PATH "Ball Animation/RedBall.tga");
	m_shadow.LoadImage(ASSET_PATH "Ball Animation/Shadow.tga");

	m_sidePanel.LoadImage(ASSET_PATH "GUI/TacticalDestruction.png");

	//m_gridWidth = (DRAWING_MANAGER.GetScreenWidth() - m_sidePanel.GetWidth() - m_gridStartX * 2) / 75 + 1;
	//m_gridHeight = (DRAWING_MANAGER.GetScreenHeight() - m_gridStartY * 2) / 75 + 1;

	//m_gridWidth = 10;
	//m_gridHeight = 10;

	this->m_connectionImages = new Image[10];
	char imageName[256];
	for(int i=0; i < 10; i++)
	{


		sprintf_s(imageName, ASSET_PATH  "Connection Images/ConnectionImage%d.png", i + 1);
		m_connectionImages[i].LoadImage(imageName);

	}

	m_lockedImages = new Image[10];
	for(int i=0; i < 10; i++)
	{

		sprintf_s(imageName, ASSET_PATH "Locked Images/LockedImage%d.png", i+1);
		m_lockedImages[i].LoadImage(imageName);

	}

	m_blastArea.Allocate(m_gridWidth * m_gridHeight);
	m_blastSquares.Allocate(m_gridWidth * m_gridHeight);
	m_balls = new Ball *[m_gridWidth * m_gridHeight];
	memset(m_balls, 0, sizeof(Ball *)*(m_gridWidth * m_gridHeight));

}

void Game::ClearGameBoard()
{

	m_blastArea.Clear(false);
	m_gameOver = false;
	m_detonating = false;
	m_finalScore = 0;
	m_nMoves = 0;
	m_totalTime = 0.0;

	int i=0;
	for(; i < m_gridHeight * m_gridWidth; i++)
	{

		if (m_balls[i])
		{

			if (!m_blastArea.FindItem(m_balls[i]))
			{

				m_blastArea.AddObject(m_balls[i]);

			}

		}

	}

	m_creatingNewGameBoard = true;

}

#define min(x, y) ( ((x) < (y)) ? (x) : (y) )

void Game::CreateBalls()
{

	ClearGameBoard();

	int i, j;
	int maxRed = m_gridHeight;

	int nWhite=0, nBlack=0;

	int *connections = new int[m_gridWidth * m_gridHeight];
	for(int connection=0; connection < m_gridWidth * m_gridHeight; connection++)
	{

		connections[connection] = -1;

	}

	for(int connection=0; connection < min(m_gridWidth / 3, 10); connection++)
	{

		int n=0;

		while (n < 3)
		{

			for(i=0; i < m_gridHeight && n < 3; i++)
			{

				for(j=0; j < m_gridWidth && n < 3; j++)
				{

					if (connections[j + i*m_gridWidth] == -1)
					{

						if (rand() % (m_gridWidth * m_gridHeight) == 0)
						{

							connections[j + i*m_gridWidth] = connection;
							n++;

						}

					}

				}

			}

		}

	}

	for(i=0; i < m_gridHeight; i++)
	{

		for(j=0; j < m_gridWidth; j++)
		{

			Ball *ball = m_world.AddBall();
			ball->m_game = this;
			ball->m_blackBall = &m_blackBall;
			ball->m_whiteBall = &m_whiteBall;
			ball->m_redBall = &m_redBall;
			ball->m_shadow = &m_shadow;
			ball->m_selectionAnimation = m_selectionAnimation;
			ball->m_hoverAnimation = m_hoverAnimation;
			ball->m_explosionAnimation = m_explosion;
			ball->m_connectionImages = m_connectionImages;
			ball->m_lockedImages = m_lockedImages;
			//ball->m_location = Vector2(75*(j) + m_gridStartX, 75*(i) + m_gridStartY);

			int flop = (i % 2) ? (m_gridWidth - j) : j;
			ball->m_location = Vector2(75*(j) + m_gridStartX, -100 - (m_gridHeight - i - 1)*(600) - rand() % 400);
			ball->m_radius = 35;

			int color = rand()%101;

			if (color <= 50) { ball->m_color = Ball::BLACK; nBlack++; }
			else if (color <= 100) { ball->m_color = Ball::WHITE; nWhite++; }

			ball->m_row = i;
			ball->m_column = j;

			ball->m_detonationConnection = connections[j + i*m_gridWidth];

			m_balls[j + i*m_gridWidth] = ball;

		}

	}

	for(j=0; j < m_gridWidth; j++)
	{

		for(i=0; i < 2; i++)
		{

			int row = rand() % (m_gridHeight);
			Ball *ball = m_balls[j + m_gridWidth*row];

			int color = rand()%2;

			if (color && ball->m_detonationConnection == -1)
			{

				if (ball->m_color == Ball::BLACK) nBlack--;
				else if (ball->m_color == Ball::WHITE) nWhite--;

				ball->m_color = Ball::RED;

			}


		}

	}

	//if (nWhite % 2)
	//{

	//	i=0;
	//	while(m_balls[i++]->m_color == Ball::BLACK && i < m_gridWidth * m_gridHeight);
	//	m_balls[i]->m_color = Ball::BLACK;

	//}

	while(EliminateSquare(&nBlack, &nWhite));

	m_possibleScore = nBlack + nWhite;
	if (nWhite % 2)
	{

		m_possibleScore -= 2;

	}

}

int Game::GetScore()
{

	double scorePerObject = pow(m_scorePerObject * (1 - (m_possibleScore / (m_gridHeight * m_gridWidth))), 2);

	return Round(scorePerObject * (m_finalScore) * (2 * m_finalScore - 1) / 6.0);

}

bool Game::EliminateSquare(int *nBlack, int *nWhite)
{

	int i=0;

	for(; i < m_gridHeight * m_gridWidth; i++)
	{

		if (!m_balls[i]) continue;

		Ball *leftTop = m_balls[i];
		Ball *rightTop = GetBall(m_balls[i]->m_row, m_balls[i]->m_column+1);
		Ball *rightBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column+1);
		Ball *leftBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column);
		if (!leftBottom || !rightBottom || !rightTop) continue;

		if (leftTop->m_color == rightTop->m_color)
		{

			if (leftTop->m_color == rightBottom->m_color)
			{

				if (leftTop->m_color == leftBottom->m_color)
				{


					if (leftTop->m_color == Ball::BLACK) (*nBlack)--;
					else if (leftTop->m_color == Ball::WHITE) (*nWhite)--;

					leftTop->m_color = (leftTop->m_color == Ball::BLACK) ? Ball::WHITE : Ball::BLACK;

					return true;

				}

			}

		}

	}

	return false;

}

Ball *Game::GetBall(int row, int column)
{

	if (row >= m_gridHeight || row < 0) return 0;
	if (column >= m_gridWidth || column < 0) return 0;

	return m_balls[row * m_gridWidth + column];

}

bool Game::DetonateSquare()
{

	GenerateBlastSquares();
	RefineBlastSquares();

	return (m_blastArea.m_nObjects > 0);

}

bool Game::IsSquare(Ball *ball, BlastSquare *square)
{

	Ball *rightTop = GetBall(ball->m_row, ball->m_column + 1);
	Ball *rightBottom = GetBall(ball->m_row + 1, ball->m_column + 1);
	Ball *leftBottom = GetBall(ball->m_row + 1, ball->m_column);

	if (!rightTop || !rightBottom || !leftBottom) return false;

	if (rightTop->m_color != ball->m_color) return false;
	if (rightBottom->m_color != ball->m_color) return false;
	if (leftBottom->m_color != ball->m_color) return false;

	square->Reference = ball;
	square->TopRight = rightTop;
	square->BottomRight = rightBottom;
	square->BottomLeft = leftBottom;

	return true;

}

void Game::GenerateBlastSquares()
{

	this->m_blastSquares.Clear(true);

	int i=0;
	int arrayLength = m_gridHeight * m_gridWidth;
	for(; i < arrayLength; i++)
	{

		if (!m_balls[i]) continue;

		BlastSquare placeholder;

		if (IsSquare(m_balls[i], &placeholder))
		{

			BlastSquare *newSquare = m_blastSquares.AddObject();
			*newSquare = placeholder;

		}

	}

}

void Game::RefineBlastSquares()
{

	m_blastArea.Clear(false);
	for(int i=0; i < m_blastSquares.m_nObjects; i++)
	{

		BlastSquare *square = m_blastSquares.m_array[i];

		for(int cell=0; cell < 4; cell++)
		{

			if (!InBlast(square->Balls[cell]))
			{

				m_blastArea.AddObject(square->Balls[cell]);

				if (square->Balls[cell]->m_detonationConnection != -1)
				{

					for(int connectionBall=0; connectionBall < m_gridWidth * m_gridHeight; connectionBall++)
					{

						if (m_balls[connectionBall] && m_balls[connectionBall]->m_detonationConnection == square->Balls[cell]->m_detonationConnection)
						{

							if (!InBlast(m_balls[connectionBall]))
							{

								m_blastArea.AddObject(m_balls[connectionBall]);

							}

						}

					}

				}

			}

		}

	}

}

int Game::CommonCells(BlastSquare *square)
{

	int count=0;
	for(int i=0; i < m_blastSquares.m_nObjects; i++)
	{

		count += m_blastSquares.m_array[i]->CommonSquares(square);

	}

	return count;

}

bool Game::ExpandBlast()
{

	int i=0;
	for(; i < m_gridHeight * m_gridWidth; i++)
	{

		if (!m_balls[i]) continue;
		if (!InBlast(m_balls[i])) continue;

		//m_blastArea.AddObject(m_balls[i]); 18,

		//Ball *leftTop = m_balls[i];
		//Ball *rightTop = GetBall(m_balls[i]->m_row, m_balls[i]->m_column+1);
		//Ball *rightBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column+1);
		//Ball *leftBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column);
		////if (!leftBottom || !rightBottom || !rightTop) continue;

		//Ball::COLOR REF = Ball::BLACK;
		//if (leftTop && m_blastArea.FindItem(leftTop)) REF = leftTop->m_color;
		//if (rightTop && m_blastArea.FindItem(rightTop)) REF = rightTop->m_color;
		//if (rightBottom && m_blastArea.FindItem(rightBottom)) REF = rightBottom->m_color;
		//if (leftBottom && m_blastArea.FindItem(leftBottom)) REF = leftBottom->m_color;

		//Ball *in[4];
		//in[0] = leftTop ? (m_blastArea.FindItem(leftTop) ? 0 : leftTop) : 0;
		//in[1] = rightTop ? (m_blastArea.FindItem(rightTop) ? 0 : rightTop) : 0;
		//in[2] = rightBottom ? (m_blastArea.FindItem(rightBottom) ? 0 : rightBottom) : 0;
		//in[3] = leftBottom ? (m_blastArea.FindItem(leftBottom) ? 0 : leftBottom) : 0;

		//int n=0, ii=0;
		//for(; ii < 4; ii++) if (!in[ii]) n++;

		Ball *OUT[4];

		OUT[0] = m_balls[i];
		OUT[1] = GetBall(m_balls[i]->m_row, m_balls[i]->m_column+1);
		OUT[2] = GetBall(m_balls[i]->m_row+1, m_balls[i]->m_column+1);
		OUT[3] = GetBall(m_balls[i]->m_row+1, m_balls[i]->m_column);

		bool invalid=false;

		int n=0;
		int ii=0;
		for(; ii < 4; ii++)
		{

			if (!OUT[ii]) invalid=true;

			if (OUT[ii]->m_color != m_balls[i]->m_color) invalid=true;

			if (InBlast(OUT[ii]))
			{

				OUT[ii] = 0;
				n++;

			}

		}

		if (invalid) continue;

		bool added=false;

		if (n > 1 && n < 4)
		{

			for(ii=0; ii<4; ii++)
			{

				if (OUT[ii]) 
				{

					m_blastArea.AddObject(OUT[ii]);
					added=true;

				}

			}

		}

		if (added) return added;

		//bool added=false;

		//if (connected)
		//{
		//	for(ii=0; ii < 4; ii++)
		//	{

		//		if (OUT[ii])
		//		{

		//			if (OUT[ii]->m_color == REF)
		//			{

		//				m_blastArea.AddObject(OUT[ii]);
		//				added=true;

		//			}

		//		}

		//	}

		//}

		//if (added) return added;

		//Ball *ball = m_balls[i];

		//int row=m_balls[i]->m_row;
		//int column=m_balls[i]->m_column;

		//int colExt=LONG_MAX;
		//int rowExt=LONG_MIN;

		//bool foundAny=false;

		//// Pass 1 - Determing extents

		//for(row = m_balls[i]->m_row; row < GAME_BOARD_HEIGHT; row++)
		//{

		//	for(column =m_balls[i]->m_column; column < GAME_BOARD_WIDTH; column++)
		//	{

		//		if (!GetBall(row, column) || GetBall(row, column)->m_black != m_balls[i]->m_black) 
		//		{

		//			colExt = imin(colExt, column-1);

		//			foundAny = true;					

		//		}


		//	}

		//}

		//rowExt = imin(rowExt, row-1);

		//std::cout << colExt << " " << rowExt << "\n";


		//if (foundAny && rowExt > ball->m_row && colExt > ball->m_column)
		//{

		//	// Pass 2 - Detonation
		//	for(row =ball->m_row; row <= rowExt; row++)
		//	{

		//		for(column = ball->m_column; column <= colExt; column++)
		//		{

		//			DetonateBall(GetBall(row, column));

		//		}

		//	}

		//	return true;

		//}

	}

	return false;

}

void Game::DetonateAllSquares()
{

	DetonateSquare();

}

Vector2 Game::GetNominal(int row, int column)
{

	return Vector2(75 * column + m_gridStartX, 75 * row + m_gridStartY);

}

void Game::Process()
{

	if (IsSettled() && !IsExploding() && !m_blastArea.m_nObjects && !m_creatingNewGameBoard) 
	{

		if (!DetonateSquare() && m_detonating)
		{

			m_gameOver = true;
			m_detonating = false;

		}

	}

	if (!IsExploding() && m_blastArea.m_nObjects && !m_creatingNewGameBoard) 
	{

		int i=0;
		for(; i < m_blastArea.m_nObjects; i++)
		{

			m_blastArea.m_array[i]->m_exploding = true;

		}

		if (!m_gameOver) m_detonating = true;

	}

	if (!IsExploding() && m_blastArea.m_nObjects && m_creatingNewGameBoard)
	{
		
		int i=0;
		for(; i < m_blastArea.m_nObjects; i++)
		{

			m_blastArea.m_array[i]->m_exploding = true;

		}

	}

	if (m_creatingNewGameBoard && !m_blastArea.m_nObjects)
	{

		CreateBalls();
		m_creatingNewGameBoard = false;

	}

	if (m_blastArea.m_nObjects && m_blastArea.m_array[m_nextDetonate]->IsExploded())
	{

		if (!m_creatingNewGameBoard)
		{

			DetonateBall(m_blastArea.m_array[m_nextDetonate]);

		}

		else
		{

			int i=0;
			for(; i < m_blastArea.m_nObjects; i++)
			{

				DetonateBall(m_blastArea.m_array[m_nextDetonate]);

			}

		}

	}

	m_world.ProcessInput();
	m_world.Process();

	DRAWING_MANAGER.DrawImage(&m_sidePanel, Vector2(DRAWING_MANAGER.GetScreenWidth(), 0), TOP_RIGHT);

	int border = 5;

	SDL_Rect rect;
	rect.x = m_gridStartX  - 90 / 2;
	rect.y = m_gridStartY - 90 / 2;
	rect.w = m_gridWidth * 75 + 15;
	rect.h = m_gridHeight * 75 + 15;

	//SDL_FillRect(DRAWING_MANAGER.GetScreen(), &rect, SDL_MapRGBA(DRAWING_MANAGER.GetScreen()->format, 2, 122, 187, 255));

	rect.x += border;
	rect.y += border;
	rect.h -= border*2;
	rect.w -= border*2;

	//SDL_FillRect(DRAWING_MANAGER.GetScreen(), &rect, SDL_MapRGBA(DRAWING_MANAGER.GetScreen()->format, 50, 50, 50, 255));

	m_world.Render();

	if (!m_gameOver && !m_creatingNewGameBoard)
	{

		m_totalTime += TIMER.GetFrameDuration();

	}

}

bool Game::IsSettled()
{

	int i=0;

	for(; i < m_gridHeight * m_gridWidth; i++)
	{

		if (m_balls[i])
		{

			if (!m_balls[i]->IsSettled()) 
				return false;

		}

	}

	return true;

}

bool Game::IsExploding()
{

	int i=0;

	for(; i < m_gridHeight * m_gridWidth; i++)
	{

		if (m_balls[i])
		{

			if (!m_balls[i]->IsExploded()) 
				return true;

		}

	}

	return false;

}