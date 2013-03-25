#include "Graphics.h";

Graphics::Graphics(SDL_Surface *screen) {
	TTF_Init();
	this->screen = screen;
	this->font = TTF_OpenFont( "lazy.ttf", 14 );;
	this->textColor = { 255, 255, 255 };

}

void Graphics::renderMotors(Quadcopter* quad) {

	//render motors
	SDL_Surface *motorscreen = TTF_RenderText_Solid( font, "Motor 1     Motor 2     Motor 3    Motor 4", textColor );
	//white placeholders
	SDL_Rect rect = {20,30, 50, 100};
	SDL_FillRect(this->screen, &rect, 0xffffff);
	rect = {100, 30, 50, 100};
	SDL_FillRect(this->screen, &rect, 0xffffff);
	rect = {180, 30, 50, 100};
	SDL_FillRect(this->screen, &rect, 0xffffff);
	rect = {260, 30, 50, 100};
	SDL_FillRect(this->screen, &rect, 0xffffff);
	//Green fill
	rect = {20,30, 50, (quad->getMotorPower(1)-1000)/10};
	SDL_FillRect(this->screen, &rect, 0x00ff00);
	rect = {100, 30, 50, (quad->getMotorPower(2)-1000)/10};
	SDL_FillRect(this->screen, &rect, 0x00ff00);
	rect = {180, 30, 50, (quad->getMotorPower(3)-1000)/10};
	SDL_FillRect(this->screen, &rect, 0x00ff00);
	rect = {260, 30, 50, (quad->getMotorPower(4)-1000)/10};
	SDL_FillRect(this->screen, &rect, 0x00ff00);

	apply_surface( 10, 10, motorscreen, this->screen ); //Update the screen
	SDL_Flip(screen);
}

void Graphics::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}
