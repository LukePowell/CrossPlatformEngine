//Implements per-vertex Phong Lighting
//Fragment Shader
out vec3 vPosition;
out vec3 vNormal;//Normal to the vertex will have to renormalize in the pixel shader

uniform mat4 uMVP;
uniform mat4 uNormalMatrix;
uniform mat4 uModelView;
uniform mat4 uProjection;
uniform mat4 trans;
uniform mat4 rot;
uniform mat4 scale;

layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aNormal;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec4 aKd;
layout(location = 4) in vec4 aKa;
layout(location = 5) in vec4 aKs;
layout(location = 6) in float aNs;
layout(location = 7) in int aMatIndex;

out vec4 vKd;
out vec4 vKs;
out vec4 vKa;
out float vNs;
out vec2 texCoord;

void main()
{	
	//Set up the needed texture coordinates
	//gl_TexCoord[0] = gl_MultiTexCoord0;

    //Varyings
    mat4 normalMat = uModelView * trans * scale;
	normalMat = transpose(inverse(normalMat));

	vPosition = vec3(uModelView * trans * scale * aPos);
	vNormal = vec3(normalize(normalMat * aNormal));	

	vKd = aKd;
	vKs = aKs;
	vKa = aKa;
	vNs = aNs;

	gl_Position = uProjection * uModelView * trans * scale * aPos;
}
