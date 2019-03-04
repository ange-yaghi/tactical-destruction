#ifndef BALL_H
#define BALL_H

#include "GameObject.h"
#include "GUI.h"

class Game;
class Ball : public GameObject
{

public:

	Ball();
	~Ball() {}

	bool OnMouseClick(Vector2 location);
	bool OnRightClick(Vector2 location);
	void ProcessInput();
	void Process();
	void Render();
	void RenderShadow();

	int m_radius;

	Game *m_game;

	Image *m_blackBall;
	Image *m_whiteBall;
	Image *m_redBall;
	Image *m_shadow;
	Image *m_lockedImages;
	Animation m_selectionAnimation;
	Animation m_hoverAnimation;
	Animation m_explosionAnimation;

	int m_numConnectionImages;
	Image *m_connectionImages;

	void OnDestroy();
	void Register();

	int m_column;
	int m_row;

	double m_speed;

	enum COLOR
	{

		BLACK,
		WHITE,
		RED

	};

	int m_detonationConnection;

	COLOR m_color;

	int m_lockedColor;

	bool m_selectionState;
	bool m_hoverState;
	bool m_exploding;

	bool IsSettled();
	bool IsExploded();


};

#endif