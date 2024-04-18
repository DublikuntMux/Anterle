#include <SDL.h>

#include "resource/time.hpp"

namespace Anterle {
double Time::FixedTimeStep = 1.0 / 60.0;
double Time::_deltaTime = 0.0;
double Time::_lastFrame = 0.0;
double Time::_currentFrame = 0.0;
double Time::FixedAccumulator = 0.0;
double Time::_currentTime = SDL_GetTicks() / 1000.0;

void Time::update()
{
  _currentFrame = SDL_GetTicks() / 1000.0;
  _deltaTime = _currentFrame - _lastFrame;
  _lastFrame = _currentFrame;

  double frameTime = _currentFrame - _currentTime;
  _currentTime = _currentFrame;

  FixedAccumulator += frameTime;
}

double Time::getDeltaTime() { return _deltaTime; }

double Time::getLastFrame() { return _lastFrame; }

double Time::getCurentFrame() { return _currentFrame; }
}// namespace Anterle
