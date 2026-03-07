// This variables comes from the vertex shader
// They are baricentric interpolated by pixel according to the distance to every vertex
uniform sampler2D u_texture;

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

void main()
{
    // Set the ouput color per pixel
    vec4 tex_color = texture2D(u_texture, v_uv);
    
    vec3 N = normalize(v_world_normal);
    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = normalize(reflect(-L, N));

    vec3 u_ip = (u_ka * u_ia) + (u_kd * clamp(dot(L, N), 0.0, 1.0) * u_id) + (u_ks * pow(clamp(dot(R, V), 0.0, 1.0), u_s) * u_is);
    
    gl_FragColor = vec4(u_ip, 1.0) * tex_color;
}
