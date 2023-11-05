#include <cstddef>
#include <cstdint>

#include <glad/gles2.h>

#include "object/game_object.hpp"
#include "object/particle_generator.hpp"
#include "resource/shader.hpp"
#include "resource/texture.hpp"

namespace Anterle {
ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, uint32_t amount)
  : m_amount(amount), m_shader(shader), m_texture(texture)
{
  uint32_t VBO;
  float particle_quad[] = { 0.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,

    0.0f,
    1.0f,
    0.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    1.0f,
    0.0f,
    1.0f,
    0.0f };
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
  glBindVertexArray(0);

  for (uint32_t i = 0; i < m_amount; ++i) m_particles.push_back(Particle());
}

void ParticleGenerator::Update(float dt, GameObject object, uint32_t newParticles, glm::vec2 offset)
{
  for (uint32_t i = 0; i < newParticles; ++i) {
    size_t unusedParticle = firstUnusedParticle();
    respawnParticle(m_particles[unusedParticle], object, offset);
  }
  for (uint32_t i = 0; i < m_amount; ++i) {
    Particle &p = m_particles[i];
    p.Life -= dt;
    if (p.Life > 0.0f) {
      p.Position -= p.Velocity * dt;
      p.Color.a -= dt * 2.5f;
    }
  }
}

void ParticleGenerator::Draw()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  m_shader.Use();
  for (Particle particle : m_particles) {
    if (particle.Life > 0.0f) {
      m_shader.SetVector2f("offset", particle.Position);
      m_shader.SetVector4f("color", particle.Color);
      m_texture.Bind();
      glBindVertexArray(m_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

uint32_t lastUsedParticle = 0;
uint32_t ParticleGenerator::firstUnusedParticle()
{
  for (uint32_t i = lastUsedParticle; i < m_amount; ++i) {
    if (m_particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }

  for (uint32_t i = 0; i < lastUsedParticle; ++i) {
    if (m_particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }

  lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle particle, GameObject object, glm::vec2 offset)
{
  float random = ((rand() % 100) - 50) / 10.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  particle.Position = object.Position + random + offset;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
  particle.Velocity = object.Velocity * 0.1f;
}
}// namespace Anterle
