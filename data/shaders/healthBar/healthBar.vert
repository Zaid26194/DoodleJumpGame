#version 400

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoord;

uniform float angle;
uniform float position;
uniform float size;

void main ()
{
	float ctheta = cos( angle );
	float stheta = sin( angle );
	mat2 rotation = mat2( ctheta, -stheta, stheta, ctheta );
	
	vec2 pos = position + rotation*( size/100 * in_position.xy);

	gl_Position = vec4(in_position,1);//vec4(16*pos,0,1);
}
