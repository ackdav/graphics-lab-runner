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



void main()
{
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
    mediump vec3 nTemp = texture2D(NormalMap, texCoordVarying.st).rgb;
    nTemp = normalize(nTemp * 2.0 - 1.0);
    nTemp = normalize(tbn * nTemp);
    n = nTemp;
    
    
    mediump vec3 l = normalize(LightPos - pos).xyz;
    
    lowp float intensity = dot(n, l);
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


    
    float sil = dot(normalize(EyePos.xyz - pos.xyz), normalVarying);
    
    
    if (sil > 0.95)
        color *= 0.95*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.8)
        color *= 0.8*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.7)
        color *= 0.7*vec4(1.0,1.0,1.0,1.0);
    else if (sil > 0.6)
        color *= 0.6*vec4(1.0,1.0,1.0,1.0);
    else
        color *= 0.5*vec4(1.0,1.0,1.0,1.0);

    
   

    gl_FragColor = (ambientResult+diffuseResult ) * color ;
    
}
