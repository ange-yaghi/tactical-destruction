#include <game.h>

#include <input_manager.h>
#include <drawing_manager.h>
#include <timing.h>
#include <math.h>
#include <falling_animation.h>
#include <keyframe_animation.h>

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

	return 0;
}

Game::Game() {
	m_gridStartX = 60;
	m_gridStartY = 60;

	m_ball1 = m_ball2 = nullptr;

	m_blastArea.SetSortMethod(&BallSortCompare);
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

Game::~Game() {}

void Game::OnBallSelected(Ball *ball) {
	if (m_ball1 == nullptr && ball->m_color != Ball::RED) {
		m_ball1 = ball;
		return;
	}
	else if (m_ball1 != nullptr && m_ball1->m_systemID == ball->m_systemID) {
		m_ball1 = nullptr;
		return;
	}

	if (m_ball2 == nullptr) {
		if (ball->m_color != Ball::RED && ball->IsAdjacent(*m_ball1)) {
			m_ball2 = ball;

			/*
			Ball::COLOR temp = m_ball1->m_color;
			int tempConnection = m_ball1->m_detonationConnection;

			m_ball1->m_color = m_ball2->m_color;
			m_ball2->m_color = temp;

			m_ball1->m_detonationConnection = m_ball2->m_detonationConnection;
			m_ball2->m_detonationConnection = tempConnection;
			*/
			KeyframeAnimation *animation1 = m_ball1->AddAnimationController<KeyframeAnimation>();
			Keyframe *key1 = animation1->AddKeyframe(0.0f);
			key1->AddFlag(Keyframe::LOCATION_KEY);
			key1->SetPosition(m_ball1->m_location);

			Keyframe *key2 = animation1->AddKeyframe(0.25f);
			key2->AddFlag(Keyframe::LOCATION_KEY);
			key2->SetPosition(m_ball2->m_location);

			KeyframeAnimation *animation2 = m_ball2->AddAnimationController<KeyframeAnimation>();
			Keyframe *key1b = animation2->AddKeyframe(0.0f);
			key1b->AddFlag(Keyframe::LOCATION_KEY);
			key1b->SetPosition(m_ball2->m_location);

			Keyframe *key2b = animation2->AddKeyframe(0.25f);
			key2b->AddFlag(Keyframe::LOCATION_KEY);
			key2b->SetPosition(m_ball1->m_location);

			int tempRow = m_ball1->m_row;
			int tempColumn = m_ball1->m_column;
			m_ball1->m_row = m_ball2->m_row;
			m_ball1->m_column = m_ball2->m_column;
			
			m_ball2->m_row = tempRow;
			m_ball2->m_column = tempColumn;

			m_balls[m_ball1->m_row * m_gridWidth + m_ball1->m_column] = m_ball1;
			m_balls[m_ball2->m_row * m_gridWidth + m_ball2->m_column] = m_ball2;

			m_ball2 = nullptr;
			m_ball1 = nullptr;

			m_nMoves++;
		}

		return;
	}
	else if (m_ball2->m_systemID == ball->m_systemID) {
		m_ball2 = nullptr;
		return;
	}
}

bool Game::IsBallSelected(int ID) const {
	return (
		(m_ball2 != nullptr && m_ball2->m_systemID == ID) || 
		(m_ball1 != nullptr && m_ball1->m_systemID == ID));
}

bool Game::IsValidHover(const Ball *ball) const {
	if (m_creatingNewGameBoard || !IsSettled() || IsExploding() || ball->m_color == Ball::RED) return false;
	if (m_ball1 == nullptr) return true;

	return m_ball1->IsAdjacent(*ball);
}

void Game::DeleteBall(Ball *ball) {
	int x, y;

	x = ball->m_column;
	y = ball->m_row;

	m_blastArea.DeleteObject(ball, false);
	//m_balls[y * m_gridWidth + x] = nullptr;
	m_world.DeleteObject(ball);

	if (!m_gameOver && !m_creatingNewGameBoard) {
		m_finalScore++;
	}
}

void Game::MoveBallDown(Ball *ball) {
	if (ball == nullptr) return;

	m_balls[(ball->m_row + 1) * m_gridWidth + ball->m_column] = m_balls[ball->m_row * m_gridWidth + ball->m_column];
	m_balls[ball->m_row * m_gridWidth + ball->m_column] = nullptr;

	ball->m_row++;
	ball->m_falling = true;
}

void Game::LoadImages() {
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

	this->m_connectionImages = new Image[10];
	char imageName[256];
	for (int i = 0; i < 10; i++) {
		sprintf_s(imageName, ASSET_PATH  "Connection Images/ConnectionImage%d.png", i + 1);
		m_connectionImages[i].LoadImage(imageName);
	}

	m_lockedImages = new Image[10];
	for (int i = 0; i < 10; i++) {
		sprintf_s(imageName, ASSET_PATH "Locked Images/LockedImage%d.png", i+1);
		m_lockedImages[i].LoadImage(imageName);
	}

	m_blastArea.Allocate(m_gridWidth * m_gridHeight);
	m_blastSquares.Allocate(m_gridWidth * m_gridHeight);
	m_balls = new Ball *[m_gridWidth * m_gridHeight];
	memset(m_balls, 0, sizeof(Ball *) * (m_gridWidth * m_gridHeight));
}

void Game::ClearGameBoard() {
	m_blastArea.Clear(false);
	m_gameOver = false;
	m_detonating = false;
	m_finalScore = 0;
	m_nMoves = 0;
	m_totalTime = 0.0;

	for (int i = 0; i < m_gridHeight * m_gridWidth; i++) {
		if (m_balls[i] != nullptr) {
			if (m_blastArea.FindItem(m_balls[i]) == nullptr) {
				m_blastArea.AddObject(m_balls[i]);
			}
		}
	}

	m_creatingNewGameBoard = true;
}

#define min(x, y) ( ((x) < (y)) ? (x) : (y) )

void Game::CreateBalls() {
	ClearGameBoard();

	int maxRed = m_gridHeight;
	int nWhite=0, nBlack=0;

	int *connections = new int[m_gridWidth * m_gridHeight];
	for (int connection = 0; connection < m_gridWidth * m_gridHeight; connection++) {
		connections[connection] = -1;
	}

	for (int connection = 0; connection < min(m_gridWidth / 3, 10); connection++) {
		int n=0;
		while (n < 3) {
			for (int i=0; i < m_gridHeight && n < 3; i++) {
				for (int j=0; j < m_gridWidth && n < 3; j++) {
					if (connections[j + i*m_gridWidth] == -1) {
						if (rand() % (m_gridWidth * m_gridHeight) == 0) {
							connections[j + i*m_gridWidth] = connection;
							n++;
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < m_gridHeight; i++) {
		for (int j = 0; j < m_gridWidth; j++) {
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

			ball->m_location = Vector2(75 * j + m_gridStartX, -100 - (m_gridHeight - i - 1) * 600 - rand() % 400);
			ball->m_radius = 35;

			int u = rand() % 2;
			if (u == 0) { ball->m_color = Ball::BLACK; nBlack++; }
			else if (u == 1) { ball->m_color = Ball::WHITE; nWhite++; }

			ball->m_row = i;
			ball->m_column = j;

			ball->m_detonationConnection = connections[j + i * m_gridWidth];

			m_balls[j + i * m_gridWidth] = ball;

			// Add falling animation
			FallingAnimation *animation = ball->AddAnimationController<FallingAnimation>();
			animation->SetTargetY((float)GetNominal(ball->m_row, ball->m_column).y);
		}
	}

	for (int j = 0; j < m_gridWidth; j++) {
		for (int i = 0; i < 2; i++) {
			int row = rand() % m_gridHeight;
			Ball *ball = m_balls[j + m_gridWidth * row];

			int color = rand() % 2;
			if (color != 0 && ball->m_detonationConnection == -1) {
				if (ball->m_color == Ball::BLACK) nBlack--;
				else if (ball->m_color == Ball::WHITE) nWhite--;

				ball->m_color = Ball::RED;
			}
		}
	}

	while (EliminateSquare(&nBlack, &nWhite));

	m_possibleScore = nBlack + nWhite;
	if (nWhite % 2 == 1) {
		m_possibleScore -= 2;
	}
}

int Game::GetScore() const {
	double scorePerObject = pow(m_scorePerObject * (1 - (m_possibleScore / (m_gridHeight * m_gridWidth))), 2);
	return Round(scorePerObject * (m_finalScore) * (2 * m_finalScore - 1) / 6.0);
}

bool Game::EliminateSquare(int *nBlack, int *nWhite) {
	for (int i = 0; i < m_gridHeight * m_gridWidth; i++) {
		if (m_balls[i] == nullptr) continue;

		Ball *leftTop = m_balls[i];
		Ball *rightTop = GetBall(m_balls[i]->m_row, m_balls[i]->m_column+1);
		Ball *rightBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column+1);
		Ball *leftBottom = GetBall(m_balls[i]->m_row + 1, m_balls[i]->m_column);
		if (leftBottom == nullptr 
			|| rightBottom == nullptr
			|| rightTop == nullptr) continue;

		if (leftTop->m_color == rightTop->m_color) {
			if (leftTop->m_color == rightBottom->m_color) {
				if (leftTop->m_color == leftBottom->m_color) {
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

Ball *Game::GetBall(int row, int column) const {
	if (row >= m_gridHeight || row < 0) return nullptr;
	if (column >= m_gridWidth || column < 0) return nullptr;

	return m_balls[row * m_gridWidth + column];
}

bool Game::DetonateSquare() {
	GenerateBlastSquares();
	RefineBlastSquares();

	return (m_blastArea.m_nObjects > 0);
}

bool Game::IsSquare(Ball *ball, BlastSquare *square) const {
	Ball *rightTop = GetBall(ball->m_row, ball->m_column + 1);
	Ball *rightBottom = GetBall(ball->m_row + 1, ball->m_column + 1);
	Ball *leftBottom = GetBall(ball->m_row + 1, ball->m_column);

	if (rightTop == nullptr
		|| rightBottom == nullptr 
		|| leftBottom == nullptr) return false;

	if (rightTop->m_color != ball->m_color) return false;
	if (rightBottom->m_color != ball->m_color) return false;
	if (leftBottom->m_color != ball->m_color) return false;

	square->Reference = ball;
	square->TopRight = rightTop;
	square->BottomRight = rightBottom;
	square->BottomLeft = leftBottom;

	return true;
}

void Game::GenerateBlastSquares() {
	this->m_blastSquares.Clear(true);

	int arrayLength = m_gridHeight * m_gridWidth;
	for (int i = 0; i < arrayLength; i++) {
		if (m_balls[i] == nullptr) continue;

		BlastSquare newSquare;
		if (IsSquare(m_balls[i], &newSquare)) {
			*m_blastSquares.AddObject() = newSquare;
		}
	}
}

void Game::RefineBlastSquares() {
	m_blastArea.Clear(false);
	for (int i = 0; i < m_blastSquares.m_nObjects; i++) {
		BlastSquare *square = m_blastSquares.m_array[i];

		for (int cell = 0; cell < 4; cell++) {
			if (!InBlast(square->Balls[cell])) {
				m_blastArea.AddObject(square->Balls[cell]);

				if (square->Balls[cell]->m_detonationConnection != -1) {
					for (int connectionBall=0; connectionBall < m_gridWidth * m_gridHeight; connectionBall++) {
						if (m_balls[connectionBall] != nullptr
							&& m_balls[connectionBall]->m_detonationConnection == square->Balls[cell]->m_detonationConnection
							&& !InBlast(m_balls[connectionBall])) {

							m_blastArea.AddObject(m_balls[connectionBall]);
						}
					}
				}
			}
		}
	}
}

int Game::CommonCells(const BlastSquare *square) const {
	int count = 0;
	for (int i = 0; i < m_blastSquares.m_nObjects; i++) {
		count += m_blastSquares.m_array[i]->CommonSquares(square);
	}

	return count;
}

void Game::DetonateAllSquares() {
	DetonateSquare();
}

Vector2 Game::GetNominal(int row, int column) const {
	return Vector2(75 * column + m_gridStartX, 75 * row + m_gridStartY);
}

void Game::Process() {
	if (IsSettled() && !IsExploding() && m_blastArea.m_nObjects == 0 && !m_creatingNewGameBoard) {
		if (!DetonateSquare() && m_detonating) {
			m_gameOver = true;
			m_detonating = false;
		}
	}

	if (!IsExploding() && m_blastArea.m_nObjects > 0 && !m_creatingNewGameBoard) {
		for (int i = 0; i < m_blastArea.m_nObjects; i++) {
			Ball *ball = m_blastArea.m_array[i];
			ball->m_exploding = true;

			m_balls[ball->m_row * m_gridWidth + ball->m_column] = nullptr;

			// Move the whole column down
			int y = ball->m_row;
			for (int i = y - 1; i >= 0; i--) {
				MoveBallDown(GetBall(i, ball->m_column));
			}
		}

		float *impulses = new float[m_gridWidth];
		for (int i = 0; i < m_gridWidth; i++) {
			impulses[i] = 1.0f;
		}

		for (int i = m_gridHeight - 1; i >= 0; i--) {
			for (int j = 0; j < m_gridWidth; j++) {
				if (impulses[j] > 0.0f) {
					impulses[j] = -100.0f - (float)(rand() % 20);
				}
				else {
					impulses[j] -= (float)(rand() % 5);
				}

				Ball *ball = m_balls[i * m_gridWidth + j];
				if (ball != nullptr && ball->m_falling) {
					FallingAnimation *animation = ball->AddAnimationController<FallingAnimation>();
					animation->SetStartingImpulse(Vector2(0.0f, impulses[j]));
					animation->SetTargetY((float)GetNominal(ball->m_row, ball->m_column).y);

					// Reset the flag
					ball->m_falling = false;
				}
			}
		}

		delete[] impulses;

		if (!m_gameOver) m_detonating = true;
	}

	if (!IsExploding() && m_blastArea.m_nObjects > 0 && m_creatingNewGameBoard) {
		for (int i = 0; i < m_blastArea.m_nObjects; i++) {
			m_blastArea.m_array[i]->m_exploding = true;
		}

		for (int i = 0; i < m_gridWidth * m_gridHeight; i++) {
			m_balls[i] = nullptr;
		}
	}

	if (m_creatingNewGameBoard && m_blastArea.m_nObjects == 0) {
		CreateBalls();
		m_creatingNewGameBoard = false;
	}

	if (m_blastArea.m_nObjects > 0 && m_blastArea.m_array[m_nextDetonate]->IsExploded()) {
		int blastAreaCount = m_blastArea.m_nObjects;
		for (int i = 0; i < blastAreaCount; i++) {
			DeleteBall(m_blastArea.m_array[m_nextDetonate]);
		}

		int a = 0;
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
	rect.h -= border * 2;
	rect.w -= border * 2;

	//SDL_FillRect(DRAWING_MANAGER.GetScreen(), &rect, SDL_MapRGBA(DRAWING_MANAGER.GetScreen()->format, 50, 50, 50, 255));

	m_world.Render();

	if (!m_gameOver && !m_creatingNewGameBoard) {
		m_totalTime += TIMER.GetFrameDuration();
	}
}

bool Game::IsSettled() const {
	for (int i = 0; i < m_gridHeight * m_gridWidth; i++) {
		if (m_balls[i] != nullptr && !m_balls[i]->IsSettled()) {
			return false;
		}
	}

	return true;
}

bool Game::IsExploding() const {
	for (int i = 0; i < m_gridHeight * m_gridWidth; i++) {
		if (m_balls[i] != nullptr && m_balls[i]->IsExploding()) {
			return true;
		}
	}

	for (int i = 0; i < m_blastArea.m_nObjects; i++) {
		if (m_blastArea.m_array[i]->IsExploding()) {
			return true;
		}
	}

	return false;
}
