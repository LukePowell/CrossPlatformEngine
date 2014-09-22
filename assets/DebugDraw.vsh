#version 330
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uTrans;
uniform mat4 uRot;
uniform mat4 uScale;

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aColor;

out vec4 color;

void main()
{
	color = aColor;
	gl_Position = uProjection * uView * uTrans * uScale * uRot * aPos;
}