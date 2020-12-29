/**
  Handle sound states and cooldowns.
*/
#ifndef ASTEROIDS_SOUND_H
#define ASTEROIDS_SOUND_H

#include "SDL_mixer.h"

#include <filesystem>
#include <memory>

struct sound_resource {
  std::unique_ptr<Mix_Chunk, void(*)(Mix_Chunk*)> r;

  sound_resource(const std::filesystem::path &);
};

struct sound_resources {
  static inline const int channel_count = 3;

  sound_resource bang_medium;
  sound_resource fire;
  sound_resource thrust;
  float cooldowns[channel_count] = { 0 };

  sound_resources(const std::filesystem::path &);

  /// Decrease cooldowns for playing sound again
  void update_sound_cooldowns(float dt);

  /// Play sound at given channel and setup cooldown
  void play_sound(const sound_resource &sound, int channel, float cooldown);
};

#endif /* ASTEROIDS_SOUND_H */
