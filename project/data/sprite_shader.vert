
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

uniform mediump vec4 borders;

attribute vec4 Position;
attribute vec3 Normal;
attribute vec3 Tangent;
attribute vec4 TexCoord;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space

varying vec4 vVertex;
//uniform String objName;

uniform mediump vec4 PlayerPosition;

varying mediump vec4 PlayerPosVarying;

void main()
{
    posVarying = ModelMatrix * Position;
    normalVarying = normalize(NormalMatrix * Normal);
    tangentVarying = normalize(NormalMatrix * Tangent);
    texCoordVarying = TexCoord;
    
    if (texCoordVarying.x > borders[1]) {
        texCoordVarying.x = borders[1];
    } else if (texCoordVarying.x < borders[0]) {
        texCoordVarying.x = borders[0];
    } else {
        texCoordVarying.x = texCoordVarying.x;
    }
    if (texCoordVarying.y > borders[3]) {
        texCoordVarying.y = borders[3];
    } else if (texCoordVarying.y < borders[2]) {
        texCoordVarying.y = borders[2];
    } else {
        texCoordVarying.y = texCoordVarying.y;
    }
    texCoordVarying.t = texCoordVarying.t;

    PlayerPosVarying = PlayerPosition;
    
    gl_Position = ProjectionMatrix * ViewMatrix * posVarying;
    vVertex = gl_Position;
    
    
}
