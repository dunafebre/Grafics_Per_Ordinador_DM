#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "main/includes.h"
#include "image.h"
#include "shader.h"
#include "material.h"

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
    AnimationType animType;
    Material* material;

    Entity(Mesh* m, const Matrix44& md, Texture* t, AnimationType at)
    {
        mesh = m;
        model = md;
        animType = at;
    }

    //void Render(Image* framebuffer, Camera* camera, const Color& c0, const Color& c1, const Color& c2, FloatImage* zBuffer, eRenderMode mode, bool useTexture, bool useZBuffer, bool useInterpolatedUV);
    void RenderLab4(Camera* camera);
    void RenderLab5(sUniformData& data);
    void Update(float seconds_elapsed);
    
};


