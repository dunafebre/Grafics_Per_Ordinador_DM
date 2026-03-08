#pragma once

#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "main/includes.h"
#include "image.h"
#include "shader.h"

#include <string>
#include <sys/stat.h>
#include <cstring>
#include <iostream>

struct sLight {
    Vector3 position;
    Vector3 color;
};

struct sUniformData {
    Matrix44 model_matrix;
    Matrix44 view_projection;
    Vector3 camera_position;
    Vector3 ambient_light; //ia
    std::vector<sLight> scene_light; //id i is -> color //light_position -> position
    int active_light = 0;
    
    int use_color_texture = 1;
    int use_specular_texture = 1;
    int use_normal_texture = 1;
};

class Material {
public:
    Shader* shader;
    Texture* texture;
    Texture* normal_texture;
    Vector3 ka;
    Vector3 kd;
    Vector3 ks;
    float shininess;

    Material(Shader* s, Texture* t, Texture* nt, Vector3 KA, Vector3 KD, Vector3 KS, float sh)
    {
        shader = s;
        texture = t;
        normal_texture = nt;
        ka = KA;
        kd = KD;
        ks = KS;
        shininess = sh;
    }

    void Enable(const sUniformData& data);
    void Disable();
    
};
