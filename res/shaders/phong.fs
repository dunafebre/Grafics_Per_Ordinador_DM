uniform vec3 u_ks; // specular coefficient
uniform vec3 u_ia; // ambient light
uniform vec3 u_id; // diffuse light
uniform vec3 u_is; // specular light

varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform int use_color_texture;
uniform int use_specular_texture;
uniform int use_normal_texture;

uniform mat4 u_model;

void main()
{
    vec3 N;              // normal final
    vec3 normal_tex = vec3(0.0);
    vec4 tex_color = vec4(1.0);

    // Normal mapping
    if (use_normal_texture != 0) {
        normal_tex = texture2D(u_normal_texture, v_uv).xyz;
        normal_tex = normal_tex * 2.0 - 1.0;                      // convertir de [0,1] a [-1,1]
        N = normalize((u_model * vec4(normal_tex, 0.0)).xyz);     // transformar normal al espacio mundial
        N = normalize(mix(v_world_normal, N, 0.5));               // mezclar con la normal del vértice
    } else {
        N = normalize(v_world_normal);
    }

    // Textura de color
    if (use_color_texture != 0) {
        tex_color = texture2D(u_texture, v_uv);
    }

    // Vectores de iluminación
    vec3 L = normalize(u_light_position - v_world_position);      // luz
    vec3 V = normalize(u_camera_position - v_world_position);     // vista
    vec3 R = normalize(reflect(-L, N));                            // reflejo

    // Coeficientes de Phong
    vec3 ka_final;
    vec3 kd_final;
    vec3 ks_final;

    if (use_color_texture != 0) {
        ka_final = tex_color.rgb;
        kd_final = tex_color.rgb;
    } else {
        ka_final = u_ka;
        kd_final = u_kd;
    }

    if (use_specular_texture != 0) {
        ks_final = vec3(tex_color.a);
    } else {
        ks_final = u_ks;
    }

    // Cálculo final de iluminación
    vec3 color = (ka_final * u_ia)
               + (kd_final * clamp(dot(L, N), 0.0, 1.0) * u_id)
               + (ks_final * pow(clamp(dot(R, V), 0.0, 1.0), u_s) * u_is);

    // Color final
    gl_FragColor = vec4(color, 1.0) * tex_color;
}
