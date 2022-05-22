#shader vertex
#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float TexId;
layout(location = 3) in vec3 cubePos;

out vec3 v_TexCoord; 

uniform mat4 u_MVP;

void main() {
    gl_Position =  u_MVP * position;
    v_TexCoord = position.xyz - cubePos;
}

#shader fragment
#version 410 core

layout(location = 0) out vec4 color;

in vec3 v_TexCoord;

uniform samplerCube u_Texture;

void main() {
    vec3 shadow = vec3(1.0);
    if(v_TexCoord.x > 0.499999) {
        shadow *= 0.7;
    }else if(v_TexCoord.z > 0.499999) {
        shadow *= 0.8;
    }else if(v_TexCoord.z < -0.499999) {
        shadow *= 0.9;
    }else if(v_TexCoord.x < -0.499999) {
        shadow *= 0.76;
    }
    color = texture(u_Texture, v_TexCoord) * vec4(shadow, 1.0);
    //color = texColor;
}
