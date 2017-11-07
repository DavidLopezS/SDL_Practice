#include <SDL.h>		// Always needs to be included for an SDL app
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <time.h>
#include <iostream>

//Game general information
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

int posX, posY;

bool isInPlace(int x, int y, SDL_Rect theRect) {
	if (x > theRect.x && y > theRect.y && x < theRect.x + theRect.w && y < theRect.y + theRect.h) {
		return true;
	}
	else {
		return false;
	}
}

int main(int, char*[]) {

	// --- INIT ---
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) throw "No es pot inicialitzar SDL subsystems";

	const Uint8 imgFlags{ IMG_INIT_PNG | IMG_INIT_JPG };
	if (!(IMG_Init(imgFlags) & imgFlags)) throw "Error : SDL_image init";

	if (TTF_Init() != 0) "No es pot inicialitzar SDL_ttf";

	const Uint8 mixFlags{ MIX_INIT_MP3 | MIX_INIT_OGG };
	if (!(Mix_Init(mixFlags) & mixFlags)) throw "Error : SDL_mixer init";

	// --- WINDOW ---
	SDL_Window *window{ SDL_CreateWindow("SDL...", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN) };
	if (window == nullptr) throw "No es pot inicialitzar SDL_Window";

	// --- RENDERER ---
	SDL_Renderer *renderer{ SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) };
	if (renderer == nullptr) throw "No es pot inicialitzar SDL_Renderer";

	// --- SPRITES ---
	//bg1
	SDL_Texture *bgTexture{ IMG_LoadTexture(renderer, "../res/img/bg.jpg") };
	if (bgTexture == nullptr) throw "No s'ha pogut crear les textures";
	SDL_Rect bgRect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Bg2
	SDL_Texture *bgTexture2{ IMG_LoadTexture(renderer, "../res/img/bgCastle.jpg") };
	if (bgTexture2 == nullptr) throw "No s'ha pogut crear les textures";
	SDL_Rect bgRect2{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	
	//Kinton
	/*SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../res/img/Kintoun.png") };
	if(playerTexture == nullptr) throw "No s'ha pogut crear les textures";
	
	SDL_Rect playerRect{ 0, 0, 350/2, 189/2 };
	SDL_Rect playerTarget{ 0, 0, 100, 100 };
	*/

	SDL_Texture *cloudTexture{ IMG_LoadTexture(renderer, "../res/img/gold.png") };
	if (cloudTexture== nullptr) throw "No s'ha pogut crear les textures";

	SDL_Rect cloudRect{ 2, 500, 350/4, 189/4 };

	SDL_Texture *cloudTexture2{ IMG_LoadTexture(renderer, "../res/img/gold.png") };
	if (cloudTexture2 == nullptr) throw "No s'ha pogut crear les textures";

	SDL_Rect cloudRect2{ 350, 400, 350/6, 189/6};

	SDL_Texture *cloudTexture3{ IMG_LoadTexture(renderer, "../res/img/gold.png") };
	if (cloudTexture3 == nullptr) throw "No s'ha pogut crear les textures";

	SDL_Rect cloudRect3{ 550, 300, 350 / 5, 189 / 5 };

	SDL_Texture *cloudTexture4{ IMG_LoadTexture(renderer, "../res/img/gold.png") };
	if (cloudTexture4 == nullptr) throw "No s'ha pogut crear les textures";

	SDL_Rect cloudRect4{ 450, 200, 350 / 8, 189 / 8 };

	SDL_Texture *moneyTexture{ IMG_LoadTexture(renderer, "../res/img/gold.png") };
	if (moneyTexture == nullptr) throw "This is no good";

	SDL_Rect moneyRect{ 208, 470, 350 / 4, 189 / 4 };
		// --- Animated Sprite ---
		//Castle Character

	SDL_Texture *playerTexture{ IMG_LoadTexture(renderer, "../res/img/spCastle.png") };
	SDL_Rect playerRect, playerPosition;
	int textWidth, textHeight, frameWidth, frameHeight;
	SDL_QueryTexture(playerTexture, NULL, NULL, &textWidth, &textHeight);
	frameWidth = textWidth / 12;
	frameHeight = textHeight / 8;
	playerPosition.x = playerPosition.y = 200;
	playerRect.x = playerRect.y = 0;//32,64,96
	playerPosition.h = playerRect.h = frameHeight;
	playerPosition.w = playerRect.w = frameWidth;
	int frameTime = 0;
	// --- TEXT ---
	TTF_Font *font{ TTF_OpenFont("../res/ttf/saiyan.ttf", 80) };
	if (font == nullptr) throw "No es pot inicialitzar the TTF_Font";
	
	Uint8 colorA = 255;
	Uint8 colorB = 150;
	Uint8 colorC = 0;
	Uint8 colorD = 150;
	SDL_Surface *tmpSurf{ TTF_RenderText_Blended(font, "Play", SDL_Color{ colorA, colorB, 0, 255}) };
	if (tmpSurf == nullptr) throw "UNable to exist XD jajajaja LOL";
	SDL_Texture *textTexture{ SDL_CreateTextureFromSurface(renderer, tmpSurf) };
	SDL_Rect textRect{ 350, 100, tmpSurf->w, tmpSurf->h };
	SDL_FreeSurface(tmpSurf);

	SDL_Surface *tmpSurf2{ TTF_RenderText_Blended(font, "Quit", SDL_Color{ colorC, colorD, 0, 255 }) };
	if (tmpSurf2 == nullptr) throw "UNable to exist XD jajajaja LOL";
	SDL_Texture *textTexture2{ SDL_CreateTextureFromSurface(renderer, tmpSurf2) };
	SDL_Rect textRect2{ 350, 250, tmpSurf2->w, tmpSurf2->h };
	SDL_FreeSurface(tmpSurf2);

	TTF_CloseFont(font);
	// --- AUDIO ---
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		throw "Not able";
	}
	Mix_Music *soundtrack{ Mix_LoadMUS("../res/au/mainTheme.mp3") };
	if (!soundtrack) throw "JAjajajaja LOL";
	Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
	Mix_PlayMusic(soundtrack, -1);
	// --- TIME ---
	clock_t lastTime = clock();
	float timeDown = 10.f;
	float deltaTime = 0.f;

	// --- GAME LOOP ---
	SDL_Event event;
	bool isRunning = true;
	bool click = false;
	bool currScene = true;
	while (isRunning){
		// HANDLE EVENTS
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:		isRunning = false; break;
			case SDL_KEYDOWN:	
				if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false; 
				if(event.key.keysym.sym == SDLK_DOWN){
					playerRect.x = playerRect.y = 0;
					playerPosition.y += 10;
					if(playerPosition.y == 580){
						playerPosition.y = 570;
					}
				}
				if (event.key.keysym.sym == SDLK_LEFT) {
					playerRect.x = playerRect.y = 32;
					playerPosition.x -= 10;
					if (playerPosition.x == -10) {
						playerPosition.x = 0;
					}
				}
				if (event.key.keysym.sym == SDLK_RIGHT) {
					playerRect.x = playerRect.y = 64;
					playerPosition.x += 10;
					if (playerPosition.x == 780) {
						playerPosition.x = 770;
					}
				}
				if (event.key.keysym.sym == SDLK_UP) {
					playerRect.x = playerRect.y = 96;
					playerPosition.y -= 10;
					if (playerPosition.y == 150) {
						playerPosition.y = 160;
					}
				}
				break;
			case SDL_MOUSEMOTION:
				//playerTarget.x = event.motion.x - 50; 
				//playerTarget.y = event.motion.y - 50;
				posX = event.motion.x;
				posY = event.motion.y;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					click = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				click = false;
				break;
			default:;
			}
		}
		bool isTherePlay = isInPlace(posX, posY, textRect);
		bool isThereExits = isInPlace(posX, posY, textRect2);
		// UPDATE
		//playerRect.x += (playerTarget.x - playerRect.x) / 10;
		//playerRect.y += (playerTarget.y - playerRect.y) / 10;
		frameTime++;
		if (FPS / frameTime <= 9) {
			frameTime = 0;
			playerRect.x += frameWidth;
			if (playerRect.x >= textWidth/4) {
				playerRect.x = 0;
			}
		}

		deltaTime = (clock() - lastTime);
		lastTime = clock();
		deltaTime /= CLOCKS_PER_SEC;
		timeDown -= deltaTime;
		//std::cout << timeDown << std::endl;
		if (isTherePlay == true && click == true){
			currScene = false;
		}
		if (isThereExits == true && click == true) {
			isRunning = false;
		}
		// DRAW
			//Background
		if (currScene == true) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bgTexture, nullptr, &bgRect);
			SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
			SDL_RenderCopy(renderer, textTexture2, nullptr, &textRect2);
			//SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
			//Animated Sprite
		//SDL_RenderCopy(renderer, playerTexture, &playerRect, &playerPosition);
			SDL_RenderPresent(renderer);
		}
		if (currScene == false) {
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bgTexture2, nullptr, &bgRect2);
			SDL_RenderCopy(renderer, cloudTexture, nullptr, &cloudRect);
			SDL_RenderCopy(renderer, cloudTexture2, nullptr, &cloudRect2);
			SDL_RenderCopy(renderer, cloudTexture3, nullptr, &cloudRect3);
			SDL_RenderCopy(renderer, cloudTexture4, nullptr, &cloudRect4);
			SDL_RenderCopy(renderer, moneyTexture, nullptr, &moneyRect);
			//Animated Sprite
			SDL_RenderCopy(renderer, playerTexture, &playerRect, &playerPosition);
			SDL_RenderPresent(renderer);
		} 
	}

	// --- DESTROY ---
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(textTexture2);
	SDL_DestroyTexture(bgTexture2);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(cloudTexture);
	SDL_DestroyTexture(cloudTexture2);
	SDL_DestroyTexture(cloudTexture3);
	SDL_DestroyTexture(cloudTexture4);
	SDL_DestroyTexture(moneyTexture);
	Mix_CloseAudio();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	// --- QUIT ---
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
}