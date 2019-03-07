#ifndef BALL_H
#define BALL_H

#include <game_object.h>
#include <gui.h>

// Forward declarations
class Game;

class Ball : public GameObject {
public:
	enum COLOR {
		BLACK,
		WHITE,
		RED
	};

public:
	Ball();
	~Ball() {}

	bool OnMouseClick(const Vector2 &location);
	bool OnRightClick(const Vector2 &location);
	bool IsAdjacent(const Ball &ball) const;
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

	int m_detonationConnection;

	COLOR m_color;

	int m_lockedColor;

	bool m_selectionState;
	bool m_hoverState;
	bool m_exploding;

	bool IsSettled() const;
	bool IsExploded() const;
	bool IsExploding() const;
};

#endif /* BALL_H */
