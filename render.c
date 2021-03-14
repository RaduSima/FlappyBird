#include "header.h"

void render(GameState* gameState)
{
	renderBackground(gameState);

	renderPipes(gameState);

	renderBird(gameState);

	renderBases(gameState);

	renderScore(gameState);

	renderGameOver(gameState);

	renderOkButton(gameState);

	//renderAutismButton(gameState);

	renderNewBestScore(gameState);

	SDL_RenderPresent(gameState->renderer);
}

void renderBackground(GameState* gameState)
{
	SDL_Rect backgroundRect1 = { gameState->background.x, gameState->background.y, gameState->background.w, gameState->background.h };
	SDL_Rect backgroundRect2 = { gameState->background.w, gameState->background.y, gameState->background.w, gameState->background.h };
	SDL_RenderCopy(gameState->renderer, gameState->backgroundTexture, NULL, &backgroundRect1);
	SDL_RenderCopy(gameState->renderer, gameState->backgroundTexture, NULL, &backgroundRect2);
}

void renderPipes(GameState* gameState)
{
	int i;

	for (i = 0; i < PIPES_NUMBER; i++)
	{
		//Lower pipes
		SDL_Rect LowerPipeRect = { gameState->pipesLower[i].x, gameState->pipesLower[i].y, gameState->pipesLower[i].w, gameState->pipesLower[i].h };
		SDL_RenderCopy(gameState->renderer, gameState->PipeLowerTexture, NULL, &LowerPipeRect);

		//Upper pipes
		SDL_Rect UpperPipeRect = { gameState->pipesUpper[i].x, gameState->pipesUpper[i].y, gameState->pipesUpper[i].w, gameState->pipesUpper[i].h };
		SDL_RenderCopy(gameState->renderer, gameState->PipeUpperTexture, NULL, &UpperPipeRect);
	}
}

void renderBird(GameState* gameState)
{
	SDL_Rect birdRect = { gameState->bird.x, gameState->bird.y, gameState->bird.w, gameState->bird.h };
	SDL_RenderCopyEx(gameState->renderer, gameState->birdTexture[gameState->birdTextureCounter], NULL, &birdRect, gameState->bird.angle, NULL, SDL_FLIP_NONE);

	if (gameState->timer.frame_count % 3 == 0)
		gameState->birdTextureCounter++;
	if (gameState->birdTextureCounter > 2)
		gameState->birdTextureCounter = 0;
}

void renderBases(GameState* gameState)
{
	int i;

	for (i = 0; i < BASES_NUMBER; i++)
	{
		SDL_Rect baseRect = { gameState->bases[i].x, gameState->bases[i].y, gameState->bases[i].w, gameState->bases[i].h };
		SDL_RenderCopy(gameState->renderer, gameState->baseTexture, NULL, &baseRect);
	}
}

void renderScore(GameState* gameState)
{
	//Create score string
	char scoreString[100] = "";
	sprintf_s(scoreString, 100, "%d", gameState->score);

	//Create label surface
	SDL_Color white = { 255, 255, 255, 255 };
	gameState->surface = TTF_RenderText_Blended(gameState->font, scoreString, white);
	gameState->labelWidth = gameState->surface->w;
	gameState->labelHeight = gameState->surface->h;

	//Create label texture
	gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

	//Draw the text
	SDL_Rect textRect = { (WINDOW_WIDTH - gameState->labelWidth) / 2, 20, gameState->labelWidth, gameState->labelHeight };
	SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
}

void renderGameOver(GameState* gameState)
{
	if (gameState->bird.state == BIRD_UNLOADING)
	{
		//Gameover image on the top part of the screen
		SDL_Rect gameOverRect = { WINDOW_WIDTH / 2 - 100, 70, 192, 42 };
		SDL_RenderCopy(gameState->renderer, gameState->gameOverTexture, NULL, &gameOverRect);

		//Score at the bottom left of the screen
		//Create score string
		char scoreString[100] = "";
		sprintf_s(scoreString, 100, "Score: %d", gameState->score);

		//Create label surface
		SDL_Color color = { 0, 0, 0, 0 };
		gameState->surface = TTF_RenderText_Blended(gameState->font, scoreString, color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect = { 10, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2, gameState->labelWidth - 90, gameState->labelHeight - 10 };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);

		//Best score at the bottom right of the screen
		//Create best score string
		sprintf_s(scoreString, 100, "Best: %d", gameState->bestScore);

		//Create label surface
		gameState->surface = TTF_RenderText_Blended(gameState->font, scoreString, color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;
		gameState->bestScoreLabelWidth = gameState->labelWidth;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect2 = { WINDOW_WIDTH - gameState->labelWidth + 90 - 10, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2, gameState->labelWidth - 90, gameState->labelHeight - 10 };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect2);
	}
}

void renderOkButton(GameState* gameState)
{
	if (gameState->bird.state == BIRD_UNLOADING)
	{
		//Create orangle rectangle
		SDL_Rect okRect = { WINDOW_WIDTH / 2 - 150 / 2, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10, 150, 42 };
		SDL_SetRenderDrawColor(gameState->renderer, 255, 165, 0, 255);
		SDL_RenderFillRect(gameState->renderer, &okRect);

		//Create label surface
		SDL_Color color = { 0, 0, 0, 0 };
		gameState->surface = TTF_RenderText_Blended(gameState->font, "Again", color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect = { 252, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 8, gameState->labelWidth - 25, gameState->labelHeight };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
	}
}

void renderAutismButton(GameState* gameState)
{
	if (gameState->bird.state == BIRD_LOADING)
	{
		//Create orangle rectangle
		SDL_Rect okRect = { 20, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10, 150, 42 };
		SDL_SetRenderDrawColor(gameState->renderer, 255, 165, 0, 255);
		SDL_RenderFillRect(gameState->renderer, &okRect);

			//1
		//Create label surface
		SDL_Color color = { 0, 0, 0, 0 };
		gameState->surface = TTF_RenderText_Blended(gameState->autismFont, "Cringe mode:", color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect1 = { 30, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 5, gameState->labelWidth, gameState->labelHeight };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect1);

			//2
		//Create label surface
		if(gameState->autismMode)
			gameState->surface = TTF_RenderText_Blended(gameState->autismFont, "ENABLED", color);
		else
			gameState->surface = TTF_RenderText_Blended(gameState->autismFont, "DISABLED", color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect2 = { 45, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 8 + 20, gameState->labelWidth, gameState->labelHeight };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect2);
	}
}

void renderNewBestScore(GameState* gameState)
{
	if (gameState->newBestScoreFlag && gameState->bird.state == BIRD_UNLOADING)
	{
		//Create label surface
		SDL_Color color = { 255, 0, 0, 255 };
		gameState->surface = TTF_RenderText_Blended(gameState->font, "NEW", color);
		gameState->labelWidth = gameState->surface->w;
		gameState->labelHeight = gameState->surface->h;

		//Create label texture
		gameState->label = SDL_CreateTextureFromSurface(gameState->renderer, gameState->surface);

		//Draw the text
		SDL_Rect textRect = { WINDOW_WIDTH - gameState->bestScoreLabelWidth + 90 - 10, WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 20, gameState->labelWidth - 50, gameState->labelHeight - 20 };
		SDL_RenderCopy(gameState->renderer, gameState->label, NULL, &textRect);
	}
}