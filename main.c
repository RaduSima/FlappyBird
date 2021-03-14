#include "header.h"

int main(int argc, char* argv[])
{

	GameState gameState;
	srand(time(0));

	//Start the game
	loadGame(&gameState);

	while (!gameState.done)
	{
		float frame_delay = 1000.0f / FRAME_RATE;
		int frame_start = SDL_GetTicks(), frame_time = 0;

		checkEvent(&gameState);
		process(&gameState);
		render(&gameState);
		gameState.timer.frame_count++;

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_delay > frame_time)
			SDL_Delay(frame_delay - frame_time);
	}

	closeGame(&gameState);

	return 0;
}
