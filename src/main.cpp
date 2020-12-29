#include "asteroids/game.h"
#include "asteroids/scope_guard.h"
#include "asteroids/world.h"

#include <SDL2/SDL.h>
#include <SDL_mixer.h>

#include <filesystem>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    SDL_Log("SDL init error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SCOPE_EXIT{ SDL_Quit(); };

  int flags = 0;
  int initted = Mix_Init(flags);
  if ((initted & flags) != flags) {
    SDL_Log("Mix_Init: Failed to init!\n");
    SDL_Log("Mix_Init: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }
  SCOPE_EXIT{ Mix_Quit(); };

  if(Mix_OpenAudio(11025, AUDIO_S16SYS, 2, 1024) == -1) {
    SDL_Log("Mix_OpenAudio: %s\n", Mix_GetError());
    return EXIT_FAILURE;
  }
  SCOPE_EXIT{ Mix_CloseAudio(); };

  SDL_Window *window = SDL_CreateWindow("Asteroids",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WORLD_WIDTH, WORLD_HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) {
    SDL_Log("SDL window creation error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SCOPE_EXIT{ SDL_DestroyWindow(window); };

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_Log("SDL renderer creation error: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }
  SCOPE_EXIT{ SDL_DestroyRenderer(renderer); };

  game g{ std::filesystem::current_path() };
  g.run(renderer);

  /*FILE *fps_file = fopen("./fps.out", "w");
  if (!fps_file) {
    SDL_Log("Failed to open ./fps.out for writing!");
  }*/
  //if (fps_file) fclose(fps_file);

  return EXIT_SUCCESS;
}
