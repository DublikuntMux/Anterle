#include <cstdint>
#include <glad/gl.h>

#include "game_object.hpp"
#include "particle_generator.hpp"
#include "shader.hpp"
#include "texture.hpp"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture,
                                     uint32_t amount)
    : _amount(amount), _shader(shader), _texture(texture) {
  this->init();
}

void ParticleGenerator::Update(float dt, GameObject &object,
                               uint32_t newParticles, glm::vec2 offset) {
  for (uint32_t i = 0; i < newParticles; ++i) {
    int unusedParticle = this->firstUnusedParticle();
    this->respawnParticle(this->_particles[unusedParticle], object, offset);
  }
  for (uint32_t i = 0; i < this->_amount; ++i) {
    Particle &p = this->_particles[i];
    p.Life -= dt;
    if (p.Life > 0.0f) {
      p.Position -= p.Velocity * dt;
      p.Color.a -= dt * 2.5f;
    }
  }
}

void ParticleGenerator::Draw() {
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  this->_shader.Use();
  for (Particle particle : this->_particles) {
    if (particle.Life > 0.0f) {
      this->_shader.SetVector2f("offset", particle.Position);
      this->_shader.SetVector4f("color", particle.Color);
      this->_texture.Bind();
      glBindVertexArray(this->_VAO);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init() {
  uint32_t VBO;
  float particle_quad[] = {
      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

      0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &this->_VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(this->_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad,
               GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindVertexArray(0);

  for (uint32_t i = 0; i < this->_amount; ++i)
    this->_particles.push_back(Particle());
}

uint32_t lastUsedParticle = 0;
uint32_t ParticleGenerator::firstUnusedParticle() {
  for (uint32_t i = lastUsedParticle; i < this->_amount; ++i) {
    if (this->_particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }

  for (uint32_t i = 0; i < lastUsedParticle; ++i) {
    if (this->_particles[i].Life <= 0.0f) {
      lastUsedParticle = i;
      return i;
    }
  }

  lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object,
                                        glm::vec2 offset) {
  float random = ((rand() % 100) - 50) / 10.0f;
  float rColor = 0.5f + ((rand() % 100) / 100.0f);
  particle.Position = object.Position + random + offset;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
  particle.Velocity = object.Velocity * 0.1f;
}