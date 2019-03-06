#ifndef GUI_H
#define GUI_H

#include <game_object.h>

// Forward delarations
class Game;
class SimpleLabel;

class GUI : public GameObject {
public:
	GUI() : GameObject(GUI_OBJECT) {}
	~GUI() {}

	void CreateButtons();
	void OnPressedButton(int ID);

	Game *m_game;

	Image m_newGameNormal;
	Image m_newGameHovered;
	Image m_newGamePressed;

	Image m_closeNormal;
	Image m_closeHovered;
	Image m_closePressed;

	Image m_gameOver;
	Image m_detonation;

	static Image m_symbols;

	SimpleLabel *m_scoreLabel;
	SimpleLabel *m_numMoves;
	SimpleLabel *m_time;

	void Render();
	void Process();
};

#endif /* GUI_H */
