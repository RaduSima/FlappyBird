#include "header.h"

void process(GameState* gameState)
{
	int i;

	updateScore(gameState);

	updateBasesPosition(gameState);

	updatePipesPosition(gameState);

	updateBirdPosition(gameState);

	collision(gameState);
}               

void updateScore(GameState* gameState)
{
	if (gameState->bird.x > gameState->pipesLower[gameState->pipes_score_counter].x)
	{
		gameState->score++;
		gameState->pipes_score_counter++;
		Mix_PlayChannel(-1, gameState->pointSound, 0);
	}
	if (gameState->pipes_score_counter == PIPES_NUMBER)
		gameState->pipes_score_counter = 0;
}

void updateBasesPosition(GameState* gameState)
{
	int i;
	if (gameState->bird.state == BIRD_ALIVE || gameState->bird.state == BIRD_LOADING)
		for (i = 0; i < BASES_NUMBER; i++)
			gameState->bases[i].x -= gameState->bird.dx;

	if (gameState->bases[gameState->bases_counter].x + gameState->bases[gameState->bases_counter].w < 0)
	{
		if (!gameState->bases_counter)
			gameState->bases[gameState->bases_counter].x = gameState->bases[BASES_NUMBER - 1].x + gameState->bases[BASES_NUMBER - 1].w;
		else
			gameState->bases[gameState->bases_counter].x = gameState->bases[gameState->bases_counter - 1].x + gameState->bases[gameState->bases_counter - 1].w;
		gameState->bases_counter++;
		if (gameState->bases_counter == BASES_NUMBER)
			gameState->bases_counter = 0;
	}
}

void updatePipesPosition(GameState* gameState)
{
	int i;

	//Move pipes to left
	if (gameState->bird.state == BIRD_ALIVE)
		for (i = 0; i < PIPES_NUMBER; i++)
		{
			gameState->pipesLower[i].x -= gameState->bird.dx;
			gameState->pipesUpper[i].x -= gameState->bird.dx;
		}

	//Reroll pipes
	if (gameState->pipesLower[gameState->pipes_counter].x + gameState->pipesLower[gameState->pipes_counter].w < 0)
	{
		//Lower pipes
		if (!gameState->pipes_counter)
			gameState->pipesLower[gameState->pipes_counter].x = gameState->pipesLower[PIPES_NUMBER - 1].x + gameState->pipesLower[PIPES_NUMBER - 1].w + PIPES_X_DISTANCE;
		else
			gameState->pipesLower[gameState->pipes_counter].x = gameState->pipesLower[gameState->pipes_counter - 1].x + gameState->pipesLower[gameState->pipes_counter - 1].w + PIPES_X_DISTANCE;

		//Upper pipes
		if (!gameState->pipes_counter)
			gameState->pipesUpper[gameState->pipes_counter].x = gameState->pipesUpper[PIPES_NUMBER - 1].x + gameState->pipesUpper[PIPES_NUMBER - 1].w + PIPES_X_DISTANCE;
		else
			gameState->pipesUpper[gameState->pipes_counter].x = gameState->pipesUpper[gameState->pipes_counter - 1].x + gameState->pipesUpper[gameState->pipes_counter - 1].w + PIPES_X_DISTANCE;

		//Reset pipes counter
		gameState->pipes_counter++;
		if (gameState->pipes_counter == PIPES_NUMBER)
			gameState->pipes_counter = 0;
	}
}

void updateBirdPosition(GameState* gameState)
{
	if (gameState->bird.state != BIRD_LOADING)
	{
		//Gravity
		gameState->bird.dy += GRAVITY;
		if (gameState->bird.dy > MAX_GRAVITY)
			gameState->bird.dy = MAX_GRAVITY;
		if (gameState->bird.dy < MAX_JUMP)
			gameState->bird.dy = MAX_JUMP;

		//Update bird position
		gameState->bird.y += gameState->bird.dy;

		if ((gameState->bird.state == BIRD_DEAD || gameState->bird.state == BIRD_UNLOADING) && gameState->bird.y > WINDOW_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT)
		{
			gameState->bird.y = WINDOW_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT;
			gameState->bird.state = BIRD_UNLOADING;
		}

		//Update angle
		if (gameState->bird.dy >= 0)
			gameState->bird.angle += gameState->bird.dy;
		else
			gameState->bird.angle += gameState->bird.dy * 2;

		if (gameState->bird.angle > 90)
			gameState->bird.angle = 90;
		if (gameState->bird.angle < -20)
			gameState->bird.angle = -20;
	}
}

void collision(GameState* gameState)
{
	int i, flag = 0;
	if (gameState->bird.state == BIRD_ALIVE)
	{
		//Collision with pipes
		for (i = 0; i < PIPES_NUMBER; i++)
		{
			float bird_x = gameState->bird.x, bird_y = gameState->bird.y, bird_w = gameState->bird.w - 5, bird_h = gameState->bird.h - 5;
			float pipeLower_x = gameState->pipesLower[i].x, pipeLower_y = gameState->pipesLower[i].y, pipeLower_w = gameState->pipesLower[i].w, pipeLower_h = gameState->pipesLower[i].h;
			float pipeUpper_x = gameState->pipesUpper[i].x, pipeUpper_y = gameState->pipesUpper[i].y, pipeUpper_w = gameState->pipesUpper[i].w, pipeUpper_h = gameState->pipesUpper[i].h;

			//Lower pipes
			if (!((bird_x > (pipeLower_x + pipeLower_w)) || (pipeLower_x > bird_x + bird_w) || (bird_y > (pipeLower_y + pipeLower_h)) || (pipeLower_y > (bird_y + bird_h))))
			{
				gameState->bird.state = BIRD_DEAD;
				flag = 1;
			}

			//Upper pipes
			if (!((bird_x > (pipeUpper_x + pipeUpper_w)) || (pipeUpper_x > bird_x + bird_w) || (bird_y > (pipeUpper_y + pipeUpper_h)) || (pipeUpper_y > (bird_y + bird_h))))
			{
				gameState->bird.state = BIRD_DEAD;
				flag = 1;
			}
		}

		//Above the pipes......
		if (gameState->bird.y < 0)
		{
			gameState->bird.state = BIRD_DEAD;
			flag = 1;
		}

		//Collision with bases
		if (gameState->bird.y > WINDOW_HEIGHT - BASE_HEIGHT - BIRD_HEIGHT + 5)
		{
			gameState->bird.state = BIRD_DEAD;
			flag = 1;
		}
	}

	if (flag)
	{
		Mix_PlayChannel(-1, gameState->hitSound, 0);
		Mix_PlayChannel(-1, gameState->dieSound, 0);
		updateBestScore(gameState);
	}
}

void updateBestScore(GameState* gameState)
{
	int flag = 0;
	FILE* scoreFile;

	//Creates file, if it does not exist
	scoreFile = fopen("scoreFile.txt", "a");
	if (!scoreFile)
	{
		printf("Could not open scoreFile.\n");
		exit(1);
	}
	fseek(scoreFile, 0, SEEK_END); // go to end of file
	if (ftell(scoreFile) == 0)
		fprintf(scoreFile, "%d", 0);
	fclose(scoreFile);

	//Reads best score from file
	scoreFile = fopen("scoreFile.txt", "r");
	if (!scoreFile)
	{
		printf("Could not open scoreFile.\n");
		exit(1);
	}
	fscanf(scoreFile, "%d", &gameState->bestScore);
	if (gameState->score > gameState->bestScore)
	{
		flag = 1;
		gameState->bestScore = gameState->score;
		gameState->newBestScoreFlag = 1;
	}
	fclose(scoreFile);

	//Updates best score in file
	if (flag)
	{
		scoreFile = fopen("scoreFile.txt", "w");
		if (!scoreFile)
		{
			printf("Could not open scoreFile.\n");
			exit(1);
		}
		fprintf(scoreFile, "%d", gameState->bestScore);
		fclose(scoreFile);
	}
}

void resetTimer(GameState* gameState)
{
	gameState->timer.start_ticks = SDL_GetTicks();
	gameState->timer.elapsed_ticks = 0;
	gameState->timer.delta_time = 0.0f;
}

void updateTimer(GameState* gameState)
{
	gameState->timer.elapsed_ticks = SDL_GetTicks() - gameState->timer.start_ticks;
	gameState->timer.delta_time = gameState->timer.elapsed_ticks * 0.001f;
}