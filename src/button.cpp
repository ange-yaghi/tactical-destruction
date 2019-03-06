#include <button.h>

#include <input_manager.h>
#include <drawing_manager.h>

Button::Button() : GameObject(GUI_OBJECT) {}

void Button::Register() {
	INPUT_MANAGER.Register(this);
}

bool Button::OnMouseClick(const Vector2 &location) {
	ProcessInput();

	if (m_state == S_PRESSED) {
		m_gui->OnPressedButton(m_buttonID);
		return true;
	}

	return false;
}

void Button::ProcessInput() {
	m_state = S_NORMAL;

	Vector2 mousePos = INPUT_MANAGER.GetMousePos();

	if (mousePos.x >= m_location.x && mousePos.x <= m_location.x + m_normalImage->GetWidth()) {
		if (mousePos.y >= m_location.y && mousePos.y <= m_location.y + m_normalImage->GetHeight()) {
			m_state = S_HOVER;

			if (INPUT_MANAGER.IsMousePressed()) {
				m_state = S_PRESSED;
			}
		}
	}
}

void Button::Process() {}

void Button::Render() {
	if (m_state == S_NORMAL) {
		DRAWING_MANAGER.DrawImage(m_normalImage, m_location);
	}
	else if (m_state == S_HOVER) {
		DRAWING_MANAGER.DrawImage(m_hoverImage, m_location);
	}
	else if (m_state == S_PRESSED) {
		DRAWING_MANAGER.DrawImage(m_normalImage, m_location);
	}
}
