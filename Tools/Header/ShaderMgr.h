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

    GLint textureCubeMapShader;
    GLint textureCubeMapShader_iMatrix;
    GLint textureCubeMapShader_iColor;
    GLint textureCubeMapShader_iCubeMap;

    GLint textureSkybox;
    GLint textureSkybox_iCubeMap;
    GLint textureSkybox_iMatrix;
public:
    void OnInit();
    void OnUnInit();

    void UseSolidColor(M3DVector4f color);
    void UseDiffuse(M3DVector4f color, M3DMatrix44f mvpMatrix);
    void UseDiffuse(M3DVector4f color, const M3DMatrix44f mvpMatrix);
    void UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture);
    void UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture, GLuint time);
    void UseCubeMap(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint cubeMap);
    void UseSkyBox(const M3DMatrix44f mvpMatrix, GLuint cubeMap);
    //创建shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
};

#endif // SHADERMGR__
