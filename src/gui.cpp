#include <gui.h>

#include <input_manager.h>
#include <drawing_manager.h>
#include <game.h>
#include <button.h>
#include <simple_label.h>

Image GUI::m_symbols;

void GUI::CreateButtons() {
	m_newGameNormal.LoadImage(ASSET_PATH "GUI/NewGameNormal.png");
	m_newGameHovered.LoadImage(ASSET_PATH "GUI/NewGameHovered.png");
	m_newGamePressed.LoadImage(ASSET_PATH "GUI/NewGamePressed.png");

	m_closeNormal.LoadImage(ASSET_PATH "GUI/CloseNormal.png");
	m_closeHovered.LoadImage(ASSET_PATH "GUI/CloseHovered.png");
	m_closePressed.LoadImage(ASSET_PATH "GUI/ClosePressed.png");

	m_gameOver.LoadImage(ASSET_PATH "GUI/GameOver.png");
	m_detonation.LoadImage(ASSET_PATH "GUI/Detonation.png");

	m_symbols.LoadImage(ASSET_PATH "GUI/Text.png");

	Button *newGame = m_game->m_world.AddButton();
	newGame->m_gui = this;
	newGame->m_buttonID = 0;
	newGame->m_location = Vector2(DRAWING_MANAGER.GetScreenWidth() - 380, 434);
	newGame->m_normalImage = &m_newGameNormal;
	newGame->m_hoverImage = &m_newGameHovered;
	newGame->m_pressedImage = &m_newGamePressed;

	Button *close = m_game->m_world.AddButton();
	close->m_gui = this;
	close->m_buttonID = 1;
	close->m_location = Vector2(DRAWING_MANAGER.GetScreenWidth() - 195, 434);
	close->m_normalImage = &m_closeNormal;
	close->m_hoverImage = &m_closeHovered;
	close->m_pressedImage = &m_closePressed;

	m_scoreLabel = m_game->m_world.AddSimpleLabel();
	m_scoreLabel->m_textSymbols = &m_symbols;
	m_scoreLabel->m_totalChars = 10;
	m_scoreLabel->m_location = Vector2(DRAWING_MANAGER.GetScreenWidth() - 380, 290);

	m_numMoves = m_game->m_world.AddSimpleLabel();
	m_numMoves->m_textSymbols = &m_symbols;
	m_numMoves->m_totalChars = 8;
	m_numMoves->m_location = Vector2(DRAWING_MANAGER.GetScreenWidth() - 308, 345);

	m_time = m_game->m_world.AddSimpleLabel();
	m_time->m_textSymbols = &m_symbols;
	m_time->m_totalChars = 8;
	m_time->m_location = Vector2(DRAWING_MANAGER.GetScreenWidth() - 308, 382);
}

void GUI::OnPressedButton(int ID) {
	if (ID == 0) m_game->ClearGameBoard();
	else if (ID == 1) m_game->m_quit = true;
}

void GUI::Process() {
	sprintf_s(m_scoreLabel->m_text, 32, "%i:%i", m_game->GetScore(), Round(m_game->m_finalScore));
	sprintf_s(m_numMoves->m_text, 32, "%i", m_game->m_nMoves);

	int totalSeconds = Round(m_game->m_totalTime);

	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds - hours * 3600) / 60;
	int seconds = totalSeconds - hours * 3600 - minutes * 60;

	sprintf_s(m_time->m_text, 32, "%.2i:%.2i:%.2i", hours, minutes, seconds);
}

void GUI::Render() {
	if (m_game->m_gameOver)	{
		DRAWING_MANAGER.DrawImage(&m_gameOver, Vector2(DRAWING_MANAGER.GetScreenWidth() - 380, 476));
	}

	if (m_game->m_detonating) {
		DRAWING_MANAGER.DrawImage(&m_detonation, Vector2(DRAWING_MANAGER.GetScreenWidth() - 380, 476));
	}
}
