#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "entity.h"
#include "image.h"
#include "material.h"
#include "main/includes.h"

#include <string>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

void Material::Enable(const sUniformData& data)
{
    shader->Enable();

    shader->SetMatrix44("u_model", data.model_matrix);
    shader->SetMatrix44("u_viewprojection", data.view_projection);

    shader->SetVector3("u_camera_position", data.camera_position);

    const sLight& light = data.scene_light[data.active_light];

    shader->SetVector3("u_light_position", light.position);
    shader->SetVector3("u_id", light.color);
    shader->SetVector3("u_is", light.color);

    shader->SetVector3("u_ia", data.ambient_light);

    shader->SetVector3("u_ka", ka);
    shader->SetVector3("u_kd", kd);
    shader->SetVector3("u_ks", ks);

    shader->SetUniform1("u_s", shininess);

    shader->SetUniform1("use_color_texture", data.use_color_texture);
    shader->SetUniform1("use_specular_texture", data.use_specular_texture);
    shader->SetUniform1("use_normal_texture", data.use_normal_texture);

    if(texture)
        shader->SetTexture("u_texture", texture);

    if(normal_texture)
        shader->SetTexture("u_normal_texture", normal_texture);
}

void Material::Disable(){
    shader->Disable();
}
