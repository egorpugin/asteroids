#pragma once

#include "sound.h"

#include <entt/entt.hpp>

struct SDL_Renderer;

/// Only object that is controlled by the player is space ship. We consider
/// input events to be "sticky". Ship fires all time while key is down.
struct input_events {
    bool ship_left = false;
    bool ship_right = false;
    bool ship_thrust = false;
    bool ship_fire = false;
};

struct game {
  entt::registry registry;
  input_events events;
  sound_resources sounds;

  game(const std::filesystem::path &root);

  void run(SDL_Renderer *renderer);

private:
  void step(float dt);
  void render(SDL_Renderer *renderer);
};
