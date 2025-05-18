#version 330

out vec4 FragColor;
in vec2 pos;

uniform float Time;

void main()
{
	vec3 color = vec3(1.,0.,1.0);
	vec3 circle = vec3(.5,.5+sin(Time/1000.+pos.x*8)/8.,.2);
	vec2 posComp = fract(pos*10);
	float d = length(posComp-circle.xy) - circle.z+sin(Time/1000.+pos.x*8)/8.;
	d = smoothstep(0.0,0.01,d); 
	
    gl_FragColor  = vec4((1-d)*color, 1.0f);		
} 
