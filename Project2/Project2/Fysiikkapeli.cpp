#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <fstream>

bool running = 1;
int resx = 1600;
int resy = 900;
bool windowed = 1;
float areamultiplier = 1;
int xoffset = 0;
int yoffset = 0;
bool coord[3841][2161] = { false };
int speedx = 0;
int speedy = 0;
int speedx2 = 0;
int speedy2 = 0;
bool boost = 0;
bool boost2 = 0;
int special = 100;
int special2 = 100;
int direction = 2;
bool input = 1;
int temp = 0;
bool changewindow = 1;
int holding = 0;

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
void render(SDL_Renderer* renderer, SDL_Window* window, SDL_Rect r, SDL_Rect r2, SDL_Rect l, SDL_Rect f, SDL_Rect w, SDL_Rect w2, SDL_Rect Message_rect) {
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 11);
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

	w.x = w.x / areamultiplier + xoffset;
	w.y = w.y / areamultiplier + yoffset;
	w.w /= areamultiplier;
	w.h /= areamultiplier;

	w2.x = w2.x / areamultiplier + xoffset;
	w2.y = w2.y / areamultiplier + yoffset;
	w2.w /= areamultiplier;
	w2.h /= areamultiplier;

	Message_rect.x = 0;
	Message_rect.y = 0;
	Message_rect.w = SDL_GetWindowSurface(window)->w;
	Message_rect.h = (SDL_GetWindowSurface(window)->w)/30;
	SDL_Color black = { 0, 0, 0 };
	std::string s = std::to_string(special) + "                                                          " + std::to_string(special2);
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
	SDL_RenderFillRect(renderer, &f);
	SDL_RenderFillRect(renderer, &w);
	SDL_RenderFillRect(renderer, &w2);
	SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
	SDL_DestroyTexture(Message);
	SDL_RenderPresent(renderer);	
}
void controls(SDL_Rect r) {
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_Event e;
	SDL_PollEvent(&e);
	int maxspeed = 6;

	if (keys[SDL_SCANCODE_D] && speedx < maxspeed) {	//player 1
		speedx += 2;
		if (keys[SDL_SCANCODE_Q]&& wall(r.x, r.y + r.h, r.w, 2, 0)) {
			if (special > 0) {
				speedx += 20;
				special -= 100;
			}
		}
	}
	if (keys[SDL_SCANCODE_A] && speedx > -maxspeed) {
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
				if (special > 0) {
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
		speedy += 1;
	}
	if (keys[SDL_SCANCODE_ESCAPE])
		running = 0;
	if (keys[SDL_SCANCODE_PAGEUP]&&areamultiplier > 0.3)
		areamultiplier -= 0.02;
	if (keys[SDL_SCANCODE_PAGEDOWN]&&areamultiplier < 3)
		areamultiplier += 0.02;
	if (keys[SDL_SCANCODE_LALT] && keys[SDL_SCANCODE_RETURN] && !holding) {
		if (windowed)
			windowed = 0;
		else
			windowed = 1;
		changewindow = 1;
		holding = 10;
	}
	else if (holding > 0){
		holding--;
	}
}
void controls2(SDL_Rect r2) {
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
		speedy2 += 1;
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
	speedy += 1;
	speedy2 += 1;
	if (wall(r.x, r.y + r.h, r.w, 2, 0)) {
		if (speedx > 0) {
			speedx--;
		}
		if (speedx < 0) {
			speedx++;
		}
	}
	if (wall(r2.x, r2.y + r2.h, r2.w, 2, 0)) {
		if (speedx2 > 0) {
			speedx2--;
		}
		if (speedx2 < 0) {
			speedx2++;
		}
	}
}
void level(int level, SDL_Rect &r, SDL_Rect &r2, SDL_Rect &l, SDL_Rect &f, SDL_Rect &w, SDL_Rect &w2) {
	speedx = 0;
	speedx2 = 0;
	speedy = 0;
	speedy2 = 0;
if (level == 1) {
		r.x = 300;
		r.y = 50;
		r.w = 50;
		r.h = 50;

		r2.x = 475;
		r2.y = 50;
		r2.w = 50;
		r2.h = 50;

		l.x = 275;
		l.y = 470;
		l.w = 500;
		l.h = 2;
		wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

		f.x = 10;
		f.y = 600;
		f.w = 1200;
		f.h = 10;
		wall(f.x + 1, f.y + 1, f.w - 2, f.h - 1, 1);

		w.x = 1000;
		w.y = 100;
		w.w = 5;
		w.h = 500;
		wall(w.x + 1, w.y + 1, w.w - 2, w.h - 1, 1);

		w2.x = 20;
		w2.y = 150;
		w2.w = 5;
		w2.h = 450;
		wall(w2.x + 1, w2.y + 1, w2.w - 2, w2.h - 1, 1);
	}

if (level == 2) {
		r.x = 300;
		r.y = 200;
		r.w = 50;
		r.h = 50;

		r2.x = 200;
		r2.y = 200;
		r2.w = 50;
		r2.h = 50;

		l.x = 1000;
		l.y = 500;
		l.w = 270;
		l.h = 10;
		wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

		f.x = 5;
		f.y = 700;
		f.w = 600;
		f.h = 10;
		wall(f.x + 1, f.y + 1, f.w - 2, f.h - 1, 1);

		w.x = 1270;
		w.y = 100;
		w.w = 5;
		w.h = 600;
		wall(w.x + 1, w.y + 1, w.w - 2, w.h - 1, 1);

		w2.x = 5;
		w2.y = 150;
		w2.w = 3;
		w2.h = 550;
		wall(w2.x + 1, w2.y + 1, w2.w - 2, w2.h - 1, 1);
	}
}
int movinglevel(int level, SDL_Rect &r, SDL_Rect &r2, SDL_Rect &l, SDL_Rect &f, SDL_Rect &w, SDL_Rect &w2) {
	if (level == 1) {
		if (direction == 1) {			//platforms
			if (l.x < 500) {
				wall(l.x, l.y + 1, 1, l.h - 1, 2);
				l.x++;
			}
			else
				direction = -1;
		}
		if (direction == -1) {
			if (l.x > 0) {
				wall(l.x + l.w, l.y + 1, 1, l.h - 1, 2);
				l.x--;
			}
			else
				direction = 1;
		}
		if (direction == 2) {
			if (l.y < 550) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.y++;
			}
			else
				direction = -2;
		}
		if (direction == -2) {
			if (l.y > 350) {
				wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 2);
				l.y--;
			}
			else
				direction = 2;
		}
		wall(w.x + 1, w.y + 1, w.w - 2, w.h - 1, 1);	//Move platforms
		wall(f.x + 1, f.y + 1, f.w - 2, f.h - 1, 1);
		wall(l.x + 1, l.y + 1, l.w - 2, l.h - 1, 1);

		if ((r.x > 1000 && wall(r.x, r.y + r.h, r.w, 2, 0)) || (r2.x > 1000 && wall(r2.x, r2.y + r2.h, r2.w, 2, 0))) {
			level = 2;
		}
	}
	else if (level == 2) {
		if ((r.x > 1100 && r.y < 510) || (r2.x > 1100 && r2.y < 510)) {
			level = 1;
		}
	}
	return level;
}
void playercollision(SDL_Rect r, SDL_Rect r2) {
	if (wall(r2.x + 1, r2.y + r2.h, r2.w - 2, 2, 0) || wall(r.x + 1, r.y + r.h, r.w - 2, 2, 0) || (!speedx && !speedx2)) {
		input = 1;
	}
	if (r2.x - r.x < 50 && r2.x - r.x > -50 && r2.y - r.y < 50 && r2.y - r.y > -50) { //player collision
		if ((speedy < -4 && speedy2 < -4) || (r.y-r2.y < 35 && r.y-r2.y > -35)) {
			std::cout << speedy << ", " << speedy2 << ", " << r.y - r2.y << "\n";
			temp = speedx;
			speedx = speedx2;
			speedx2 = temp;
		}
		if (r.x < r2.x) {
			speedx -= 2;
			speedx2 += 2;
		}
		else if (r.x > r2.x) {
			speedx2 -= 2;
			speedx += 2;
		}
		temp = speedy;
		if (speedy < 0 && speedy2>0 || speedy > 0 && speedy2 < 0 || (!wall(r.x, r.y + r.h, r.w, 5, 0) && !wall(r2.x, r2.y + r2.h, r2.w, 5, 0))) {
			speedy = speedy2;
			speedy2 = temp;
		}
		else {
			speedy = -speedy + 1;
			speedy2 = -speedy2 + 1;
		}
		if (r2.x - r.x < 40 && r2.x - r.x > -40 && r2.y - r.y < 45 && r2.y - r.y > -45)
			input = 0;
		if (!wall(r2.x, r2.y + r2.h, r2.w, 2, 0) && !wall(r.x, r.y + r.h, r.w, 2, 0)) {
			input = 0;
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
	Message_rect.w = 200;
	Message_rect.h = 30;

	// Set render color to red ( background will be rendered in this color )
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Clear winow
	SDL_RenderClear(renderer);

	// Creat a rect at pos ( 50, 50 ) that's 50 pixels wide and 50 pixels high.
	SDL_Rect r;
	r.x = 275;
	r.y = 50;
	r.w = 50;
	r.h = 50;

	SDL_Rect r2;
	r2.x = 475;
	r2.y = 50;
	r2.w = 50;
	r2.h = 50;

	SDL_Rect l;
	l.x = 275;
	l.y = 270;
	l.w = 500;
	l.h = 2;

	SDL_Rect f;
	f.x = 10;
	f.y = 600;
	f.w = 1200;
	f.h = 10;

	SDL_Rect w;
	w.x = 1000;
	w.y = 100;
	w.w = 5;
	w.h = 500;

	SDL_Rect w2;
	w2.x = 20;
	w2.y = 150;
	w2.w = 5;
	w2.h = 450;

	bool makelevel = 1;
	int lvl = 2;
	int prevlvl = lvl;
	SDL_PollEvent(&e);

	clock_t t1,t2,t3;
	t1 = clock();
	t3 = clock();

	while (running)
	{
		if (makelevel) {
			wall(0, 0, 1280, 720, 2);
			level(lvl, r, r2, l, f, w, w2);
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
			render(renderer, window, r, r2, l, f, w, w2, Message_rect);	//render
			t1 = clock();
		}
		SDL_PollEvent(&e);			
		if (e.type == SDL_QUIT) {		
			break;
		}
		if (keys[SDL_SCANCODE_R] || r.y>1220 || r2.y>1220) {				//reset
			wall(0, 0, 1280, 720, 2);
			level(lvl, r, r2, l, f, w, w2);
			speedy = 0;
			speedy2 = 0;
		}
		if ((t2 - t3) > (1000 / physicsfps)) {			//Game
			t3 = clock();			
			if (input) {
				controls(r);
				controls2(r2);
			}

			playercollision(r, r2);

			collision(r);	//collision player 1
			collision2(r2);	//collision player 2
			
			r.x += speedx;
			r.y += speedy;
			r2.x += speedx2;
			r2.y += speedy2;

			acceleration(r,r2);	//gravity and horizontal
			lvl = movinglevel(lvl, r, r2, l, f, w, w2);
			if (lvl != prevlvl) {
				makelevel = 1;
				prevlvl = lvl;
				printf("Make new level\n");
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