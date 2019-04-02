#ifndef SHADERMGR__
#define SHADERMGR__ XXX
#include "Tools.h"
enum ShaderType
{
    STSolid,
    STDiffuse,
    STTexture2d,
    STTextureArray,
    STCubemap,
    STSkybox,
    STTextureSprite,
    STBlur,
    STTBO,
    STMax,
};



class ShaderMgr
{
    typedef void (ShaderMgr::*VoidDeldgate)();
private:
    std::map<ShaderType, VoidDeldgate> initfunctions;

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

    GLint textureSprite;
    GLint textureSprite_iColorMap;
    GLint textureSprite_iMatrix;
    GLint textureSprite_iSize;

    GLint blurShader;
    GLint blur_iMatrix;
    GLint blurTexture[6];

    GLint tboShader;
    GLint tboShader_iMatrix;
    GLint tboShader_iColorMap;
    GLint tboShader_iMaxWidth;
    GLint tboShader_iMaxHeight;

public:
    ShaderMgr();
    void OnInit(int type=-1);
    void OnUnInit();
    void InitSolid();
    void InitDiffuse();
    void InitTexture2d();
    void InitTextureArray();
    void InitCubemap();
    void InitTextureSkybox();
    void InitTextureSprite();
    void InitBlur();
    void InitTbo();
public:
    void UseSolidColor(M3DVector4f color);
    void UseDiffuse(M3DVector4f color, M3DMatrix44f mvpMatrix);
    void UseDiffuse(M3DVector4f color, const M3DMatrix44f mvpMatrix);
    void UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit);
    void UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit, GLuint time);
    void UseCubeMap(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit);
    void UseSkyBox(const M3DMatrix44f mvpMatrix, GLuint textureUnit);
    void UseSpritePoint(const M3DMatrix44f mvpMatrix, GLuint textureUnit, GLfloat size);
    void UseBlurShader(const M3DMatrix44f mvpMatrix, int textureUnit);
    void UseTboShader(const M3DMatrix44f mvpMatrix, int maxWidth, int maxHeight, int textureUnit);
    //创建shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
};

#endif // SHADERMGR__
