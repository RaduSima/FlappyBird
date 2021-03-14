#include "header.h"

void loadGame(GameState* gameState)
{
	gameState->autismMode = 0;

	initSDL();

	allocateMemory(gameState);

	declareVariables(gameState);

	createWindow(gameState);

	createRenderer(gameState);

	loadFont(gameState);

	createTextures(gameState);

	loadSounds(gameState);
}

void closeGame(GameState* gameState)
{
	freeMemory(gameState);
	unloadSounds(gameState);
	closeSDL();
}

void initSDL()
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); //Start SDL2
	TTF_Init(); //Start SDL_ttf
	Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
}

void allocateMemory(GameState* gameState)
{
	gameState->bases = malloc(BASES_NUMBER * sizeof(Base));
	if (!gameState->bases)
		error();

	gameState->pipesLower = malloc(PIPES_NUMBER * sizeof(Pipe));
	if (!gameState->pipesLower)
		error();
	gameState->pipesUpper = malloc(PIPES_NUMBER * sizeof(Pipe));
	if (!gameState->pipesUpper)
		error();

	gameState->birdTexture = malloc(BIRD_FRAMES_NUMBER * sizeof(SDL_Texture*));
	if (!gameState->birdTexture)
		error();
}

void declareVariables(GameState* gameState)
{
	int i;

	gameState->done = 0;
	gameState->score = 0;
	gameState->bestScore = 0;
	gameState->newBestScoreFlag = 0;
	gameState->bases_counter = 0;
	gameState->pipes_counter = 0;
	gameState->pipes_score_counter = 0;
	gameState->bestScoreLabelWidth = 0;
	gameState->birdTextureCounter = 0;

	//Timer

	gameState->timer.frame_count = 0;
	gameState->timer.delta_time = 0.0f;

	gameState->timer.start_ticks = SDL_GetTicks();
	gameState->timer.elapsed_ticks = 0;

	//Bases
	for (i = 0; i < BASES_NUMBER; i++)
	{
		gameState->bases[i].w = BASE_WIDTH;
		gameState->bases[i].h = BASE_HEIGHT;
		gameState->bases[i].x = i * BASE_WIDTH;
		gameState->bases[i].y = WINDOW_HEIGHT - BASE_HEIGHT;
	}

	//Background
	gameState->background.w = BACKGROUND_WIDTH;
	gameState->background.h = BACKGROUND_HEIGHT;
	gameState->background.x = 0;
	gameState->background.y = 0;

	//Bird
	gameState->bird.dx = BIRD_SPEED;
	gameState->bird.dy = 0;
	gameState->bird.w = BIRD_WIDTH;
	gameState->bird.h = BIRD_HEIGHT;
	gameState->bird.x = BIRD_X;
	gameState->bird.y = BIRD_Y;
	gameState->bird.angle = 0;
	gameState->bird.state = BIRD_LOADING;

	//Pipes
	initPipes(gameState);

}

void initPipes(GameState* gameState)
{
	int i;
	//First one
	gameState->pipesLower[0].x = PIPES_X_STARTING_POINT;
	gameState->pipesLower[0].y = WINDOW_HEIGHT - 250;
	gameState->pipesLower[0].w = PIPES_WIDTH;
	gameState->pipesLower[0].h = PIPES_HEIGHT;

	gameState->pipesUpper[0].x = PIPES_X_STARTING_POINT;
	gameState->pipesUpper[0].y = gameState->pipesLower[0].y - gameState->pipesLower[0].h - PIPES_Y_DISTANCE;
	gameState->pipesUpper[0].w = PIPES_WIDTH;
	gameState->pipesUpper[0].h = PIPES_HEIGHT;

	//The other
	for (i = 1; i < PIPES_NUMBER; i++)
	{
		//Lower pipes
		//Set x
		gameState->pipesLower[i].x = gameState->pipesUpper[i - 1].x + gameState->pipesUpper[i - 1].w + PIPES_X_DISTANCE;

		//Random sign and number for y
		int random_number = rand() % 2, sign = 1;
		if (random_number)
			sign = -1;
		int random_y_offset = (rand() % 100);
		if (random_y_offset < 50)
			random_y_offset = 100 - random_y_offset;
		random_y_offset *= sign;

		//Set y
		gameState->pipesLower[i].y = gameState->pipesLower[i - 1].y + random_y_offset;

		//Set y limits
		if (gameState->pipesLower[i].y > WINDOW_HEIGHT - 150)
			gameState->pipesLower[i].y = gameState->pipesLower[i - 1].y - random_y_offset / 2;
		else if (gameState->pipesLower[i].y < WINDOW_HEIGHT - 350)
			gameState->pipesLower[i].y = gameState->pipesLower[i - 1].y - random_y_offset / 2;

		//Width and length
		gameState->pipesLower[i].w = PIPES_WIDTH;
		gameState->pipesLower[i].h = PIPES_HEIGHT;

		//Upper pipes
		gameState->pipesUpper[i].x = gameState->pipesUpper[i - 1].x + gameState->pipesUpper[i - 1].w + PIPES_X_DISTANCE;
		gameState->pipesUpper[i].y = gameState->pipesLower[i].y - gameState->pipesLower[i].h - PIPES_Y_DISTANCE;
		gameState->pipesUpper[i].w = PIPES_WIDTH;
		gameState->pipesUpper[i].h = PIPES_HEIGHT;
	}
}

void createWindow(GameState* gameState)
{
	gameState->window = SDL_CreateWindow("Game Window", //window title
		SDL_WINDOWPOS_UNDEFINED, //initial x pos
		SDL_WINDOWPOS_UNDEFINED, //initial y pos
		WINDOW_WIDTH, //width, in pixels
		WINDOW_HEIGHT, //height, in pixels
		0 //flags
	);

	// Check that the window was successfully created
	if (gameState->window == NULL)
	{
		printf("Could not create window: %s\n", SDL_GetError());
		exit(1);
	}
}

void createRenderer(GameState* gameState)
{
	gameState->renderer = SDL_CreateRenderer(gameState->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void loadFont(GameState* gameState)
{
	gameState->font = TTF_OpenFont("font.ttf", FONT_SIZE);
	if (!gameState->font)
	{

		printf("%s\n", TTF_GetError);
		exit(1);
	}

	gameState->autismFont = TTF_OpenFont("font.ttf", AUTISM_FONT_SIZE);
	if (!gameState->autismFont)
	{

		printf("%s\n", TTF_GetError);
		exit(1);
	}
}

void createTextures(GameState* gameState)
{
	//Base
	createTexture(&gameState->baseTexture, "base.png", gameState->renderer);

	//Background
	createTexture(&gameState->backgroundTexture, "background-day.png", gameState->renderer);

	//Bird
	createBirdTextures(gameState);

	//Pipes
	createTexture(&gameState->PipeLowerTexture, "png/pipe-green-lower.png", gameState->renderer);
	createTexture(&gameState->PipeUpperTexture, "png/pipe-green-upper.png", gameState->renderer);

	//Gameover
	createTexture(&gameState->gameOverTexture, "gameover.png", gameState->renderer);
}

void loadSounds(GameState* gameState)
{
	//Die sound
	gameState->dieSound = Mix_LoadWAV("audio/die.wav");
	if (!gameState->dieSound)
	{
		printf("Could not open audio/die.wav\n");
		exit(1);
	}

	//Hit sound
	gameState->hitSound = Mix_LoadWAV("audio/hit.wav");
	if (!gameState->hitSound)
	{
		printf("Could not open audio/hit.wav\n");
		exit(1);
	}

	//Point sound
	gameState->pointSound = Mix_LoadWAV("audio/point.wav");
	if (!gameState->pointSound)
	{
		printf("Could not open audio/point.wav\n");
		exit(1);
	}

	//Swoosh sound
	gameState->swooshSound = Mix_LoadWAV("audio/swoosh.wav");
	if (!gameState->swooshSound)
	{
		printf("Could not open audio/swoosh.wav\n");
		exit(1);
	}

	//Wing sound
	gameState->wingSound = Mix_LoadWAV("audio/wing.wav");
	if (!gameState->wingSound)
	{
		printf("Could not open audio/wing.wav\n");
		exit(1);
	}
}

void loadAutisticSounds(GameState* gameState)
{
	//Die sound
	gameState->dieSound = Mix_LoadWAV("audio/zdrang.wav");
	if (!gameState->dieSound)
	{
		printf("Could not open audio/die.wav\n");
		exit(1);
	}

	//Hit sound
	gameState->hitSound = Mix_LoadWAV("audio/zdrang.wav");
	if (!gameState->hitSound)
	{
		printf("Could not open audio/hit.wav\n");
		exit(1);
	}

	//Point sound
	gameState->pointSound = Mix_LoadWAV("audio/pac.wav");
	if (!gameState->pointSound)
	{
		printf("Could not open audio/point.wav\n");
		exit(1);
	}

	//Swoosh sound
	gameState->swooshSound = Mix_LoadWAV("audio/swoosh.wav");
	if (!gameState->swooshSound)
	{
		printf("Could not open audio/swoosh.wav\n");
		exit(1);
	}

	//Wing sound
	gameState->wingSound = Mix_LoadWAV("audio/e.wav");
	if (!gameState->wingSound)
	{
		printf("Could not open audio/wing.wav\n");
		exit(1);
	}
}

void createBirdTextures(GameState* gameState)
{
	int i;
	for (i = 0; i < BIRD_FRAMES_NUMBER; i++)
	{
		//Create bird surface
		char ImageName[30] = "";
		sprintf_s(ImageName, 30, "png/yellowbird%d.png", i);
		createTexture(&gameState->birdTexture[i], ImageName, gameState->renderer);
	}
}

void createTexture(SDL_Texture** texture, char* img_path, SDL_Renderer* renderer)
{
	//Create surface
	SDL_Surface* surface = IMG_Load(img_path);
	if (!surface)
	{
		printf("Cannot find %s\n", img_path);
		SDL_Quit();
		exit(1);
	}

	//Create texture
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void freeMemory(GameState* gameState)
{
	if(gameState->bases)
		free(gameState->bases);
	if(gameState->birdTexture)
		free(gameState->birdTexture);
	if(gameState->pipesLower)
		free(gameState->pipesLower);
	if (gameState->pipesUpper)
		free(gameState->pipesUpper);

	//Delete window
	SDL_DestroyWindow(gameState->window);

	//Delete renderer
	SDL_DestroyRenderer(gameState->renderer);
}

void unloadSounds(GameState* gameState)
{
	if(gameState->dieSound)
		Mix_FreeChunk(gameState->dieSound);
	if (gameState->hitSound)
		Mix_FreeChunk(gameState->hitSound);
	if (gameState->pointSound)
		Mix_FreeChunk(gameState->pointSound);
	if (gameState->swooshSound)
		Mix_FreeChunk(gameState->swooshSound);
	if (gameState->wingSound)
		Mix_FreeChunk(gameState->wingSound);
}

void closeSDL()
{
	SDL_Quit();
	TTF_Quit();
	Mix_CloseAudio();
}

void error()
{
	printf("Could not allocate memory!\n");
	exit(1);
}