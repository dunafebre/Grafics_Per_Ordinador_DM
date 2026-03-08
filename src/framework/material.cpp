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

void Material::Enable(const sUniformData& data){
    shader->Enable();

    shader->SetMatrix44("u_model", data.model_matrix);
    shader->SetMatrix44("u_viewprojection", data.view_projection);
    
    const sLight& light = data.scene_light[data.active_light];
    shader->SetUniform3("u_light_position", light.position.x, light.position.y, light.position.z);
    shader->SetUniform3("u_camera_position", data.camera_position.x, data.camera_position.y, data.camera_position.z);
    
    shader->SetUniform1("u_s", shininess);
    shader->SetUniform3("u_ka", ka.x, ka.y, ka.z);
    shader->SetUniform3("u_kd", kd.x, kd.y, kd.z);
    shader->SetUniform3("u_ks", ks.x, ks.y, ks.z);
    
    shader->SetUniform3("u_ia", data.ambient_light.x, data.ambient_light.y, data.ambient_light.z);
    shader->SetUniform3("u_id", light.color.x, light.color.y, light.color.z);
    shader->SetUniform3("u_is", light.color.x, light.color.y, light.color.z);
    
    shader->SetTexture("u_texture", texture);
    shader->SetTexture("u_normal_texture", normal_texture);
    
    shader->SetInt("use_color_texture", data.use_color_texture);
    shader->SetInt("use_specular_texture", data.use_specular_texture);
    shader->SetInt("use_normal_texture", data.use_normal_texture);
}

void Material::Disable(){
    shader->Disable();
}
