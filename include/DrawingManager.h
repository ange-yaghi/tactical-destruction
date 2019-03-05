#ifndef DRAWING_MANAGER_H
#define DRAWING_MANAGER_H

#include "Vector.h"
#include "Image.h"
#include <dynamic_allocator.h>

#include <SDL.h>

#define TOP_LEFT Vector2(0, 0)
#define TOP_RIGHT Vector2(1, 0)
#define BOTTOM_LEFT Vector2(0, 1)
#define BOTTOM_RIGHT Vector2(1, 1)
#define CENTERED Vector2(0.5, 0.5)

class DrawingManager
{

public:

	DrawingManager() 
	{
	
		m_screen = 0;
		ALLOCATOR.m_showOutput = false;

	}

	~DrawingManager() {}

	void InitializeScreen();

	void DrawImage(Image *image, Vector2 location, Vector2 offset=TOP_LEFT);
	void DrawImageCropped(Image *image, Vector2 location, Vector2 cropLoc, Vector2 cropSize);

	SDL_Window *GetScreen() { return m_screen; }

	void StartFrame();
	void EndFrame();

	void SetClearColor(int r, int g, int b, int a);
	//Uint32 GetClearColor() { return m_clearColor; }

	int GetScreenWidth() { return m_width; }
	int GetScreenHeight() { return m_height; }

public:

	int m_width;
	int m_height;

	DynamicAllocator<10 * MB> ALLOCATOR;

protected:

	SDL_Renderer *m_renderer;
	SDL_Window *m_screen;
	Uint32 m_clearColor;

};

extern DrawingManager DRAWING_MANAGER;

#endif