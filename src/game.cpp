#include <asteroids/game.h>

#include "asteroids/sound.h"
#include "asteroids/world.h"

#include <SDL2/SDL.h>

namespace {

bool process_events(input_events &events) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      return true;
    }
    else if (e.type == SDL_KEYDOWN) {
      if (e.key.keysym.sym == SDLK_ESCAPE) {
        return true;
      }
      else if (e.key.keysym.sym == SDLK_UP) {
        events.ship_thrust = true;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_LEFT) {
        events.ship_left = true;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_RIGHT) {
        events.ship_right = true;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_SPACE) {
        events.ship_fire = true;
        return false;
      }
    }
    else if (e.type == SDL_KEYUP) {
      if (e.key.keysym.sym == SDLK_UP) {
        events.ship_thrust = false;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_LEFT) {
        events.ship_left = false;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_RIGHT) {
        events.ship_right = false;
        return false;
      }
      else if (e.key.keysym.sym == SDLK_SPACE) {
        events.ship_fire = false;
        return false;
      }
    }
  }
  return false;
}

int randi(int min, int max) {
  return min + (int)((float)rand() / (RAND_MAX / (float)(max - min)));
}

float randf(float min, float max) {
  return min + (float)rand() / (float)(RAND_MAX / (max - min));
}

velocity random_asteroid_velocity(const velocity &v) {
  return { v.x + randf(ASTEROID_VELOCITY_MIN, ASTEROID_VELOCITY_MAX), v.y + randf(ASTEROID_VELOCITY_MIN, ASTEROID_VELOCITY_MAX) };
}

void spawn_player(entt::registry &registry) {
  const auto entity = registry.create();
  registry.emplace<player>(entity);
  registry.emplace<position>(entity, WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f);
  registry.emplace<velocity>(entity, 0.f, 0.f);
  registry.emplace<rotation>(entity, 0.f);
  registry.emplace<mass>(entity, PLAYER_MASS);
  registry.emplace<radius>(entity, PLAYER_COLLIDE_RADIUS);
}

void spawn_asteroid(entt::registry &registry, const position &pos, const velocity &vel, mass m, radius r) {
  const auto entity = registry.create();
  registry.emplace<asteroid>(entity, randi(ASTEROID_EDGES_MIN, ASTEROID_EDGES_MAX));
  registry.emplace<position>(entity, pos);
  registry.emplace<velocity>(entity, random_asteroid_velocity(vel));
  registry.emplace<rotation>(entity, randf(0, 2 * M_PI));
  registry.emplace<mass>(entity, m);
  registry.emplace<radius>(entity, r);
}

void spawn(entt::registry &registry) {
  spawn_player(registry);
  for (int i = 0; i < ASTEROID_AMOUNT; i++) {
    float radius_ = randf(ASTEROID_SIZE_MIN, ASTEROID_SIZE_MAX);
    spawn_asteroid(registry, {randf(0, WORLD_WIDTH), randf(0, WORLD_HEIGHT)}, { 0, 0 }
      , { ASTEROID_DENSITY * (float)M_PI * radius_ * radius_ }, { radius_ });
  }
}

} // namespace

game::game(const std::filesystem::path &root)
  : sounds{root / "sounds"} {
}

void game::run(SDL_Renderer *renderer) {
  spawn(registry);

  float dt = 0;
  clock_t lastTick = 0;
  int i = 0;
  while (true) {
    bool quit = process_events(events);
    if (quit) break;

    clock_t currentTick = clock();
    dt = (float)(currentTick - lastTick) / CLOCKS_PER_SEC;
    lastTick = currentTick;

    step(dt);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    render(renderer);
    SDL_RenderPresent(renderer);

    i += 1;
    float fps = 1 / dt;
    if (i % 2000 == 0) {
      SDL_Log("FPS: %f", fps);
    }
    clock_t ptick1 = clock();
    /*if (i % 1000 == 0 && fps_file) {
      fprintf(fps_file, "%i,%f\n", i, fps);
    }*/
    clock_t ptick2 = clock();
    lastTick += ptick2 - ptick1;
  }
}

void game::step(float dt) {
  sounds.update_sound_cooldowns(dt);

  auto player_view = registry.view<player, position, velocity, rotation, mass>();
  player_view.each([this, &dt](auto &player, auto &pos, auto &vel, auto &rot, auto &mass) mutable {
    // update_player
    if (player.thrust) {
      float acc = dt * PLAYER_THRUST / mass.x;
      vel.x += cos(rot.x) * acc;
      vel.y += sin(rot.x) * acc;
    }
    player.thrust = false;
    if (player.fire_cooldown > 0) {
      player.fire_cooldown -= dt;
    }

    // apply_events
    if (events.ship_left) {
      rot.x -= PLAYER_ROTATION_SPEED * dt;
    }
    if (events.ship_right) {
      rot.x += PLAYER_ROTATION_SPEED * dt;
    }
    if (events.ship_thrust) {
      player.thrust = true;
      sounds.play_sound(sounds.thrust, 0, 0.35);
    }
    if (events.ship_fire) {
      if (player.fire_cooldown <= 0) {
        sounds.play_sound(sounds.fire, 1, 0.2);

        auto bpos = pos;
        v2f bvel{ 1, 0 };
        bvel.rotate(rot.x);
        bvel.scale(BULLET_SPEED);
        bvel += vel;

        // spawn_bullet
        const auto entity = registry.create();
        registry.emplace<bullet>(entity, BULLET_LIFE_TIME);
        registry.emplace<position>(entity, bpos);
        registry.emplace<velocity>(entity, bvel);
        registry.emplace<radius>(entity, BULLET_RADIUS);

        player.fire_cooldown = PLAYER_FIRE_COOLDOWN;
      }
    }
  });

  auto bullet_view = registry.view<bullet>();
  for (auto entity : bullet_view) {
    auto &b = bullet_view.get<bullet>(entity);
    b.life_time -= dt;
    if (b.life_time < 0) {
      registry.destroy(entity);
    }
  }

  // movement and positions
  auto mv_view = registry.view<position>();
  for (auto entity : mv_view) {
    auto &p = mv_view.get<position>(entity);
    if (registry.has<velocity>(entity)) {
      auto &v = registry.get<velocity>(entity);
      p.x += v.x * dt;
      p.y += v.y * dt;
    }
    if (p.x < 0) p.x += WORLD_WIDTH;
    if (p.x > WORLD_WIDTH) p.x -= WORLD_WIDTH;
    if (p.y < 0) p.y += WORLD_HEIGHT;
    if (p.y > WORLD_HEIGHT) p.y -= WORLD_HEIGHT;
  }

  // collisions
  auto col_pl = registry.view<player, position, radius>();
  auto col_a = registry.view<asteroid, position, radius>();
  auto col_b = registry.view<bullet, position, radius>();
  // with player
  for (auto e1 : col_pl) {
    auto &p1 = registry.get<position>(e1);
    auto r1 = registry.get<radius>(e1).x;
    bool collide = false;
    for (auto e2 : col_a) {
      const auto &p2 = registry.get<position>(e2);
      auto r2 = registry.get<radius>(e2).x;
      float distsq = p1.dist_squared(p2);
      float r = r1 + r2;
      if (distsq < r * r) {
        // respawn player
        sounds.play_sound(sounds.bang_medium, 2, 0.3);
        p1 = { WORLD_WIDTH * 0.5f, WORLD_HEIGHT * 0.5f };
        registry.get<velocity>(e1) = { 0.f, 0.f };
        registry.get<rotation>(e1) = { 0.f };
        collide = true;
      }
    }
    if (collide)
      break;
  }
  // with bullet
  for (auto e1 : col_b) {
    const auto &p1 = registry.get<position>(e1);
    auto r1 = registry.get<radius>(e1).x;
    bool collide = false;
    for (auto e2 : col_a) {
      const auto &p2 = registry.get<position>(e2);
      auto r2 = registry.get<radius>(e2).x;
      float distsq = p1.dist_squared(p2);
      float r = r1 + r2;
      if (distsq < r * r) {
        // break asteroid
        sounds.play_sound(sounds.bang_medium, 2, 0.2);
        if (registry.valid(e1))
          registry.destroy(e1);
        // spawn asteroid cracks
        float r = 0.5 * r2;
        if (r >= ASTEROID_SIZE_MIN) {
          auto &pos = p2;
          auto &vel = registry.get<velocity>(e2);
          float m = ASTEROID_DENSITY * M_PI * r * r;
          spawn_asteroid(registry, pos, vel, { m }, { r });
          spawn_asteroid(registry, pos, vel, { m }, { r });
        }
        registry.destroy(e2);
        collide = true;
      }
    }
    if (collide)
      break;
  }
}
