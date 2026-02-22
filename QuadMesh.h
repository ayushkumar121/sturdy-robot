//
// Created by ari on 2/23/26.
//

#ifndef STURDY_ROBOT_QUADMESH_H
#define STURDY_ROBOT_QUADMESH_H

class QuadMesh {
public:
    ~QuadMesh();
    static QuadMesh& getInstance();
    void draw() ;

private:
    QuadMesh();
    unsigned int vao;
    unsigned int vbo;
};


#endif //STURDY_ROBOT_QUADMESH_H