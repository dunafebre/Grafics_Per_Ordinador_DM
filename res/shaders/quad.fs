// Receive the uvs interpolated from the vertex
varying vec2 v_uv;

// Receive the texture as a sampler2D from our application
uniform sampler2D u_texture;

void main()
{
    vec4 texture_color = texture2D(u_texture, v_uv);
    gl_FragColor = texture_color;
}
