#pragma once

#include <cstdint>
#include <vector>

#include "audio/auidio_server.hpp"
#include "object/game_level.hpp"
#include "object/particle_generator.hpp"
#include "render/sprite_renderer.hpp"
#include "render/text_renderer.hpp"

namespace Anterle {
enum class GameState { GAME_ACTIVE, GAME_MENU, GAME_WIN };

class Game
{
public:
  GameState State;
  uint16_t Width, Height;

  std::vector<GameLevel> Levels;

  bool Keys[1024];
  bool KeysProcessed[1024];

  Game(uint16_t width, uint16_t height);
  virtual ~Game();

  virtual void Init();
  virtual void Update(double dt);
  virtual void ProcessInput(double dt);
  virtual void Render();
  virtual void ResetLevel();

protected:
  SpriteRenderer *Renderer;
  ParticleGenerator *Particles;
  TextRenderer *Text;
  AudioSystem *Audio;
};
}// namespace Anterle
