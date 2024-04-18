#pragma once

namespace Anterle {
class Time
{
public:
  static void update();

  static double getDeltaTime();
  static double getLastFrame();
  static double getCurentFrame();

public:
  static double FixedTimeStep;
  static double FixedAccumulator;

private:
  static double _deltaTime;
  static double _lastFrame;
  static double _currentFrame;
  static double _currentTime;
};
}// namespace Anterle
