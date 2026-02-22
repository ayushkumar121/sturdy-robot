//
// Created by ari on 2/23/26.
//

#ifndef STURDY_ROBOT_RENDERER_H
#define STURDY_ROBOT_RENDERER_H
#include <vector>

#include "Quad.h"
#include "Shader.h"


class Renderer {
public:
    void begin(const Basic::Mat4 &projection);
    void submit(Quad& quad);
    void end() const;
private:
    std::vector<Quad> drawList;
    Basic::Mat4 projection;
};


#endif //STURDY_ROBOT_RENDERER_H