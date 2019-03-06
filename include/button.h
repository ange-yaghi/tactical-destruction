#ifndef BUTTON_H
#define BUTTON_H

#include <game_object.h>

#include <gui.h> 

class Button : public GameObject {
public:
	enum BUTTON_STATE {
		S_NORMAL,
		S_HOVER,
		S_PRESSED
	};

public:
	Button();
	~Button() {}

	bool OnMouseClick(const Vector2 &location);
	void ProcessInput();
	void Process();
	void Render();

	void Register();

	BUTTON_STATE m_state;

	Image *m_normalImage;
	Image *m_hoverImage;
	Image *m_pressedImage;

	int m_buttonID;
	GUI *m_gui;
};

#endif /* BUTTON_H */
