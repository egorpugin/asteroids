#pragma once

#include "v2.h"

#include <SDL2/SDL_stdinc.h>

#define WORLD_WIDTH 1480
#define WORLD_HEIGHT 1024

struct position : v2f {};
struct velocity : v2f {};
struct rotation { float v; };
struct mass { float v; };
struct radius { float v; };

struct player {
  static inline const auto mass = 100000.f;
  static inline const auto render_width = 30;
  static inline const auto render_height = 25;
  static inline const auto collide_radius = 15.f;
  static inline const auto thrust_value = 20000000;
  static inline const auto fire_cooldown_max = 0.3f;
  static inline const auto rotation_speed = M_PI;

  /// Whether player ship is accelerating at the moment
  bool thrust;
  /// Counts until moment when player can emit a new bullet
  float fire_cooldown;
};

struct asteroid {
  static inline const auto edges_min = 8;
  static inline const auto edges_max = 20;
  static inline const auto size_min = 10;
  static inline const auto size_max = 130;
  static inline const auto density = 1;
  static inline const auto amount = 20;
  static inline const auto velocity_min = -100;
  static inline const auto velocity_max = 100;

  /// Amount of edges for rendering
  int edges;
};

struct bullet {
  static inline const auto speed = 200.f;
  static inline const auto max_lifetime = 2.f;
  static inline const auto radius = 1.f;

  /// Amount of time left until despawn
  float life_time;
};
