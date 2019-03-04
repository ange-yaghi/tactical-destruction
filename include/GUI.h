#ifndef GUI_H
#define GUI_H

#include "GameObject.h"

class Game;
class SimpleLabel;
class GUI : public GameObject
{

public:

	GUI() {}
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

class Button : public GameObject
{

public:

	Button();
	~Button() {}

	bool OnMouseClick(Vector2 location);
	void ProcessInput();
	void Process();
	void Render();

	void Register();

	enum BUTTON_STATE
	{

		S_NORMAL,
		S_HOVER,
		S_PRESSED,

	};

	BUTTON_STATE m_state;

	Image *m_normalImage;
	Image *m_hoverImage;
	Image *m_pressedImage;

	int m_buttonID;
	GUI *m_gui;

};

class SimpleLabel : public GameObject
{

public:

	SimpleLabel() { m_totalChars=0; m_text[0] = 0; }
	~SimpleLabel() {}

	void Render();

	char m_text[32];
	int m_totalChars;

	Image *m_textSymbols;

};

#endif