#version 330
uniform vec4 uColor;

out vec4 oColor;

in vec4 color;

void main()
{
	oColor = uColor;
}