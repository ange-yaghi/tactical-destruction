#ifndef SIMPLE_LABEL_H
#define SIMPLE_LABEL_H

#include <game_object.h>

class SimpleLabel : public GameObject {
public:
	SimpleLabel() : GameObject(GUI_OBJECT) { m_totalChars = 0; m_text[0] = 0; }
	~SimpleLabel() {}

	void Render();

	char m_text[32];
	int m_totalChars;

	Image *m_textSymbols;
};

#endif /* SIMPLE_LABEL_H */
