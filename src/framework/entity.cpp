#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "entity.h"
#include "image.h"
#include "shader.h"
#include "main/includes.h"

#include <string>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

/*void Entity::Render(Image* framebuffer, Camera* camera, const Color& c0, const Color& c1, const Color& c2, FloatImage* zBuffer, eRenderMode mode, bool useTexture, bool useZBuffer, bool useInterpolatedUV)
{
    for (int i = 0; i < mesh->vertices.size(); i += 3)
    {
        //local -> world
        Vector3 v0 = model * mesh->vertices[i];
        Vector3 v1 = model * mesh->vertices[i + 1];
        Vector3 v2 = model * mesh->vertices[i + 2];
        
        Vector2 uv0 = mesh->uvs[i];
        Vector2 uv1 = mesh->uvs[i + 1];
        Vector2 uv2 = mesh->uvs[i + 2];
        
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

        Vector3 p0 = Vector3(x0, y0, v0.z);
        Vector3 p1 = Vector3(x1, y1, v1.z);
        Vector3 p2 = Vector3(x2, y2, v2.z);
        
        sTriangleInfo triangle;
        triangle.p0 = p0;
        triangle.p1 = p1;
        triangle.p2 = p2;

        triangle.uv0 = uv0;
        triangle.uv1 = uv1;
        triangle.uv2 = uv2;

        triangle.c0 = c0;
        triangle.c1 = c1;
        triangle.c2 = c2;

        triangle.texture = texture;
        
        if(mode == eRenderMode::POINTCLOUD){
            framebuffer->SetPixel(p0.x, p0.y, c0);
            framebuffer->SetPixel(p1.x, p1.y, c1);
            framebuffer->SetPixel(p2.x, p2.y, c2);
        }
        else if(mode == eRenderMode::WIREFRAME){
            framebuffer->DrawLineDDA(x0, y0, x1, y1, c0);
            framebuffer->DrawLineDDA(x1, y1, x2, y2, c1);
            framebuffer->DrawLineDDA(x2, y2, x0, y0, c2);
        }
        else if(mode == eRenderMode::TRIANGLES){
            Vector2 p0 = Vector2(x0, y0);
            Vector2 p1 = Vector2(x1, y1);
            Vector2 p2 = Vector2(x2, y2);
            framebuffer->DrawTriangle(p0,p1,p2,c0,true,c0);
        }
        else if(mode == eRenderMode::TRIANGLES_INTERPOLATED){
            framebuffer->DrawTriangleInterpolated(triangle,zBuffer,useTexture,useZBuffer, useInterpolatedUV);
        }
    }
}*/

void Entity::RenderLab4(Camera* camera){
    glEnable(GL_DEPTH_TEST);

    material->shader->Enable();

    material->shader->SetMatrix44("u_model", model);
    Matrix44 vp = camera->viewprojection_matrix;
    material->shader->SetMatrix44("u_viewprojection", vp);

    material->shader->SetTexture("u_texture", material->texture);

    mesh->Render(GL_TRIANGLES);

    material->shader->Disable();
}

void Entity::RenderLab5(sUniformData& data){
    data.model_matrix = model; //actualitza la model
    material->Enable(data); //activa el seu material
    mesh->Render(GL_TRIANGLES); //dibuixa o renderitza la mesh
    material->Disable(); //neteja tot
}

void Entity::Update(float seconds_elapsed)
{
    switch(animType)
    {
        case ROTATE:
        {
            float angle = 3.141592f * 0.25f * seconds_elapsed;
            Matrix44 rotation;
            rotation.SetIdentity();
            rotation.MakeRotationMatrix(angle, Vector3(0,1,0));
            model = model * rotation;
            break;
        }

        case TRANSLATE:
        {
            float dx = -0.4f * seconds_elapsed;
            Matrix44 translation;
            translation.SetIdentity();
            translation.MakeTranslationMatrix(dx, 0.0f, 0.0f);
            model = model * translation;
            break;
        }

        case SCALE:
        {
            float scaleFactor = 1.0f + (0.5f - 1.0f) * seconds_elapsed;
            Matrix44 scale;
            scale.SetIdentity();
            scale.MakeScaleMatrix(scaleFactor, scaleFactor, scaleFactor);
            model = model * scale;
            break;
        }
        
        case ZERO:
        {
            /*Matrix44 null;
            null.SetIdentity();
            model = null;*/
        }
    }
}
