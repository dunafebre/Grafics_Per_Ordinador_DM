#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "main/includes.h"

#include <string>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

enum AnimationType {
    ZERO,
    ROTATE,
    TRANSLATE,
    SCALE
};

class Entity {
public:
    Mesh* mesh;
    Matrix44 model;
    AnimationType animType;
    
    Entity(Mesh* m, const Matrix44& md, AnimationType at)
    {
        mesh = m;
        model = md;
        animType = at;
    }

    void Render(Image* framebuffer, Camera* camera, const Color& c);
    void Update(float seconds_elapsed);
    
};
