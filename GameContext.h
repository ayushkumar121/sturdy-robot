//
// Created by ari on 2/22/26.
//

#ifndef STURDY_ROBOT_GAMECONTEXT_H
#define STURDY_ROBOT_GAMECONTEXT_H

#include "Basic.h"

class GameContext {
public:
    static GameContext& getInstance();
    Basic::Mat4 getProjectionMatrix() const;

    void setFrameWidth(int width);
    void setFrameHeight(int height);

private:
    int frameWidth{};
    int frameHeight{};
};


#endif //STURDY_ROBOT_GAMECONTEXT_H