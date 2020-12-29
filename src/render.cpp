#include "asteroids/render.h"

#include "asteroids/component/asteroid.h"
#include "asteroids/component/player.h"

#include <entt/entt.hpp>

#define PLAYER_DX (0.5 * PLAYER_RENDER_WIDTH)
#define PLAYER_DY (0.5 * PLAYER_RENDER_HEIGHT)

struct SDL_Point lines_i[2 * ASTEROID_EDGES_MAX];
struct v2f lines_f[2 * ASTEROID_EDGES_MAX];

namespace {

void render_line(SDL_Renderer *renderer, struct v2f pos, float rotation, struct v2f p1, struct v2f p2) {
  p1.rotate(rotation);
  p2.rotate(rotation);
  SDL_RenderDrawLine(renderer, pos.x + p1.x, pos.y + p1.y
    , pos.x + p2.x, pos.y + p2.y);
}

void render_lines(SDL_Renderer *renderer, struct v2f pos, float rotation, const struct v2f *ps, int n) {
  for (int i = 0; i < n; i++) {
    struct v2f p = ps[i];
    p.rotate(rotation);
    p += pos;
    lines_i[i].x = (int)p.x;
    lines_i[i].y = (int)p.y;
  }
  SDL_RenderDrawLines(renderer, (SDL_Point *)&lines_i, n);
}

v2f circloid_point(int i, int n, float r) {
  float a = (float)i * (2 * M_PI / (float)n);
  return {
      r * (1.f + (float)sin(a) * 0.3f) * (float)cos(a)
    , r * (1.f + (float)cos(a) * 0.3f) * (float)sin(a)
  };
}

void render_circloid(SDL_Renderer *renderer, int n, float a0, struct v2f pos, float r) {
  for (int i = 0; i <= n - 1; i++) {
    lines_f[2 * i] = circloid_point(i, n, r);
    lines_f[2 * i + 1] = circloid_point(i + 1, n, r);
  }
  render_lines(renderer, pos, a0, (struct v2f *)&lines_f, 2 * n);
}

void render_asteroid(SDL_Renderer *renderer, const asteroid &asteroid, const position &pos, float rotation, float radius) {
  render_circloid(renderer, asteroid.edges, rotation, pos, radius);
}

void render_player(SDL_Renderer *renderer, player &player, struct v2f pos, float rotation) {
  render_line(renderer, pos, rotation, { PLAYER_DX,  0 }
  , { -PLAYER_DX, -PLAYER_DY });
  render_line(renderer, pos, rotation, { PLAYER_DX,  0 }
  , { -PLAYER_DX,  PLAYER_DY });
  render_line(renderer, pos, rotation, { -PLAYER_DX, -PLAYER_DY }
  , { -PLAYER_DX,  PLAYER_DY });
  if (player.thrust) {
    render_line(renderer, pos, rotation, { -PLAYER_DX   , -0.5 * PLAYER_DY }
    , { -PLAYER_DX - 10,  0 });
    render_line(renderer, pos, rotation, { -PLAYER_DX   ,  0.5 * PLAYER_DY }
    , { -PLAYER_DX - 10,  0 });
  }
}

void render_bullet(SDL_Renderer *renderer, struct v2f pos) {
  SDL_RenderDrawPoint(renderer, pos.x, pos.y);
}

} // namespace

void render(SDL_Renderer *renderer, entt::registry &registry) {
  auto player_view = registry.view<player, position, rotation>();
  for (auto entity : player_view) {
    auto &pl = player_view.get<player>(entity);
    auto &pos = player_view.get<position>(entity);
    auto &rot = player_view.get<rotation>(entity);
    render_player(renderer, pl, pos, rot.x);
  }

  auto asteroid_view = registry.view<asteroid, position, rotation, radius>();
  for (auto entity : asteroid_view) {
    auto &a = asteroid_view.get<asteroid>(entity);
    auto &pos = asteroid_view.get<position>(entity);
    auto &rot = asteroid_view.get<rotation>(entity);
    auto &rad = asteroid_view.get<radius>(entity);
    render_asteroid(renderer, a, pos, rot.x, rad.x);
  }

  auto bullet_view = registry.view<bullet, position>();
  for (auto entity : bullet_view) {
    auto &pos = bullet_view.get<position>(entity);
    render_bullet(renderer, pos);
  }
}
