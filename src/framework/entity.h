#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "main/includes.h"
#include "image.h"

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

enum class eRenderMode {
    POINTCLOUD,
    WIREFRAME,
    TRIANGLES,
    TRIANGLES_INTERPOLATED
};

class Entity {
public:
    Mesh* mesh;
    Matrix44 model;
    Image* texture;
    AnimationType animType;

    Entity(Mesh* m, const Matrix44& md, Image* t, AnimationType at)
    {
        mesh = m;
        model = md;
        texture = t;
        animType = at;
    }

    void Render(Image* framebuffer, Camera* camera, const Color& c0, const Color& c1, const Color& c2, FloatImage* zBuffer, eRenderMode mode, bool useTexture, bool useZBuffer, bool useInterpolatedUV);
    void Update(float seconds_elapsed);
    
};


