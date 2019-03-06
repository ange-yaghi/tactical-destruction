#include <drawing_manager.h>

#include <Windows.h>

DrawingManager DRAWING_MANAGER;

void DrawingManager::InitializeScreen() {
	int X = GetSystemMetrics( SM_CXSCREEN );
	int Y = GetSystemMetrics( SM_CYSCREEN );

	//SDL_Rect **rects = SDL_ListModes(NULL, SDL_DOUBLEBUF);
	//m_screen = SDL_SetVideoMode(X, Y, 32, SDL_DOUBLEBUF | SDL_FULLSCREEN);

	m_screen = SDL_CreateWindow("TempTitle",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		0, 0, SDL_WINDOW_MAXIMIZED
		/*SDL_WINDOW_FULLSCREEN_DESKTOP*/);
	m_renderer = SDL_CreateRenderer(m_screen, -1, 0);

	SDL_GL_GetDrawableSize(m_screen, &m_width, &m_height);
}

void DrawingManager::DrawImage(Image *image, Vector2 location, Vector2 offset) {
	// TODO: fix
	if (image->GetTexture() == nullptr) {
		image->Initialize(m_renderer);
		int a = 0;
	}

	SDL_Rect rect;
	rect.x = Round(location.x) - offset.x * image->GetWidth();
	rect.y = Round(location.y) - offset.y * image->GetHeight();
	rect.w = image->GetWidth();
	rect.h = image->GetHeight();
	//SDL_BlitSurface(image->GetSurface(), NULL, m_screen, &rect);
	SDL_RenderCopy(m_renderer, image->GetTexture(), nullptr, &rect);
}

void DrawingManager::DrawImageCropped(Image *image, Vector2 location, Vector2 cropLoc, Vector2 cropSize) {
	// TODO: fix
	if (image->GetTexture() == nullptr) {
		image->Initialize(m_renderer);
		int a = 0;
	}

	SDL_Rect rect;
	rect.x = Round(location.x);
	rect.y = Round(location.y);
	rect.w = cropSize.x;
	rect.h = cropSize.y;

	SDL_Rect crop;
	crop.x = cropLoc.x;
	crop.y = cropLoc.y;
	crop.w = cropSize.x;
	crop.h = cropSize.y;

	//SDL_BlitSurface(image->GetSurface(), &crop, m_screen, &rect);
	SDL_RenderCopy(m_renderer, image->GetTexture(), &crop, &rect);
}

void DrawingManager::StartFrame() {
	SDL_RenderClear(m_renderer);
	//SDL_FillRect(m_screen, NULL, m_clearColor); 
	//SDL_FillRect(m_screen, NULL, SDL_MapRGBA(DRAWING_MANAGER.GetScreen()->format, rand() % 256, rand() % 256, rand() % 256, 255));
}

void DrawingManager::EndFrame() {
	//SDL_UpdateRect(m_screen, 0, 0, 0, 0);
	//SDL_Flip(m_screen);
	SDL_RenderPresent(m_renderer);
}

void DrawingManager::SetClearColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}
