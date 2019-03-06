#include <image.h>

#include <drawing_manager.h>
#include <timing.h>
#include <math.h>

#include <SDL_image.h>

Image::Image() {
	m_surface = nullptr;
	m_texture = nullptr;

	m_fname = nullptr;
}

Image::Image(const char *fname) {
	m_texture = nullptr;
	m_surface = nullptr;

	LoadImage(fname);
}

void Image::LoadImage(const char *image) {
	m_surface = IMG_Load(image);

	m_width = m_surface->w;
	m_height = m_surface->h;

	m_fname = image;
}

void Image::Initialize(SDL_Renderer *renderer) {
	m_texture = SDL_CreateTextureFromSurface(renderer, m_surface);

	SDL_free(m_surface);
	m_surface = nullptr;
}

void Image::Free() {
	if (m_surface != nullptr) SDL_FreeSurface(m_surface);
	m_surface = nullptr;
}
