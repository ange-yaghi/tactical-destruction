#include <drawing_manager.h>

#include <Windows.h>

DrawingManager DRAWING_MANAGER;

void DrawingManager::InitializeScreen() {
	int screenWidth = GetSystemMetrics( SM_CXSCREEN );
	int screenHeight = GetSystemMetrics( SM_CYSCREEN );

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
	}

	SDL_Rect rect;
	rect.x = Round(location.x - offset.x * image->GetWidth());
	rect.y = Round(location.y - offset.y * image->GetHeight());
	rect.w = Round(image->GetWidth());
	rect.h = Round(image->GetHeight());

	SDL_RenderCopy(m_renderer, image->GetTexture(), nullptr, &rect);
}

void DrawingManager::DrawImageCropped(Image *image, Vector2 location, Vector2 cropLoc, Vector2 cropSize) {
	// TODO: fix
	if (image->GetTexture() == nullptr) {
		image->Initialize(m_renderer);
	}

	SDL_Rect rect;
	rect.x = Round(location.x);
	rect.y = Round(location.y);
	rect.w = Round(cropSize.x);
	rect.h = Round(cropSize.y);

	SDL_Rect crop;
	crop.x = Round(cropLoc.x);
	crop.y = Round(cropLoc.y);
	crop.w = Round(cropSize.x);
	crop.h = Round(cropSize.y);

	SDL_RenderCopy(m_renderer, image->GetTexture(), &crop, &rect);
}

void DrawingManager::StartFrame() {
	SDL_RenderClear(m_renderer);
}

void DrawingManager::EndFrame() {
	SDL_RenderPresent(m_renderer);
}

void DrawingManager::SetClearColor(int r, int g, int b, int a) {
	SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
}
