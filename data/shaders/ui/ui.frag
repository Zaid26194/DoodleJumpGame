#version 400

#define EPSILON 1e-2
#define PI 3.14152654

// Inputs
in vec2 f_texCoord;

// Uniforms
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform float alpha;
uniform sampler2D tex;

// Outs
out vec4 out_color;

void main(void)
{
	out_color = texture( tex, f_texCoord );
}
