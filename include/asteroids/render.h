/**
  Code to render player, asteroids and bullets.
*/
#ifndef ASTEROIDS_RENDER_H
#define ASTEROIDS_RENDER_H

#include "asteroids/v2.h"

#include <entt/fwd.hpp>
#include <SDL2/SDL.h>

struct position : v2f {};
struct velocity : v2f {};
struct rotation { float x; };
struct mass { float x; };
struct radius { float x; };

struct player {
  /// Whether player ship is accelerating at the moment
  bool thrust = false;
  /// Counts until moment when player can emit a new bullet
  float fire_cooldown = 0;
};

struct asteroid {
  /// Amount of edges for rendering
  int edges;
};

struct bullet {
  /// Amount of time left until despawn
  float life_time;
};

void render(SDL_Renderer *renderer, entt::registry &);

#endif /* ASTEROIDS_RENDER_H */
