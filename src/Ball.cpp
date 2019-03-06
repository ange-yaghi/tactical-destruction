#include <ball.h>

#include <input_manager.h>
#include <game.h>
#include <drawing_manager.h>
#include <timing.h>

Ball::Ball() {
	m_selectionState = false;
	m_hoverState = false;
	m_color = BLACK;
	m_lockedColor = -1;

	m_exploding = false;

	m_detonationConnection = 0;
	m_numConnectionImages = 0;

	m_speed = 0;
}

bool Ball::OnMouseClick(Vector2 location) {
	Vector2 delta = location - m_location;

	if (m_lockedColor == -1 && delta.GetSquareMagnitude() <= m_radius * m_radius) {
		m_game->OnBallSelected(this);
		return true;
	}

	return false;
}

bool Ball::OnRightClick(Vector2 location) {
	Vector2 delta = location - m_location;

	if (delta.GetSquareMagnitude() <= m_radius * m_radius) {
		if (m_game->IsValidHover(this)) {
			int newLockedColor = -1;

			if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_1)) newLockedColor = 0;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_2)) newLockedColor = 1;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_3)) newLockedColor = 2;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_4)) newLockedColor = 3;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_5)) newLockedColor = 4;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_6)) newLockedColor = 5;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_7)) newLockedColor = 6;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_8)) newLockedColor = 7;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_9)) newLockedColor = 8;
			else if (INPUT_MANAGER.IsKeyDown(::SDL_SCANCODE_0)) newLockedColor = 9;

			if (m_lockedColor == -1) {
				if (newLockedColor == -1) m_lockedColor = 0;
				else m_lockedColor = newLockedColor;

			}
			else {
				if (newLockedColor == -1) m_lockedColor = -1;
				else m_lockedColor = newLockedColor;
			}

			return true;
		}
	}

	return false;
}

void Ball::ProcessInput() {
	bool hovering = false;

	Vector2 delta = INPUT_MANAGER.GetMousePos() - m_location;

	if (delta.GetSquareMagnitude() <= m_radius * m_radius) {
		if (!m_game->IsBallSelected(this->m_systemID)) {
			if (m_lockedColor == -1 && m_game->IsValidHover(this)) {
				hovering = true;
			}
		}
	}

	if (hovering && !m_hoverState) {
		m_hoverAnimation.SetPlayMode(Animation::PM_PLAY);
		m_hoverState = true;
	}

	if (!hovering && m_hoverState) {
		m_hoverAnimation.SetPlayMode(Animation::PM_REVERSE);
		m_hoverState = false;
	}
}

void Ball::Process() {
	if (!IsSettled()) {	
		Vector2 target = m_game->GetNominal(m_row, m_column);

		m_speed += TIMER.GetFrameDuration() * 500;
		m_location.y += m_speed * TIMER.GetFrameDuration();

		m_location.y = fmin(m_location.y, target.y);
	}
	else {
		m_speed = 0;
	}
}

void Ball::RenderShadow() {
	if (!m_exploding || m_explosionAnimation.GetCurrentFrame() <= 10) {
		DRAWING_MANAGER.DrawImage(m_shadow, m_location, CENTERED);
	}
}

void Ball::Render() {
	if (m_game->IsBallSelected(this->m_systemID) && !m_selectionState) {
		m_selectionState = true;
		m_selectionAnimation.SetPlayMode(Animation::PM_PLAY);
	}

	if (!m_game->IsBallSelected(this->m_systemID) && m_selectionState) {
		m_selectionState = false;
		m_selectionAnimation.SetPlayMode(Animation::PM_REVERSE);
	}

	m_selectionAnimation.Update();
	m_hoverAnimation.Update();

	if (!m_exploding || m_explosionAnimation.GetCurrentFrame() <= 10) {
		if (m_color == BLACK) DRAWING_MANAGER.DrawImage(m_blackBall, m_location, CENTERED);
		else if (m_color == WHITE) DRAWING_MANAGER.DrawImage(m_whiteBall, m_location, CENTERED);
		else if (m_color == RED) DRAWING_MANAGER.DrawImage(m_redBall, m_location, CENTERED);

		if (m_lockedColor != -1) DRAWING_MANAGER.DrawImage(&m_lockedImages[m_lockedColor], m_location, CENTERED);

		if (m_detonationConnection != -1) {
			DRAWING_MANAGER.DrawImage(&m_connectionImages[m_detonationConnection], m_location, CENTERED);
		}
	}

	if (!m_game->IsExploding() && m_game->IsSettled()) {
		DRAWING_MANAGER.DrawImage(m_selectionAnimation.GetCurrentImage(), m_location, CENTERED);
		DRAWING_MANAGER.DrawImage(m_hoverAnimation.GetCurrentImage(), m_location, CENTERED);
	}

	if (m_exploding) {
		m_explosionAnimation.SetPlayMode(Animation::PM_PLAY);
		m_explosionAnimation.Update();
		DRAWING_MANAGER.DrawImage(m_explosionAnimation.GetCurrentImage(), m_location, CENTERED);
	}
}

void Ball::OnDestroy() {
	INPUT_MANAGER.Unregister(this);
}

void Ball::Register() {
	INPUT_MANAGER.Register(this);
}

bool Ball::IsSettled() const {
	return (m_game->GetNominal(m_row, m_column) - m_location).IsZero();
}

bool Ball::IsExploded() const {
	return (!m_exploding || IsZero(m_explosionAnimation.GetLocation() - m_explosionAnimation.GetEnd()));
}

bool Ball::IsExploding() const {
	return m_exploding;
}
