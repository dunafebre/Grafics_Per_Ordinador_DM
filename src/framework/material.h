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
};

class Material {
public:
    Shader* shader;
    Texture* texture;
    Vector3 ka;
    Vector3 kd;
    Vector3 ks;
    float shininess;

    Material(Shader* s, Texture* t, Vector3 KA, Vector3 KD, Vector3 KS, float sh)
    {
        shader = s;
        texture = t;
        ka = KA;
        kd = KD;
        ks = KS;
        shininess = sh;
    }

    void Enable(const sUniformData& data);
    void Disable();
    
};
