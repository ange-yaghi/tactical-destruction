#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <assert.h>

#define Round(x) (int)(0.5 + (x))
inline int imax(int a, int b) { return (a > b) ? a : b; }
inline int imin(int a, int b) { return (a < b) ? a : b; }

inline double fmax(double a, double b) { return (a > b) ? a : b; }
inline double fmin(double a, double b) { return (a < b) ? a : b; }

class Image {
public:
	Image();
	Image(const char *fname);
	~Image() { Free(); }

	void LoadImage(const char *fname);
	void Initialize(SDL_Renderer *renderer);
	void Free();

	inline int GetHeight() { return m_height; }
	inline int GetWidth() { return m_width; }

	bool IsValid() { return m_surface != NULL; }

	SDL_Texture *GetTexture() const { return m_texture; }
	SDL_Surface *GetSurface() const { return m_surface; }

protected:
	const char *m_fname;

	int m_width;
	int m_height;

	SDL_Texture *m_texture;
	SDL_Surface *m_surface;
};

#endif /* IMAGE_H */
