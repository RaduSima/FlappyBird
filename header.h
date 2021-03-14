#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h> 
#include <SDL_mixer.h>

//CONFIGS
//Window defines
	//Window dimensions
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FRAME_RATE 60

	//Wiondow colors
#define WINDOW_COLOR_RED 150
#define WINDOW_COLOR_GREEN 100
#define WINDOW_COLOR_BLUE 70

//Background defines
#define BACKGROUND_WIDTH WINDOW_WIDTH / 2
#define BACKGROUND_HEIGHT WINDOW_HEIGHT

//Base defines
#define BASES_NUMBER 3
#define BASE_WIDTH 336
#define BASE_HEIGHT 112

//Bird defines
#define BIRD_WIDTH 36 + WIDE_AMPLIFIER
#define WIDE_AMPLIFIER 0
#define BIRD_HEIGHT 24 + LONG_AMPLIFIER
#define LONG_AMPLIFIER 0
#define BIRD_X 100
#define BIRD_Y 180
#define BIRD_FRAMES_NUMBER 3
#define BIRD_SPEED 2

//Bird states
#define BIRD_LOADING -2
#define BIRD_UNLOADING -1
#define BIRD_DEAD 0
#define BIRD_ALIVE 1

//Pipe defines
#define PIPES_NUMBER 1000
#define PIPES_X_DISTANCE 115
#define PIPES_Y_DISTANCE 100
#define PIPES_WIDTH 52
#define PIPES_HEIGHT 320
#define PIPES_X_STARTING_POINT WINDOW_WIDTH 

//Gravity
#define GRAVITY 0.55
#define MAX_GRAVITY 5

//Jump
#define JUMP_FORCE 100
#define MAX_JUMP -7

//Fonts
#define FONT_SIZE 30
#define AUTISM_FONT_SIZE 10

//Structs
typedef struct base
{
	float x, y, w, h;
} Base;

typedef struct background
{
	float x, y, w, h;
} Background;

typedef struct bird
{
	float x, y, w, h, dx, dy, angle;
	int state;
} Bird;

typedef struct pipe
{
	float x, y, w, h;
} Pipe;

typedef struct timer
{
	int start_ticks, elapsed_ticks, frame_count;
	float delta_time;

} Timer;

typedef struct gameState
{
	//Time
	Timer timer;

	int bases_counter;
	int pipes_counter;
	int pipes_score_counter;

	int score, bestScore, newBestScoreFlag;

	//Font
	TTF_Font* font, *autismFont;

	//Surface
	SDL_Surface* surface;

	//Renderer
	SDL_Renderer* renderer;

	//Window
	SDL_Window* window;

	//Bases
	Base* bases;

	//Background
	Background background;

	//Bird
	Bird bird;

	//Pipes
	Pipe* pipesLower; 
	Pipe* pipesUpper;

	//Textures
	SDL_Texture* baseTexture;

	SDL_Texture* backgroundTexture;

	SDL_Texture** birdTexture;
	int birdTextureCounter;

	SDL_Texture* PipeLowerTexture;
	SDL_Texture* PipeUpperTexture;

	SDL_Texture* label;
	int labelWidth, labelHeight, bestScoreLabelWidth;

	SDL_Texture* gameOverTexture;

	//Sounds
	Mix_Chunk *dieSound, *hitSound, *pointSound, *swooshSound, *wingSound;

	//Done
	int done;

	//Autism mode
	int autismMode;

} GameState;

//Functions
	//Load functions
void loadGame(GameState* gameState);
void initSDL();
void allocateMemory(GameState* gameState);
void declareVariables(GameState* gameState);
void initPipes(gameState);
void createWindow(GameState* gameState);
void createRenderer(GameState* gameState);
void loadFont(GameState* gameState);
void createTextures(GameState* gameState);
void createBirdTextures(GameState* gameState);
void createTexture(SDL_Texture** texture, char* img_path, SDL_Renderer* renderer);
void loadSounds(GameState* gameState);
void loadAutisticSounds(GameState* gameState);

	//Unload functions
void closeGame(GameState* gameState);
void freeMemory(GameState* gameState);
void unloadSounds(GameState* gameState);
void closeSDL();
void error();

	//Event check functions
void checkEvent(GameState* gameState);

	//Process functions
void process(GameState* gameState);
void updateScore(GameState* gameState);
void updateBasesPosition(GameState* gameState);
void updatePipesPosition(GameState* gameState);
void updateBirdPosition(GameState* gameState);
void collision(GameState* gameState);
void updateBestScore(GameState* gameState);
void resetTimer(GameState* gameState);
void updateTimer(GameState* gameState);

	//Render functions
void render(GameState* gameState);
void renderBackground(GameState* gameState);
void renderPipes(GameState* gameState);
void renderBird(GameState* gameState);
void renderBases(GameState* gameState);
void renderScore(GameState* gameState);
void renderGameOver(GameState* gameState);
void renderOkButton(GameState* gameState);
void renderAutismButton(GameState* gameState);
void renderNewBestScore(GameState* gameState);