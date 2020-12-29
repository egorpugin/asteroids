/**
  Module that defines player component of an entity in the game.
*/
#ifndef ASTEROIDS_PLAYER_H
#define ASTEROIDS_PLAYER_H

/// Mass of player ship in kg
#define PLAYER_MASS 100000.f
/// Visual X size of ship in meters
#define PLAYER_RENDER_WIDTH 30
/// Visual Y size of ship in meters
#define PLAYER_RENDER_HEIGHT 25
/// Collision radius for player ship in meters
#define PLAYER_COLLIDE_RADIUS 15.f
/// Force of engine in newtons
#define PLAYER_THRUST 20000000
/// Recharge time for bullets in seconds
#define PLAYER_FIRE_COOLDOWN 0.3
/// Rotation speed of ship in radians/seconds
#define PLAYER_ROTATION_SPEED M_PI

#endif /* ASTEROIDS_PLAYER_H */
