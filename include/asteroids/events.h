/**
  User input events that are passed to simulation.
*/
#ifndef ASTEROIDS_EVENTS_H
#define ASTEROIDS_EVENTS_H

/// Only object that is controlled by the player is space ship. We consider
/// input events to be "sticky". Ship fires all time while key is down.
struct input_events {
  bool ship_left = false;
  bool ship_right = false;
  bool ship_thrust = false;
  bool ship_fire = false;
};

#endif /* ASTEROIDS_EVENTS_H */
