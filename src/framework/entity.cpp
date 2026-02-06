#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "entity.h"
#include "image.h"
#include "main/includes.h"

#include <string>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

void Entity::Render(Image* framebuffer, Camera* camera, const Color& c)
{
    for (int i = 0; i < mesh->vertices.size(); i += 3)
    {
        //local -> world
        Vector3 v0 = model * mesh->vertices[i];
        Vector3 v1 = model * mesh->vertices[i + 1];
        Vector3 v2 = model * mesh->vertices[i + 2];

        //world -> view -> clip
        v0 = camera->ProjectVector(v0);
        v1 = camera->ProjectVector(v1);
        v2 = camera->ProjectVector(v2);

        //renderitzem només els triangles projectats que es troben dins del cub [-1,1]^3
        if (v0.x < -1 || v0.x > 1 || v0.y < -1 || v0.y > 1 || v0.z < -1 || v0.z > 1) continue;
        if (v1.x < -1 || v1.x > 1 || v1.y < -1 || v1.y > 1 || v1.z < -1 || v1.z > 1) continue;
        if (v2.x < -1 || v2.x > 1 || v2.y < -1 || v2.y > 1 || v2.z < -1 || v2.z > 1) continue;

        //clip -> screen
        int x0 = (int)((v0.x + 1.0f) * 0.5f * framebuffer->width);
        int y0 = (int)((v0.y + 1.0f) * 0.5f * framebuffer->height);

        int x1 = (int)((v1.x + 1.0f) * 0.5f * framebuffer->width);
        int y1 = (int)((v1.y + 1.0f) * 0.5f * framebuffer->height);

        int x2 = (int)((v2.x + 1.0f) * 0.5f * framebuffer->width);
        int y2 = (int)((v2.y + 1.0f) * 0.5f * framebuffer->height);


        //dibuixar linies
        framebuffer->DrawLineDDA(x0, y0, x1, y1, c);
        framebuffer->DrawLineDDA(x1, y1, x2, y2, c);
        framebuffer->DrawLineDDA(x2, y2, x0, y0, c);
    }
}

void Entity::Update(float seconds_elapsed)
{
    switch(animType)
    {
        case ROTATE:
        {
            float angle = seconds_elapsed;
            Vector3 axis(0, 1, 0);
            Matrix44 rotation;
            rotation.MakeRotationMatrix(angle, axis);
            model = rotation;
            break;
        }

        case TRANSLATE:
        {
            float tx = sin(seconds_elapsed) * 2.0f;
            Matrix44 translation;
            translation.MakeTranslationMatrix(tx, 0.0f, 0.0f);
            model = translation;
            break;
        }

        case SCALE:
        {
            float s = cos(seconds_elapsed) + 0.2f;
            Matrix44 scale;
            scale.MakeScaleMatrix(s, s, s);
            model = scale;
            break;
        }
    }
}
