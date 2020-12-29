#ifndef ASTEROIDS_GAME_H
#define ASTEROIDS_GAME_H

#include "events.h"
#include "sound.h"

#include <entt/entt.hpp>

struct SDL_Renderer;

struct game {
  entt::registry registry;
  input_events events;
  sound_resources sounds;
  SDL_Renderer *renderer;

  game(const std::filesystem::path &root, SDL_Renderer &renderer);

  void run();

private:
  void step(float dt);
};

#endif
