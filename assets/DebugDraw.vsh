#version 330
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uTrans;
uniform mat4 uRot;
uniform mat4 uScale;
uniform vec4 uColor;

layout(location = 0) in vec4 aPos;


out vec4 color;

void main()
{
	color = uColor;
	gl_Position = uProjection * uView * uTrans * uScale * uRot * aPos;
}