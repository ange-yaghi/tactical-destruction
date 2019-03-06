#include <image.h>
#include <world.h>
#include <input_manager.h>
#include <drawing_manager.h>
#include <timing.h>
#include <game.h>

#include <SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	DRAWING_MANAGER.InitializeScreen();
	DRAWING_MANAGER.SetClearColor(0, 0, 0, 255);

	srand(TIMER.GetTime());

	Game game;
	game.LoadImages();
	game.ClearGameBoard();

	GUI *gui = game.m_world.AddGUI();
	gui->m_game = &game;
	gui->CreateButtons();

	TIMER.Initialize();
	TIMER.Update(); // Update Timing

	SDL_Event sdlEvent;
	while (!game.m_quit) {
		INPUT_MANAGER.Reset();
		INPUT_MANAGER.PollKeyboard();

		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT) {
				game.m_quit = true;
			}

			if (sdlEvent.type == SDL_KEYDOWN) {
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					game.m_quit = true;
				}
			}

			INPUT_MANAGER.ProcessEvent(sdlEvent);
		}

		TIMER.Update(); // Update Timing

		DRAWING_MANAGER.StartFrame(); 
		game.Process();
		DRAWING_MANAGER.EndFrame();
	}

	game.m_blackBall.Free();
	game.m_whiteBall.Free();

	SDL_Quit();

	return 0;
}
