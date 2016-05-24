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

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space

uniform mediump float GameTime;

varying mediump float GameTimeVarying;


//fogParameters

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
    fogParams.fDensity = 0.1;
    fogParams.fStart = 10.0;
    fogParams.fEnd = 75.0;
    fogParams.fogColor = vec4(0.7, 0.7, 0.7, 1.0);
    fogParams.iFogEquation = 1;

    
    
    
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
 
    
    mediump vec3 l = normalize(LightPos - pos).xyz;
    
    lowp float intensity = max(dot(n, l),0.0);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // If vertex is lit, calculate specular term in view space using the Blinn-Phong model
    lowp vec4 specularResult = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos.xyz - pos.xyz);
        mediump vec3 h = normalize(l + eyeVec);
        
        mediump float specIntensity = pow(max(dot(h,n), 0.0), Ns);
        mediump vec3 specular = Ks * clamp(texture2D(SpecularMap,texCoordVarying.st).xyz* (specIntensity/4.0), 0.0, 1.0) * Is;
        specularResult = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }
    
    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
//    color=clamp(color,-1.0,1.0);
//    normal = normalize(normal * 2.0 - 1.0);
//    lowp vec4 color = vec4(1.0,0.0,1.0,1.0);

    vec4 colorVarying = vec4(vec3(0.5) + n.xyz * 0.5, 1.0);
    //gl_FragColor = (ambientResult + diffuseResult) * color + specularResult;


    float randomized = fract(sin(dot(texCoordVarying.xy ,vec2(12.9898,78.233))) * 43758.5453);
    
    float sil = dot(normalize(LightPos.xyz - pos.xyz), normalVarying);
    
    
    
    if (GameTimeVarying < 1.0){
    
    if (sil > 0.95 * 0.5/GameTimeVarying)
        color *= 0.95*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.8 * 0.5/GameTimeVarying)
        color *= 0.9*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.7 * 0.5/GameTimeVarying)
        color *= 0.85*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.6 * 0.5/GameTimeVarying)
        color *= 0.8*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.5 * 0.5/GameTimeVarying)
        color *= 0.75*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.4 * 0.5/GameTimeVarying)
        color *= 0.7*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.3 * 0.5/GameTimeVarying)
        color *= 0.65*vec4(1.0,1.0,1.0,1.0);
    else
        color *= 0.6*vec4(1.0,1.0,1.0,1.0);

        
    }

   

    gl_FragColor = (ambientResult+diffuseResult ) * color;
    
    
    float fFogCoord = abs(EyePos.z/EyePos.w);
    
    float fogFactor;
    
    float fResult = 0.0;
    if(fogParams.iFogEquation == 0)
        fResult=fogParams.fEnd-fFogCoord/(fogParams.fEnd-fogParams.fStart);
    else if(fogParams.iFogEquation == 1)
        fResult = exp(-fogParams.fDensity*fFogCoord);
    else if(fogParams.iFogEquation == 2)
        fResult = exp(-pow(fogParams.fDensity*fFogCoord, 2.0));
    
    fResult = 1.0-clamp(fResult, 0.0, 1.0);
    
    fogFactor = fResult;
    
    
    gl_FragColor = mix(gl_FragColor, fogParams.fogColor, fogFactor);

    
}
