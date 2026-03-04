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

    shader->SetMatrix44("u_model", data.model);
    shader->SetMatrix44("u_viewprojection", data->viewprojection);

    shader->SetUniform3("u_material.ka", ka.x, ka.y, ka.z);
    shader->SetUniform3("u_material.kd", kd.x, kd.y, kd.z);
    shader->SetUniform3("u_material.ks", ks.x, ks.y, ks.z);
    shader->SetUniform1("u_material.shininess", shininess);

    shader->SetTexture("u_texture", texture);
}

void Material::Disable(){
    shader->Disable();
}

