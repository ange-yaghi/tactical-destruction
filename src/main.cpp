#include <SDL.h>
#include "Image.h"
#include "World.h"
#include "InputManager.h"
#include "DrawingManager.h"
#include "Timing.h"
#include <iostream>
#include "Game.h"

int main(int argc, char *argv[])
{

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return -1;

	//SDL_WM_SetCaption("Great Game", "Great Game");

	DRAWING_MANAGER.InitializeScreen();

	SDL_Event sdlEvent;
	bool quit = false;

	//Image image;
	//image.LoadImage("Assets/Ant Animation 2/Ant.tga");

	//World world;

	//Animation animation;
	//animation.LoadAnimation("Assets/Ball Animation/WhiteBall", "tga", 0, 10); // 19
	//animation.SetPlayMode(Animation::PM_PING_PONG | Animation::PM_PLAY);

	//animation.SetTotalLength(0.25);

	//PlayerObject *player = world.AddPlayer();
	//player->m_animation = animation;
	//player->m_location = Vector2(rand() % 200, rand() % 200);

	DRAWING_MANAGER.SetClearColor(0, 0, 0, 255);

	srand(TIMER.GetTime());

	Game game;
	game.LoadImages();
	//game.CreateBalls();
	game.ClearGameBoard();

	GUI *gui = game.m_world.AddGUI();
	gui->m_game = &game;
	gui->CreateButtons();

	TIMER.Initialize();
	TIMER.Update(); // Update Timing

	while (!game.m_quit)
	{

		INPUT_MANAGER.Reset();

		//do
		//{

		//	SDL_PollEvent(&sdlEvent);
		//	INPUT_MANAGER.PollKeyboard();

		//	if (sdlEvent.type == SDL_QUIT)
		//	{

		//		quit = true;

		//	}

		//	if (sdlEvent.type == SDL_KEYDOWN)
		//	{

		//		if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) quit = true;

		//	}

		//	INPUT_MANAGER.ProcessEvent(sdlEvent);

		//}

		//while (!TIMER.IsAtNextFrame(30));

		INPUT_MANAGER.PollKeyboard();

		while (SDL_PollEvent(&sdlEvent))
		{

			if (sdlEvent.type == SDL_QUIT)
			{

				game.m_quit = true;

			}

			if (sdlEvent.type == SDL_KEYDOWN)
			{

				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) game.m_quit = true;

			}

			INPUT_MANAGER.ProcessEvent(sdlEvent);

		}

		// END

		TIMER.Update(); // Update Timing

		DRAWING_MANAGER.StartFrame(); 
		//world.ProcessInput();
		//world.Process();
		//world.Render();

		game.Process();

		//animation.Update();
		//std::cout << player->m_animation.GetCurrentFrame() << "\n";
		//DRAWING_MANAGER.DrawImage(animation.GetCurrentImage(), player->m_location, CENTERED);

		DRAWING_MANAGER.EndFrame();

	}

	game.m_blackBall.Free();
	game.m_whiteBall.Free();

	SDL_Quit();

	return 0;

}