#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include "game_object.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

struct Particle {
  glm::vec2 Position, Velocity;
  glm::vec4 Color;
  float Life;

  Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) {}
};

class ParticleGenerator {
public:
  ParticleGenerator(Shader shader, Texture2D texture, uint32_t amount);

  void Update(float dt, GameObject &object, uint32_t newParticles,
              glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  void Draw();

private:
  std::vector<Particle> _particles;
  uint32_t _amount;
  Shader _shader;
  Texture2D _texture;
  uint32_t _VAO;

  void init();
  uint32_t firstUnusedParticle();
  void respawnParticle(Particle &particle, GameObject &object,
                       glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
