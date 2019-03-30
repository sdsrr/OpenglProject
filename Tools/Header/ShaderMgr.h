#ifndef SHADERMGR__
#define SHADERMGR__ XXX
#include "Tools.h"
enum ShaderType
{
    STSolid    = 1<<0,
    STDiffuse  = 1<<1,
    STTexture2d= 1<<2,
    STTextureArray = 1<<3,
    STCubemap = 1<<4,
    STSkybox = 1<<5,
    STTextureSprite = 1<<6,
    STBlur = 1<<7,
    STMax,
};



class ShaderMgr
{
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

public:
    ShaderMgr();
    void OnInit(int type);
    void OnUnInit();
    void InitSolid();
    void InitDiffuse();
    void InitTexture2d();
    void InitTextureArray();
    void InitCubemap();
    void InitTextureSkybox();
    void InitTextureSprite();
    void InitBlur();
public:
    void UseSolidColor(M3DVector4f color);
    void UseDiffuse(M3DVector4f color, M3DMatrix44f mvpMatrix);
    void UseDiffuse(M3DVector4f color, const M3DMatrix44f mvpMatrix);
    void UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture);
    void UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture, GLuint time);
    void UseCubeMap(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint cubeMap);
    void UseSkyBox(const M3DMatrix44f mvpMatrix, GLuint cubeMap);
    void UseSpritePoint(const M3DMatrix44f mvpMatrix, GLuint texture, GLfloat size);
    void UseBlurShader(const M3DMatrix44f mvpMatrix);
    //创建shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
};

#endif // SHADERMGR__
