#include "AnterleGame.hpp"
#include "game.hpp"

AnterleGame::AnterleGame(): Game("Antrele") {}
AnterleGame::AnterleGame(GameConfigs *Configs): Game("Antrele"), Configs(Configs) {}
