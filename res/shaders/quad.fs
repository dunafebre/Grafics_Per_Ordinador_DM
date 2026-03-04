varying vec2 v_uv;
uniform sampler2D u_texture;
uniform int u_task;
uniform int u_subtask;
uniform float time;

void main()
{
    if(u_task == 1){
            if(u_subtask == 0){
                vec3 left  = vec3(0.0, 0.0, 1.0);
                vec3 right = vec3(1.0, 0.0, 0.0);
                vec3 color = mix(left, right, v_uv.x);
                gl_FragColor = vec4(color, 1.0);
            }
            else if(u_subtask == 1){
                vec2 center = vec2(0.5, 0.5);
                float dist = distance(v_uv, center);
                gl_FragColor = vec4(vec3(dist), 1.0);
            }
            else if(u_subtask == 2){
                float rep = 5.0;
                float lineasV = pow(0.5 + 0.5 * cos(v_uv.x * rep * 6.28), 15.0);
                float lineasH = pow(0.5 + 0.5 * cos(v_uv.y * rep * 6.28), 15.0);
                gl_FragColor = vec4(lineasV, 0.0, lineasH, 1.0);
            }
            else if(u_subtask == 3){
                float celdas = 16.0;
                vec2 uv_pixel = floor(v_uv * celdas) / (celdas - 1.0);
                vec3 color = vec3(uv_pixel.x, uv_pixel.y, 0.0);
                gl_FragColor = vec4(color, 1.0);
            }
            else if(u_subtask == 4){
                float celdas = 10.0;
                vec2 grid = floor(v_uv * celdas);
                float check = mod(grid.x + grid.y, 2.0);
                gl_FragColor = vec4(vec3(check), 1.0);
            }
            else if(u_subtask == 5){
                float wave = 0.5 + 0.2 * sin(v_uv.x * 6.283);
                float mascara = step(v_uv.y, wave);
                vec3 colorNormal = vec3(0.0, v_uv.y, 0.0);
                vec3 colorInvertido = vec3(0.0, 1.0 - v_uv.y, 0.0);
                vec3 colorVerde = mix(colorInvertido, colorNormal, mascara);
                gl_FragColor = vec4(colorVerde, 1.0);
            }
        }
        else if(u_task == 2){
            if(u_subtask == 0){
                vec4 texture_color = texture2D(u_texture, v_uv);
                float gray = dot(texture_color.rgb, vec3(0.299, 0.587, 0.114));
                texture_color.rgb = vec3(gray);
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 1){
                vec4 texture_color = texture2D(u_texture, v_uv);
                texture_color.rgb = 1.0 - texture_color.rgb;
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 2){
                vec4 texture_color = texture2D(u_texture, v_uv);
                float gray = dot(texture_color.rgb, vec3(0.299, 0.587, 0.114));
                vec3 yellow = vec3(1.0, 1.0, 0.0);
                texture_color.rgb = gray * yellow;
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 3){
                vec4 texture_color = texture2D(u_texture, v_uv);
                float gray = dot(texture_color.rgb, vec3(0.299, 0.587, 0.114));
                float bw = step(0.5, gray);
                texture_color.rgb = vec3(bw);
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 4){
                vec4 texture_color = texture2D(u_texture, v_uv);
                float dist = distance(v_uv, vec2(0.5, 0.5));
                float vineta = 1.0 - smoothstep(0.3, 0.75, dist);
                texture_color = vec4(texture_color.rgb * vineta, 1.0);
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 5){
                vec4 texture_color = texture2D(u_texture, v_uv);
                float offset = 2.0 / 512.0;
                vec3 result = vec3(0.0);
                for(int x = -2; x <= 2; x++){
                    for(int y = -2; y <= 2; y++){
                        result += texture2D(u_texture, v_uv + vec2(float(x), float(y)) * offset).rgb;
                    }
                }
                result /= 25.0;
                texture_color = vec4(result, 1.0);
                gl_FragColor = texture_color;
            }
        }
        else if(u_task == 3){
            if(u_subtask == 0){
                vec2 uv = v_uv;
                float pixels = 30.0 + 20.0 * sin(time);
                uv = floor(uv * pixels) / pixels;
                vec4 texture_color = texture2D(u_texture, uv);
                gl_FragColor = texture_color;
            }
            else if(u_subtask == 1){
                vec2 uv = v_uv;
                uv.x += 0.05*sin(uv.y * 10.0 + time);
                vec4 texture_color = texture2D(u_texture, uv);
                gl_FragColor = texture_color;
            }
            else{
                vec3 color  = vec3(0.0, 0.0, 0.0);
                gl_FragColor = vec4(color, 1.0);
            }
        }
}
