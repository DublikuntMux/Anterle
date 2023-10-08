#include "AnterleGame.hpp"
#include "game.hpp"
#include "backend/vulkan.hpp"

AnterleGame::AnterleGame() : Game(Anterle::VulkanBackend(1280, 720, "Antrele")) {}
AnterleGame::AnterleGame(GameConfigs *Configs) : Game(Anterle::VulkanBackend(1280, 720, "Antrele")), Configs(Configs) {}
