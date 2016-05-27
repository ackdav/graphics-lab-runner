
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

uniform mediump float pressedSeconds;

uniform mediump vec4 borders;
// borders[0] : minX
// borders[1] : maxX
// borders[2] : minY
// borders[3] : maxY

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

uniform mediump vec4 PlayerPosition;

varying mediump vec4 PlayerPosVarying;

void main()
{
    posVarying = ModelMatrix * Position;
    normalVarying = normalize(NormalMatrix * Normal);
    tangentVarying = normalize(NormalMatrix * Tangent);
    texCoordVarying = TexCoord;
    
    

    //texCoordVarying.t = 0f;
    PlayerPosVarying = PlayerPosition;
    
    // TL:
    // BL: x>0.5,y>0.5
    // BR: x<0.5,y>0.5
    //
    texCoordVarying.x = texCoordVarying.x +0.01*pressedSeconds;
    gl_Position = ProjectionMatrix * ViewMatrix * posVarying;
    vVertex = gl_Position;
    
    
}
