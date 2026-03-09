uniform sampler2D u_texture;
uniform sampler2D u_normal_texture;

uniform vec3 u_light_position;
uniform vec3 u_camera_position;

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
    vec3 N = normalize(v_world_normal);
    vec3 ka_final = u_ka;
    vec3 kd_final = u_kd;
    vec3 ks_final = u_ks;

    vec4 tex_color = vec4(1.0);

    if(use_color_texture != 0)
    {
        tex_color = texture2D(u_texture, v_uv);

        ka_final = tex_color.rgb;
        kd_final = tex_color.rgb;

        if(use_specular_texture != 0)
            ks_final = vec3(tex_color.a);
    }

    if(use_normal_texture != 0)
    {
        vec3 normal_tex = texture2D(u_normal_texture, v_uv).rgb;

        vec3 local_normal = normal_tex * 2.0 - 1.0;

        vec3 tex_world_normal = normalize((u_model * vec4(local_normal,0.0)).xyz);

        N = normalize(mix(N, tex_world_normal, 0.5));
    }

    vec3 L = normalize(u_light_position - v_world_position);
    vec3 V = normalize(u_camera_position - v_world_position);
    vec3 R = normalize(reflect(-L, N));

    vec3 color =
          ka_final * u_ia
        + kd_final * u_id * max(dot(N,L),0.0)
        + ks_final * u_is * pow(max(dot(R,V),0.0), u_s);

    gl_FragColor = vec4(color,1.0);
}
