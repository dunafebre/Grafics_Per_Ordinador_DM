// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
uniform sampler2D u_texture;
uniform sampler2D u_normal_texture;

uniform vec3 u_light_position; //punt de llum
uniform vec3 u_camera_position; //ull
uniform float u_s;
uniform vec3 u_ka;
uniform vec3 u_kd;
uniform vec3 u_ks;
uniform vec3 u_ia;
uniform vec3 u_id;
uniform vec3 u_is;

varying vec2 v_uv;
varying vec3 v_world_position;
varying vec3 v_world_normal;

uniform int use_color_texture;
uniform int use_specular_texture;
uniform int use_normal_texture;

uniform mat4 u_model;

void main()
{
    vec3 N;
    vec3 N_map = vec3(0.0);
    vec3 normal_tex = vec3(0.0);
    vec4 tex_color = vec4(1.0);
    
    // Set the ouput color per pixel
    if(use_normal_texture != 0){
        normal_tex = texture2D(u_normal_texture, v_uv).xyz;
        normal_tex = normal_tex * 2.0 - 1.0;

        N_map = normalize((u_model * vec4(normal_tex,0.0)).xyz);
        vec3 N_vertex = normalize(v_world_normal);

        N = normalize(mix(N_vertex, N_map, 0.5));
    }
    else if(use_normal_texture == 0){
        tex_color = texture2D(u_texture, v_uv);
        N = normalize(v_world_normal);
    }
    
    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = normalize(reflect(-L, N));
    
    vec3 ka_definitiu;
    vec3 kd_definitiu;
    vec3 ks_definitiu;
    
    if(use_color_texture != 0){
        ka_definitiu = tex_color.rgb;
        kd_definitiu = tex_color.rgb;
    }
    else if(use_color_texture == 0){
        ka_definitiu = u_ka;
        kd_definitiu = u_kd;
    }
    if(use_specular_texture != 0){
        ks_definitiu = vec3(tex_color.a);
    }
    else if(use_specular_texture == 0){
        ks_definitiu = u_ks;
    }
    vec3 u_ip = (ka_definitiu * u_ia) + (kd_definitiu * clamp(dot(L, N), 0.0, 1.0) * u_id) + (ks_definitiu * pow(clamp(dot(R, V), 0.0, 1.0), u_s) * u_is);
    
    if(use_normal_texture != 0){
        gl_FragColor = vec4(u_ip, 1.0) * vec4(normal_tex, 1.0);
    }
    else if(use_normal_texture == 0){
        gl_FragColor = vec4(u_ip, 1.0) * tex_color;
    }
    
}
