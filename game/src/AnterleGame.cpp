#include "AnterleGame.hpp"
#include "game.hpp"

AnterleGame::AnterleGame() : Game(1280, 720, "Antrele") {}
AnterleGame::AnterleGame(GameConfigs *Configs) : Game(1280, 720, "Antrele"), Configs(Configs) {}
