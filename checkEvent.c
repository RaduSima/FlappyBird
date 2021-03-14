#include "header.h"

void checkEvent(GameState* gameState)
{
	SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{

				//Keyboard
				case SDL_KEYDOWN:
				{
					switch (event.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							gameState->done = 1;
							break;
						}
						case SDLK_SPACE:
						{
							if (gameState->bird.state == BIRD_ALIVE)
							{
								Mix_PlayChannel(-1, gameState->wingSound, 0);
								gameState->bird.dy -= JUMP_FORCE;
							}
							if (gameState->bird.state == BIRD_LOADING)
							{
								Mix_PlayChannel(-1, gameState->wingSound, 0);
								gameState->bird.dy -= JUMP_FORCE;
								gameState->bird.state = BIRD_ALIVE;
							}
							break;
						}
					}
					break;
				}

				//Mouse
				case SDL_MOUSEBUTTONUP:
				{
					if (event.button.button == SDL_BUTTON_LEFT && gameState->bird.state == BIRD_UNLOADING)
						if (event.button.x > (WINDOW_WIDTH / 2 - 150 / 2) && event.button.x < (WINDOW_WIDTH / 2 - 150 / 2 + 150) && event.button.y >(WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10) && event.button.y < (WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10 + 42))
						{
							declareVariables(gameState);
							Mix_PlayChannel(-1, gameState->swooshSound, 0);
						}

					/*if (event.button.button == SDL_BUTTON_LEFT && gameState->bird.state == BIRD_LOADING)
						if (event.button.x > 20 && event.button.x < (20 + 150) && event.button.y >(WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10) && event.button.y < (WINDOW_HEIGHT - BASE_HEIGHT + BASE_HEIGHT / 2 - 10 + 42))
						{
							if (gameState->autismMode)
							{
								loadSounds(gameState);
								gameState->autismMode = 0;
								break;
							}
							if (!gameState->autismMode)
							{
								loadAutisticSounds(gameState);
								gameState->autismMode = 1;
								break;
							}
						}*/
					break;
				}
				 
				//Exit
				case SDL_QUIT:
				{
					gameState->done = 1;
					break;
				}
			}
		}
}