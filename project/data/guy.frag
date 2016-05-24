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



varying vec4 vVertex;

lowp vec4 fogColor;

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
    
    lowp vec4 ambientResult = vec4(Ka * Ia, 1.0);
    
    mediump vec4 pos = posVarying;
    mediump vec3 n = normalize(normalVarying);
    mediump vec3 t = normalize(tangentVarying);

    // TODO: calculate tbn matrix, ensure it's orthonormal
    mediump vec3 nM = normalize(vec3(ModelMatrix * vec4(n, 0)));
    mediump vec3 tM = normalize(vec3(ModelMatrix * vec4(t, 0.0)));
    mediump vec3 bM = normalize(vec3(ModelMatrix * vec4(normalize(vec4(cross(t,n), 0.0)))));
    mediump mat3 tbn = mat3(tM, bM, nM);
    
    // TODO: read and correctly transform normals from normal map, then use them for lighting
    
    mediump vec3 l = -normalize(LightPos - pos).xyz;
    
    lowp float intensity = dot(n, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);

    vec4 specular = vec4(0.0);
    if (dot(normalVarying, l) > 0.0)
    {
        highp vec3 eyeVec = normalize((EyePos - posVarying).xyz);
        highp vec3 h = normalize((l + eyeVec)/length(l+eyeVec));
        highp vec3 spec = Ks * pow(dot(normalVarying,h), Ns) * Is;
        specular = vec4(clamp(spec,0.0,1.0),1.0);
    }
    
    
    lowp vec4 NormalizedReflectedViewVector = vec4(l - 2.0 * ( l * n ) * n, 1.0);
    
    lowp float D = pow(dot(normalize(EyePos-vVertex), NormalizedReflectedViewVector),1.0);
    
    
    lowp vec4 color = texture2D(DiffuseMap, (D*texCoordVarying).st);
    
    

    tempColor = ambientResult+diffuseResult;

    float deltax=abs(PlayerPosition.x-pos.x);
    float deltay=abs(PlayerPosition.y-pos.y);
    float deltaz=abs(PlayerPosition.z-pos.z);
    float deltaxcir=sqrt(deltax*deltax*deltaz*deltaz);
    //float deltax=PlayerPosition.x-posVarying;
    
    
    float sunlightangle = acos(dot(sunlightdirection.xyz, normalVarying.xyz)/(length(sunlightdirection.xyz)*length(normalVarying.xyz)));
    
    
    
    
    gl_FragColor = tempColor * color ;
    
    
    if(sunlightangle>0.0 && sunlightangle < 3.141 && normalVarying.x > 0.0){
        gl_FragColor = gl_FragColor * sunlightcolor;
    }
    
    
    float fFogCoord = abs(EyePos.y/EyePos.w);
    
    float fogFactor;
    
    float fResult = 0.0;
    
    fResult = exp(-fogParams.fDensity*fFogCoord);

    fogFactor = fResult;

    
    float depth = pos.y;
    float fogFactorIntensity = (1.0-(1.0/(-depth))+0.2);
    
    if(deltax<0.2&&deltaz<0.2&&deltay>2.0 && deltaxcir<0.02){
        gl_FragColor = mix(gl_FragColor, vec4(0.0,0.0,0.0,1.0),1.0);
    }
    
    
    if(pos.y<-1.0){
    gl_FragColor = mix(gl_FragColor, fogParams.fogColor, fogFactorIntensity*fogFactor-0.1);
    }
    
    
    




    
}
