#ifndef SHADERMGR__
#define SHADERMGR__ XXX
#include "Tools.h"
enum ShaderType
{
    ST_Solid,

};


class ShaderMgr
{
private:
    GLuint solidShader;

    GLuint diffuseShader;
    GLint diffuseShader_iMatrix;
    GLint diffuseShader_iColor;

    GLuint texture2dShader;
    GLint texture2dShader_iMatrix;
    GLint texture2dShader_iColor;
    GLint texture2dShader_iColorMap;

    GLint texture2dArrayShader;
    GLint texture2dArrayShader_iMatrix;
    GLint texture2dArrayShader_iColor;
    GLint texture2dArrayShader_iColorMap;
    GLint texture2dArrayShader_iTime;
public:
    void OnInit();
    void OnUnInit();

    void UseSolidColor(M3DVector4f color);
    void UseDiffuse(M3DVector4f color, M3DMatrix44f mvpMatrix);
    void UseDiffuse(M3DVector4f color, const M3DMatrix44f mvpMatrix);
    void UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture);
    void UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture, GLuint time);

    //����shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //����shader��Դ
    bool LoadShaderFile(const char* file, GLuint shader);
};

#endif // SHADERMGR__
