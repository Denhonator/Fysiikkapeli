#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

bool running = 1;		
int resx = 1280;			//these three are read from conf.cfg located in game directory
int resy = 720;
bool windowed = 1;			
float areamultiplier = 1;		//zooming in and out
int xoffset = 0;			//if camera needs to be moved
int yoffset = 0;
bool coord[3841][2161] = { false };		//big array where I store if a pixel is wall or not
int speedx = 0;			//player 1 current speeds
int speedy = 0;
int speedx2 = 0;		//player 2
int speedy2 = 0;
bool boost = 0;			//if boost (for jumping higher) is available
bool boost2 = 0;
int special = 100;		//How much special
int special2 = 100;
int direction = 2;		//control platform moving direction
bool changewindow = 1;	//tell program to toggle fullscreen
int holding = 0;		//to avoid counting a keypress multiple times
int counter = 0;		//to slow some things down

bool wall(int x, int y, int w, int h, int make)
{
	bool wall = 0;
	int i = 0;
	int j = 0;

	while (i <= w) {
		while(j <= h){
			if (make == 1) {
				coord[x+i][y+j] = true;
				//std::cout << "Making wall at " << x+i << ", " << y+j << "\n";
			}
			if (make == 2) {
				coord[x + i][y + j] = false;
			}
			if (coord[x+i][y+j]) {
				wall = 1;
				//std::cout << "Wall found at " << x + i << ", " << y + j << "\n";
				if (!make) {
					i = 0;
					j = 0;
					return wall;
				}
			}
			j++;
		}
		i++;
		j = 0;
	}
	if (make) {
		make = 0;
	}
	i = 0;
	j = 0;
	return wall;
}
void render(SDL_Renderer* renderer, SDL_Window* window, SDL_Rect r, SDL_Rect r2, SDL_Rect l, SDL_Rect f, SDL_Rect &w1, SDL_Rect &w2, SDL_Rect &w3, SDL_Rect &w4, SDL_Rect &w5, SDL_Rect &w6, SDL_Rect w7, SDL_Rect w8, SDL_Rect Message_rect) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 14);
	if (!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	if (areamultiplier < 0.5) {
		while (((r.x - 50) / areamultiplier + xoffset) > resx / 4)
			xoffset -= 1;
		while (((r.y + 50) / areamultiplier + yoffset) > resy / 1.5)
			yoffset -= 1;
		while (((r.x - 50) / areamultiplier + xoffset) < resx / 4)
			xoffset += 1;
		while (((r.y + 50) / areamultiplier + yoffset) < resy / 1.5)
			yoffset += 1;
	}
	else {
		xoffset = 0;
		yoffset = 0;
	}

	r.x = r.x/areamultiplier + xoffset;
	r.y = r.y/areamultiplier + yoffset;
	r.w /= areamultiplier;
	r.h /= areamultiplier;

	r2.x = r2.x / areamultiplier + xoffset;
	r2.y = r2.y / areamultiplier + yoffset;
	r2.w /= areamultiplier;
	r2.h /= areamultiplier;

	l.x = l.x / areamultiplier + xoffset;
	l.y = l.y / areamultiplier + yoffset;
	l.w /= areamultiplier;
	l.h /= areamultiplier;

	f.x = f.x / areamultiplier + xoffset;
	f.y = f.y / areamultiplier + yoffset;
	f.w /= areamultiplier;
	f.h /= areamultiplier;

	w1.x = w1.x / areamultiplier + xoffset;
	w1.y = w1.y / areamultiplier + yoffset;
	w1.w /= areamultiplier;
	w1.h /= areamultiplier;

	w2.x = w2.x / areamultiplier + xoffset;
	w2.y = w2.y / areamultiplier + yoffset;
	w2.w /= areamultiplier;
	w2.h /= areamultiplier;

	w3.x = w3.x / areamultiplier + xoffset;
	w3.y = w3.y / areamultiplier + yoffset;
	w3.w /= areamultiplier;
	w3.h /= areamultiplier;

	w4.x = w4.x / areamultiplier + xoffset;
	w4.y = w4.y / areamultiplier + yoffset;
	w4.w /= areamultiplier;
	w4.h /= areamultiplier;

	w5.x = w5.x / areamultiplier + xoffset;
	w5.y = w5.y / areamultiplier + yoffset;
	w5.w /= areamultiplier;
	w5.h /= areamultiplier;

	w6.x = w6.x / areamultiplier + xoffset;
	w6.y = w6.y / areamultiplier + yoffset;
	w6.w /= areamultiplier;
	w6.h /= areamultiplier;

	w7.x = w7.x / areamultiplier + xoffset;
	w7.y = w7.y / areamultiplier + yoffset;
	w7.w /= areamultiplier;
	w7.h /= areamultiplier;

	w8.x = w8.x / areamultiplier + xoffset;
	w8.y = w8.y / areamultiplier + yoffset;
	w8.w /= areamultiplier;
	w8.h /= areamultiplier;

	Message_rect.x = 0;
	Message_rect.y = 0;
	Message_rect.w = SDL_GetWindowSurface(window)->w;
	Message_rect.h = (SDL_GetWindowSurface(window)->w)/50;
	SDL_Color black = { 0, 0, 0 };
	std::string s = std::to_string(special) + "           Q / CTRL  for  special.   Hold  up  to  boost  when  going  up,  regain  this  on  the  ground.   Bounce.            " + std::to_string(special2);
	char const *pchar = s.c_str();
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, pchar, black);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_FreeSurface(surfaceMessage);
	TTF_CloseFont(font);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderFillRect(renderer, &r);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &r2);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &l);
	SDL_SetRenderDrawColor(renderer, 155, 200, 200, 100);
	SDL_RenderFillRect(renderer, &f);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(renderer, &w1);
	SDL_RenderFillRect(renderer, &w2);
	SDL_RenderFillRect(renderer, &w3);
	SDL_RenderFillRect(renderer, &w4);
	SDL_RenderFillRect(renderer, &w5);
	SDL_RenderFillRect(renderer, &w6);
	SDL_RenderFillRect(renderer, &w7);
	SDL_RenderFillRect(renderer, &w8);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_DestroyTexture(Message);
	SDL_RenderPresent(renderer);	
}
void controls(SDL_Rect r, SDL_Rect r2, int &lvl) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event e;
	SDL_PollEvent(&e);
	int maxspeed = 6;
	if (holding > 0) {
		holding--;
	}

	if (keys[SDL_SCANCODE_D] && speedx < maxspeed) {	//player 1
		if (counter < 5)
			speedx += 2;
		if (keys[SDL_SCANCODE_Q] && wall(r.x, r.y + r.h, r.w, 2, 0)) {
			if (special > 10) {
				speedx += 20;
				special -= 100;
			}
		}
	}
	if (keys[SDL_SCANCODE_A] && speedx > -maxspeed) {
		if (counter < 5)
			speedx -= 2;
		if (keys[SDL_SCANCODE_Q] && wall(r.x, r.y + r.h, r.w, 2, 0)) {
			if (special > 0) {
				speedx -= 20;
				special -= 100;
			}
		}
	}
	if (keys[SDL_SCANCODE_W] && speedy > -maxspeed) {
		if (wall(r.x, r.y + r.h, r.w, 2, 0)) {
			speedy = -10;
			boost = 1;
			if (keys[SDL_SCANCODE_Q]) {
				if (special > 10) {
					speedy -= 15;
					special -= 100;
					boost = 0;
				}
			}
		}
		else if (boost && speedy < 0) {
			speedy -= 8;
			boost = 0;
		}
	}
	else if (keys[SDL_SCANCODE_S] && speedy < maxspeed) {
		speedx = 0;
	}
	if (keys[SDL_SCANCODE_ESCAPE])
		running = 0;
	if (keys[SDL_SCANCODE_PAGEUP]&&areamultiplier > 0.3)
		areamultiplier -= 0.02;
	if (keys[SDL_SCANCODE_PAGEDOWN]&&areamultiplier < 3)
		areamultiplier += 0.02;
	if (keys[SDL_SCANCODE_TAB] && !holding) {
		if (lvl == 6)
			lvl = 1;
		else if (lvl < 6)
			lvl++;
		holding = 10;
	}
	if (keys[SDL_SCANCODE_LALT] && keys[SDL_SCANCODE_RETURN] && !holding) {
		if (windowed)
			windowed = 0;
		else
			windowed = 1;
		changewindow = 1;
		holding = 10;
	}
}
void controls2(SDL_Rect r2, SDL_Rect r) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event e;
	SDL_PollEvent(&e);
	int maxspeed2 = 6;
	if (keys[SDL_SCANCODE_RIGHT] && speedx2 < maxspeed2) {	//player 2
		speedx2 += 2;
		if (keys[SDL_SCANCODE_RCTRL] && wall(r2.x, r2.y + r2.h, r2.w, 2, 0)) {
			if (special2 > 0) {
				speedx2 += 20;
				special2 -= 100;
			}
		}
	}
	if (keys[SDL_SCANCODE_LEFT] && speedx2 > -maxspeed2) {
		speedx2 -= 2;
		if (keys[SDL_SCANCODE_RCTRL] && wall(r2.x, r2.y + r2.h, r2.w, 2, 0)) {
			if (special2 > 0) {
				speedx2 -= 20;
				special2 -= 100;
			}
		}
	}
	if (keys[SDL_SCANCODE_UP] && speedy2 > -maxspeed2) {
		if (wall(r2.x, r2.y + r2.h, r2.w, 2, 0)) {
			speedy2 = -10;
			boost2 = 1;
			if (keys[SDL_SCANCODE_RCTRL]) {
				if (special2 > 0) {
					speedy2 -= 15;
					special2 -= 100;
					boost2 = 0;
				}
			}
		}
		else if (boost2 && speedy2 < 0) {
			speedy2 -= 8;
			boost2 = 0;
		}
	}
	else if (keys[SDL_SCANCODE_DOWN] && speedy2 < maxspeed2) {
		speedx2 = 0;
	}
}
void collision(SDL_Rect r) {
	while (wall(r.x, r.y + r.h, r.w, speedy, 0)) {
		speedy--;
	}
	while (wall(r.x, r.y + speedy, r.w, -speedy, 0)) {
		speedy++;
	}
	while (wall(r.x + speedx, r.y, -speedx, r.h - 1, 0)) {
		speedx++;
	}
	while (wall(r.x + r.w, r.y, speedx, r.h - 1, 0)) {
		speedx--;
	}
}
void collision2(SDL_Rect r2) {
	while (wall(r2.x, r2.y + r2.h, r2.w, speedy2, 0)) {
		speedy2--;
	}
	while (wall(r2.x, r2.y + speedy2, r2.w, -speedy2, 0)) {
		speedy2++;
	}
	while (wall(r2.x + speedx2, r2.y, -speedx2, r2.h - 1, 0)) {
		speedx2++;
	}
	while (wall(r2.x + r2.w, r2.y, speedx2, r2.h - 1, 0)) {
		speedx2--;
	}

}
void acceleration(SDL_Rect r, SDL_Rect r2) {
	if (counter > 0)
		counter--;
	if(!wall(r.x, r.y + r.h-1, r.w, 2, 0))
		speedy += 1;
	if (!wall(r2.x, r2.y + r2.h-1, r2.w, 2, 0))
		speedy2 += 1;
	if (wall(r.x, r.y + r.h, r.w, 2, 0) && counter < 4) {
		if (speedx > 0) {
			speedx--;
		}
		if (speedx < 0) {
			speedx++;
		}
	}
	else if (!counter) {
		if (speedx > 0) {
			speedx--;
		}
		if (speedx < 0) {
			speedx++;
		}
	}
	if (wall(r2.x, r2.y + r2.h, r2.w, 2, 0) && counter < 4) {
		if (speedx2 > 0) {
			speedx2--;
		}
		if (speedx2 < 0) {
			speedx2++;
		}
	}
	else if (!counter) {
		if (speedx2 > 0) {
			speedx2--;
		}
		if (speedx2 < 0) {
			speedx2++;
		}
	}
	if (counter == 0)
		counter = 5;
}
void level(int level, SDL_Rect &r, SDL_Rect &r2, SDL_Rect &l, SDL_Rect &f, SDL_Rect &w1, SDL_Rect &w2, SDL_Rect &w3, SDL_Rect &w4, SDL_Rect &w5, SDL_Rect &w6, SDL_Rect &w7, SDL_Rect &w8) {
	speedx = 0;
	speedx2 = 0;
	speedy = 0;
	speedy2 = 0;
	w1.x = 0; w1.y = 0; w1.w = 0; w1.h = 0;
	w2.x = 0; w2.y = 0; w2.w = 0; w2.h = 0;
	w3.x = 0; w3.y = 0; w3.w = 0; w3.h = 0;
	w4.x = 0; w4.y = 0; w4.w = 0; w4.h = 0;
	w5.x = 0; w5.y = 0; w5.w = 0; w5.h = 0;
	w6.x = 0; w6.y = 0; w6.w = 0; w6.h = 0;
	w7.x = 0; w7.y = 0; w7.w = 0; w7.h = 0;
	w8.x = 0; w8.y = 0; w8.w = 0; w8.h = 0;
	l.x = 0; l.y = 0; l.w = 0; l.h = 0;
	f.x = 0; f.y = 0; f.w = 0; f.h = 0;

if (level == 1) {
	r.x = 50;		//player 1
	r.y = 400;
	r.w = 50;
	r.h = 50;

	r2.x = 120;		//player 2
	r2.y = 400;
	r2.w = 50;
	r2.h = 50;

	f.x = 1300;		//usually using as finish
	f.y = 630;
	f.w = 50;
	f.h = 50;

	l.x = 900;
	l.y = 700;
	l.w = 505;
	l.h = 10;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

	w6.x = 5;
	w6.y = 700;
	w6.w = 300;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w1.x = 500;
	w1.y = 700;
	w1.w = 200;
	w1.h = 10;
	wall(w1.x + 1, w1.y + 1, w1.w - 2, w1.h - 1, 1);

	w7.x = 1400;
	w7.y = 100;
	w7.w = 5;
	w7.h = 600;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 5;
	w8.y = 150;
	w8.w = 3;
	w8.h = 550;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);
}

else if (level == 2) {
	r.x = 50;
	r.y = 400;
	r.w = 50;
	r.h = 50;

	r2.x = 120;
	r2.y = 400;
	r2.w = 50;
	r2.h = 50;

	f.x = 1300;
	f.y = 630;
	f.w = 50;
	f.h = 50;

	l.x = 1000;
	l.y = 700;
	l.w = 505;
	l.h = 10;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

	w6.x = 5;
	w6.y = 700;
	w6.w = 300;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w1.x = 500;
	w1.y = 700;
	w1.w = 200;
	w1.h = 10;
	wall(w1.x + 1, w1.y + 1, w1.w - 2, w1.h - 1, 1);

	w7.x = 1500;
	w7.y = 100;
	w7.w = 5;
	w7.h = 600;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 5;
	w8.y = 150;
	w8.w = 3;
	w8.h = 550;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);
}
else if (level == 3) {
	r.x = 50;
	r.y = 400;
	r.w = 50;
	r.h = 50;

	r2.x = 120;
	r2.y = 400;
	r2.w = 50;
	r2.h = 50;

	f.x = 1150;
	f.y = 430;
	f.w = 50;
	f.h = 50;				//These 3 are not solid

	l.x = 600;
	l.y = 500;
	l.w = 605;
	l.h = 10;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);		//These are solid

	w6.x = 5;
	w6.y = 700;
	w6.w = 600;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w7.x = 600;
	w7.y = 500;
	w7.w = 5;
	w7.h = 200;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 5;
	w8.y = 150;
	w8.w = 3;
	w8.h = 550;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);
}

else if (level == 4) {
	r.x = 50;
	r.y = 400;
	r.w = 50;
	r.h = 50;

	r2.x = 120;
	r2.y = 400;
	r2.w = 50;
	r2.h = 50;

	f.x = 1150;
	f.y = 430;
	f.w = 50;
	f.h = 50;

	l.x = 600;
	l.y = 500;
	l.w = 605;
	l.h = 10;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

	w6.x = 5;
	w6.y = 700;
	w6.w = 600;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w7.x = 600;
	w7.y = 500;
	w7.w = 5;
	w7.h = 200;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 5;
	w8.y = 150;
	w8.w = 3;
	w8.h = 550;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);
}

else if (level == 5) {
	r.x = 200;
	r.y = 200;
	r.w = 50;
	r.h = 50;

	r2.x = 300;
	r2.y = 200;
	r2.w = 50;
	r2.h = 50;

	f.x = 1300;
	f.y = 430;
	f.w = 50;
	f.h = 50;

	l.x = 1100;
	l.y = 500;
	l.w = 270;
	l.h = 10;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

	w6.x = 5;
	w6.y = 700;
	w6.w = 600;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w7.x = 1370;
	w7.y = 100;
	w7.w = 5;
	w7.h = 410;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 5;
	w8.y = 150;
	w8.w = 3;
	w8.h = 550;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);
	}

else if (level == 6) {
	r.x = 350;
	r.y = 250;
	r.w = 50;
	r.h = 50;

	r2.x = 475;
	r2.y = 250;
	r2.w = 50;
	r2.h = 50;

	f.x = 1100;
	f.y = 630;
	f.w = 50;
	f.h = 50;

	l.x = 275;
	l.y = 570;
	l.w = 400;
	l.h = 2;
	wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

	w5.x = 20;
	w5.y = 50;
	w5.w = 10;
	w5.h = 660;
	wall(w5.x + 1, w5.y + 1, w5.w - 2, w5.h - 1, 1);

	w6.x = 30;
	w6.y = 700;
	w6.w = 1200;
	w6.h = 10;
	wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);

	w7.x = 1000;
	w7.y = 100;
	w7.w = 5;
	w7.h = 600;
	wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);

	w8.x = 30;
	w8.y = 250;
	w8.w = 10;
	w8.h = 450;
	wall(w8.x + 1, w8.y + 1, w8.w - 2, w8.h - 1, 1);

	direction = 1;
}
}
int movinglevel(int level, SDL_Rect &r, SDL_Rect &r2, SDL_Rect f, SDL_Rect &l, SDL_Rect &w6, SDL_Rect &w7, SDL_Rect &w8) {
	if ((r.x + 50 > f.x && r.y - 50 < f.y + 50) || (r2.x + 50 > f.x && r2.y - 50 < f.y + 50)) {		//finish level trigger
		if (level == 1)
			level = 2;
		else if (level == 2) 
			level = 3;
		else if (level == 3) 
			level = 4;
		else if (level == 4) 
			level = 5;
		else if (level == 5)
			level = 6;
		else if (level == 6 && ((r.x + 50 > f.x && r.y - 50 < f.y + 50 && wall(r.x, r.y + r.h, r.w, 2, 0)) || (r2.x + 50 > f.x && r2.y - 50 < f.y + 50 && wall(r2.x, r2.y + r2.h, r2.w, 2, 0))))
			level = 1;
	}
	if (level == 4) {
		special = 0; special2 = 0;
	}
	else if (level == 6) {			//platforms
		if (direction == 1) {			
			if (l.x < 500) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.x += 2;
				if (l.y - r.y > 49 && l.y - r.y < 53 && r.x + r.w>l.x && r.x < l.x + l.w) {
					r.x += 2;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r2.x += 2;
				}
				if (l.y - r2.y > 49 && l.y - r2.y < 53 && r2.x + r2.w>l.x && r2.x < l.x + l.w) {
					r2.x += 2;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r.x += 2;
				}
			}
			else
				direction = 2;
		}
		if (direction == -1) {
			if (l.x > 150) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.x -= 2;
				if (l.y - r.y > 49 && l.y - r.y < 53 && r.x + r.w>l.x && r.x < l.x + l.w) {
					r.x -= 2;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r2.x -= 2;
				}
				if (l.y - r2.y > 49 && l.y - r2.y < 53 && r2.x + r2.w>l.x && r2.x < l.x + l.w) {
					r2.x -= 2;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r.x -= 2;
				}
			}
			else
				direction = -2;
		}
		if (direction == 2) {
			if (l.y < 630) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.y += 2;
				while (l.y - r.y > 50 && l.y - r.y < 53 && r.x + r.w>l.x && r.x < l.x + l.w) {
					r.y++;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r2.y++;
				}
				while (l.y - r2.y > 50 && l.y - r2.y < 53 && r2.x + r2.w>l.x && r2.x < l.x + l.w) {
					r2.y++;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r.y++;
				}
			}
			else
				direction = -1;
		}
		if (direction == -2) {
			if (l.y > 550) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.y -= 2;
				while (l.y - r.y < 50 && l.y - r.y > 10 && r.x + r.w>l.x && r.x < l.x + l.w) {
					r.y--;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r2.y--;
				}
				while (l.y - r2.y < 50 && l.y - r2.y > 10 && r2.x + r2.w>l.x && r2.x < l.x + l.w) {
					r2.y--;
					if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51)
						r.y--;
				}
			}
			else
				direction = 1;
		}
		wall(w7.x + 1, w7.y + 1, w7.w - 2, w7.h - 1, 1);	//Move platforms
		wall(w6.x + 1, w6.y + 1, w6.w - 2, w6.h - 1, 1);
		wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);
	}
	return level;
}
void playercollision(SDL_Rect r, SDL_Rect r2) {
	int temp = 0;
	//if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x + speedx > -51 && r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51) { //calculate if they're gonna hit or...
	//if (r2.x - r.x < 50 && r2.x - r.x > -50 && r2.y - r.y < 50 && r2.y - r.y > -50) { //bounce when they're already inside each other???
	if (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x - speedx > -51 && r2.y - r.y < 51 && r2.y - r.y > -51) {
	//	if (!wall(r.x, r.y + r.h, r.w, 2, 0) && !wall(r2.x, r2.y + r2.h, r2.w, 2, 0)) {
			temp = speedx;
			speedx = speedx2;
			speedx2 = temp;
//			}
			if (speedx*speedx2 > 0) {	//if they're going the same way
				if (r.x > r2.x)
					speedx++;
				else
					speedx2++;
			}
			while (r2.x + speedx2 - r.x - speedx < 51 && r2.x + speedx2 - r.x - speedx > -51 && r2.y - r.y < 51 && r2.y - r.y > -51) {
				if (r.x > r2.x)
					speedx++;
				else
					speedx2++;
			}
		}
	if (r2.y + speedy2 - r.y - speedy < 51 && r2.y + speedy2 - r.y - speedy > -51 && r2.x - r.x < 51 && r2.x - r.x > -51) {
		temp = speedy;
		if (speedy < 0 && speedy2>0 || speedy > 0 && speedy2 < 0 || (!wall(r.x, r.y + r.h, r.w, 5, 0) && !wall(r2.x, r2.y + r2.h, r2.w, 5, 0))) {
			speedy = speedy2;
			speedy2 = temp;
		}
		else {
			speedy = -speedy + 1;
			speedy2 = -speedy2 + 1;
		}
	}
}

int main(int argc, char** argv)
{
	int fps = 60;
	int physicsfps = 60;
	int i = 0;
	int j = 0;

	std::ifstream infile("conf.cfg");
	std::string line;
	if (infile.is_open()) {
		while (infile.good()) {
			std::getline(infile, line);
			std::cout << line << ": ";
			std::getline(infile, line);
			std::cout << line << "\n";
			resx = std::stoi(line);
			std::getline(infile, line);
			std::cout << line << ": ";
			std::getline(infile, line);
			std::cout << line << "\n";
			resy = std::stoi(line);
			std::getline(infile, line);
			std::cout << line << ": ";
			std::getline(infile, line);
			std::cout << line << "\n";
			windowed = std::stoi(line);
		}
		infile.close();
	}
	else
		std::cout << "Not open!\n";

	SDL_Window* window = NULL;
	window = SDL_CreateWindow
	(
		"Testing", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		resx,
		resy,
		SDL_WINDOW_RESIZABLE
	);
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event e;

	// Setup renderer
	SDL_Renderer* renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Init();

	SDL_Rect Message_rect;
	Message_rect.x = 0;
	Message_rect.y = 0;

	// Set render color to red ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
	SDL_Rect r;
	SDL_Rect r2;
	SDL_Rect l;
	SDL_Rect f;
	SDL_Rect w1;
	w1.x = 0; w1.y = 0; w1.w = 0; w1.h = 0;
	SDL_Rect w2;
	w2.x = 0; w2.y = 0; w2.w = 0; w2.h = 0;
	SDL_Rect w3;
	w3.x = 0; w3.y = 0; w3.w = 0; w3.h = 0;
	SDL_Rect w4;
	w4.x = 0; w4.y = 0; w4.w = 0; w4.h = 0;
	SDL_Rect w5;
	w5.x = 0; w5.y = 0; w5.w = 0; w5.h = 0;
	SDL_Rect w6;
	w6.x = 0; w6.y = 0; w6.w = 0; w6.h = 0;
	SDL_Rect w7;
	SDL_Rect w8;

	bool makelevel = 1;
	int lvl = 1;
	int prevlvl = lvl;
	SDL_PollEvent(&e);

	clock_t t1,t2,t3;
	t1 = clock();
	t3 = clock();

	while (running)
	{
		if (makelevel) {
			wall(0, 0, 1280, 720, 2);
			level(lvl, r, r2, l, f, w1, w2, w3, w4, w5, w6, w7, w8);
			makelevel = 0;
		}
		t2 = clock();
		if ((t2 - t1) > (1000 / fps)) {
			if (windowed && changewindow) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				changewindow = 0;
			}
			if (!windowed && changewindow) {
				SDL_SetWindowFullscreen(window, 0);
				SDL_SetWindowSize(window, resx, resy);
				changewindow = 0;
			}
			render(renderer, window, r, r2, l, f, w1, w2, w3, w4, w5, w6, w7, w8, Message_rect);	//render
			t1 = clock();
		}
		SDL_PollEvent(&e);			
		if (e.type == SDL_QUIT) {		
			break;
		}
		if (keys[SDL_SCANCODE_R] || r.y>1220 || r2.y>1220) {				//reset
			wall(0, 0, 3840, 2160, 2);
			level(lvl, r, r2, l, f, w1, w2, w3, w4, w5, w6, w7, w8);
			speedy = 0;
			speedy2 = 0;
		}
		if ((t2 - t3) > (1000 / physicsfps)) {			//Game
			t3 = clock();	

			controls(r,r2,lvl);
			controls2(r2,r);

			playercollision(r, r2);

			collision(r);	//collision player 1
			collision2(r2);	//collision player 2
			
			r.x += speedx;
			r.y += speedy;
			r2.x += speedx2;
			r2.y += speedy2;

			acceleration(r,r2);	//gravity and horizontal

			lvl = movinglevel(lvl, r, r2, f, l, f, w7, w8);
			if (lvl != prevlvl) {
				makelevel = 1;
				prevlvl = lvl;
				std::cout << "Level " << lvl << "\n";
			}
			if (special < 1 && special > -80) {
				special -=200;
			}
			if (special < 100){
				special++;
				if (special > -100 && special < -80)
					special = 1;
			}
			if (special2 < 1 && special2 > -80) {
				special2 -=200;
			}
			if (special2 < 100) {
				special2++;
				if (special2 > -100 && special2 < -80)
					special2 = 1;
			}

		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}