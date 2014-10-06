//#version 330

in vec4 vColor;

uniform sampler2D uDiffuseMap;
uniform sampler2D uSpecularMap;

in vec3 vPosition;
in vec3 vNormal;
in vec3 vCamPosW;
in vec4 vKd;
in vec4 vKs;
in vec4 vKa;
in float vNs;
in vec2 texCoord;

//Light and camera attributes
uniform vec3 uLightPosition;
uniform vec4 uLightDiffuse;
uniform vec4 uLightAmbient;
uniform vec4 uLightSpecular;

uniform vec3 uCamPos;

//Lighting information for all 8 possible lights
uniform int uNumActiveLights;
uniform vec3 uLightPositions[8];
uniform vec4 uLightsDiffuse[8];
uniform vec4 uLightsAmbient[8];
uniform vec4 uLightsSpecular[8];
uniform vec4 uLightsDirection[8];

//view for lights just the view transform
uniform mat4 uTestTest;//Actually simply the model transformation
uniform bool uTextureMapping;
	
//Attenuation, fog, global etc
uniform vec4 uGlobalAmbient;
uniform vec3 uAttenuationConstants;
uniform vec3 uSpotLightDirection[8];
uniform int uLightType[8];
uniform float uOutterConeAngle;
uniform float uInnerConeAngle;
uniform float uFar;
uniform float uNear;
uniform vec4 uFogColor;
uniform vec4 uDiffuse;

out vec4 outColor;

void main()
{	
	//Grab data that is not light dependent
	vec3 Normal = normalize(vNormal); 
	
	vec4 MatDiffuse;
	vec4 MatSpecular; 
	vec4 MatEmmisive = vec4(0.0,0.0,0.0,1.0);
	vec4 MatAmbient;
	
	/*if(vTextureMapping)
	{
		MatDiffuse = texture2D(uDiffuseMap,gl_TexCoord[0].st);
		MatSpecular = texture2D(uSpecularMap,gl_TexCoord[0].st);
	}*/
	/*else 
	{*/
		MatDiffuse = vKd;//uDiffuse;
		MatSpecular = vKs;
	//}
	
	//We are in eye position thus cam  pos is 0 0 - vPostion = -vPosition
	vec3 View = normalize(-vPosition);
	
	vec4 ISum = vec4(0.0,0.0,0.0,1.0);

	float SpotEffect = 1.0;
	
	vec4 Diffuse;
	//Looping on a uniform is more then likely not optimial
	for(int i = 0; i < uNumActiveLights+1; ++i)
	{
		vec3 LightVec = normalize(vec3(uTestTest * vec4(uLightPositions[i],1.0)) - vPosition);
		if(uLightType[i] == 1)
			LightVec = normalize(vec3(uTestTest * vec4(uLightsDirection[i])));
		 
		float Att = CalcAtt(uAttenuationConstants.x,uAttenuationConstants.y,uAttenuationConstants.z,length(LightVec));
		if(uLightType[i] == 1)
			Att = 1.0;
			
		vec3 Reflection = reflect(Normal,LightVec);		
			
		vec4 Ambient = CalcAmbient(MatDiffuse,uLightsAmbient[i]);
		Ambient = clamp(Ambient,0.0,1.0);
	
		Diffuse = CalcDiffuse(MatDiffuse,uLightsDiffuse[i],Normal,LightVec);
		Diffuse = clamp(Diffuse,0.0,1.0);
	
		vec4 Specular = CalcSpecular(uLightsSpecular[i],MatSpecular,Reflection,View,vNs*255.0);
		Specular = clamp(Specular,0.0,1.0);
		
		if(uLightType[i] == 2)
		{
			vec3 L = normalize(vec3(uTestTest * vec4(uSpotLightDirection[i],1.0)));
			SpotEffect = CalcSpotlight(max(dot(-LightVec,L),0.0),uInnerConeAngle,uOutterConeAngle,1.0);
		} 
		else
			SpotEffect = 1.0;

		ISum += Att * Ambient + Att * SpotEffect * (Diffuse + Specular);
	}
	
	//Everything but fog
	//ISum = MatEmmisive + uGlobalAmbient * MatDiffuse + ISum;
	
	//Fog
	float S = CalcFog(uFar,uNear,vPosition.z);
	//ISum = S * ISum + (1.0 - S) * uFogColor;
	
	clamp(ISum,0.0,1.0);
	
	ISum.a = 1.0;
	outColor = ISum;
}
