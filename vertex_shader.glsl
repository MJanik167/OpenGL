#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 TexPos;
layout (location = 2) in vec3 offset;


uniform mat4 MVP;
uniform float Time;


out vec2 pos;

void main()
{
    pos = TexPos;
    vec4 position = vec4(aPos.x, aPos.y, 0.0 ,1.0);
    position.xy = position.xy*2. - 1.;
    position.y += sin(Time/1000.+pos.x*8)/8;
    gl_Position = position;
}