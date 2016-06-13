precision mediump float;
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

uniform mediump vec4 PlayerPosition;

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space

//varying sampler2D smurfNormal;

varying vec4 vVertex;

//lowp vec4 fogColor;

varying mediump vec4 PlayerPosVarying;

// FogParameters

struct fogParameters{
    
    float fDensity;
    float fStart;
    float fEnd;
    vec4 fogColor;
    int iFogEquation;
} fogParams;







void main()
{
    
    //fogParameters
    fogParams.fDensity = 0.04;
    fogParams.fStart = 10.0;
    fogParams.fEnd = 20.0;
    fogParams.fogColor = vec4(0.7, 0.7, 0.7, 1.0);
    fogParams.iFogEquation = 2;
    
    
    mediump vec3 sunpos = vec3(-20.0, 20.0, 0);
    mediump vec3 sunlightdirection = -sunpos.xyz;
    mediump vec4 sunlightcolor = vec4(0.5,0.5,0.5,0.5);
    

    
    lowp vec4 tempColor;
    
    mediump vec4 pos = posVarying;
    
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.xy) ;

//    color = color  +  texture2D(NormalMap, (texCoordVarying).xy);
  
    
    gl_FragColor = tempColor * color ;
    
    float fFogCoord = abs(EyePos.y/EyePos.w);
    
    float fogFactor;
    
    float fResult = 0.0;
    
    fResult = exp(-fogParams.fDensity*fFogCoord);
    
    fogFactor = fResult;
    
    
    float depth = pos.y;
    float fogFactorIntensity = (1.0-(1.0/(-depth))+0.2);
    
    if(pos.y<-1.0 && gl_FragColor.w > 0.1){
        gl_FragColor = mix(gl_FragColor, fogParams.fogColor, fogFactorIntensity*fogFactor-0.1);
    }
    
    
    
    
    
    
}