//
// Created by ari on 2/22/26.
//

#include "GameContext.h"

GameContext& GameContext::getInstance() {
    static GameContext instance;
    return instance;
}

Basic::Mat4 GameContext::getProjectionMatrix() const {
    return Basic::Mat4::projection(this->frameWidth, this->frameHeight);
}

void GameContext::setFrameWidth(int width) {
    this->frameWidth = width;
}

void GameContext::setFrameHeight(int height) {
    this->frameHeight = height;
}
