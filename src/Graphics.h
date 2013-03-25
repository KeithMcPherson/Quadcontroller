#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "Quadcopter.h"

class Graphics {
	private:
	SDL_Surface *screen;
	TTF_Font *font;
	SDL_Color textColor;

	public:
	Graphics(SDL_Surface *screen);
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );
	void renderMotors(Quadcopter* quad);
};
