#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <assert.h>

#define Round(x) (int)(0.5 + x)
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

	SDL_Texture *GetTexture() { return m_texture; }
	SDL_Surface *GetSurface() { return m_surface; }

protected:
	const char *m_fname;

	int m_width;
	int m_height;

	SDL_Texture *m_texture;
	SDL_Surface *m_surface;
};

#define DEFAULT_FRAME_RATE 30

class Animation {
public:
	enum PLAY_MODE {
		PM_PLAY = 0x01,
		PM_REVERSE = 0x02,
		PM_LOOP = 0x04,
		PM_PING_PONG = 0x08
	};

public:
	Animation() { m_images = 0; m_nFrames = 0; m_frameRate = DEFAULT_FRAME_RATE; m_currentLocation = 0.0; m_playMode = PM_PLAY; }
	~Animation() { }

	void LoadAnimation(const char *fname, const char *ext, int start, int end);
	void SetFrameRate(double FPS);
	void SetTotalLength(double length);

	void GotoLocation(double location);
	void GotoBegin() { m_currentLocation = 0.0; }
	void GotoEnd() { m_currentLocation = m_nFrames * m_frameRate; }
	void Update();

	void Free();

	inline Image *GetCurrentImage() const { 
		assert(GetCurrentFrame() < m_nFrames);

		return &m_images[GetCurrentFrame()]; 
	}

	inline int GetCurrentFrame() const {
		return imin(imax(Round(m_currentLocation * m_frameRate), 0), m_nFrames - 1);
	}

	void SetPlayMode(unsigned int playMode) {
		m_playMode = playMode;
	}

	void AddPlayMode(PLAY_MODE mode) {
		m_playMode = m_playMode | mode;
	}

	double GetLocation() const {
		return m_currentLocation;
	}

	inline double GetEnd() const { return (m_nFrames - 1) / m_frameRate; }

protected:
	int m_nFrames;
	double m_currentLocation;
	Image *m_images;
	double m_frameRate;

	unsigned int m_playMode;

	bool m_reverse; // Used only in ping-pong mode
};

#endif /* IMAGE_H */
