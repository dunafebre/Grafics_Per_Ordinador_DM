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
    
    shader->SetUniform3("u_light_position", data.scene_light.position.x, data.scene_light.position.y, data.scene_light.position.z);
    shader->SetUniform3("u_camera_position", data.camera_position.x, data.camera_position.y, data.camera_position.z);
    
    shader->SetUniform1("u_s", shininess);
    shader->SetUniform3("u_ka", ka.x, ka.y, ka.z);
    shader->SetUniform3("u_kd", kd.x, kd.y, kd.z);
    shader->SetUniform3("u_ks", ks.x, ks.y, ks.z);
    
    shader->SetUniform3("u_ia", data.ambient_light.x, data.ambient_light.y, data.ambient_light.z);
    shader->SetUniform3("u_id", data.scene_light.color.x, data.scene_light.color.y, data.scene_light.color.z);
    shader->SetUniform3("u_is", data.scene_light.color.x, data.scene_light.color.y, data.scene_light.color.z);
    
    shader->SetTexture("u_texture", texture);
}

void Material::Disable(){
    shader->Disable();
}
