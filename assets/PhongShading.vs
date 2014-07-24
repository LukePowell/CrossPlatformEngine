//Implements per-vertex Phong Lighting
//Fragment Shader
varying vec3 vPosition;
varying vec3 vNormal;//Normal to the vertex will have to renormalize in the pixel shader

void main()
{	
	//Set up the needed texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;

    //Varyings
	vPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vNormal = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = ftransform();
	
	//vColor =  texture2D(uDiffuseMap,gl_TexCoord[0].st);
}
