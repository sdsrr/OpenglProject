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

class BaseShader
{
public:
    GLuint id;
    GLint mvMatrix;
    GLint mvpMatrix;
    GLint normalMatrix;
    GLint diffuseColor;
    GLint lightPosition;
    GLint cameraPosition;
    GLint colorMap[6];
    const char* vp;
    const char* fp;
public:
    BaseShader(const char* vp, const char* fp);
};

class BaseShaderParam
{
public:
    M3DVector4f environmentColor;
    M3DVector4f diffuseColor;
    M3DMatrix44f mvpMatrix;
    M3DMatrix44f mvMatrix;
    M3DMatrix33f normalMatrix;
    GLint colorMap[6];
    GLfloat lightPosition[3];
    GLfloat cameraPosition[3];
public:
    void SetDiffuseColor(M3DVector4f color);
    void SetEnvironmentColor(M3DVector4f color);
    void SetMVPMatrix(const M3DMatrix44f matrix);
    void SetMVMatrix(const M3DMatrix44f matrix);
    void SetNormalMatrix(const M3DMatrix44f matrix);
    void SetCameraPosition(GLfloat x, GLfloat y, GLfloat z);
    void SetLightPostion(GLfloat x, GLfloat y, GLfloat z);
    void SetLightPostion(GLfloat position[3]);
};

class ShaderMgr
{
    typedef void (ShaderMgr::*VoidDeldgate)();
private:
    std::map<ShaderType, VoidDeldgate> initfunctions;

    BaseShader* solidShader = new BaseShader("Tools/Shader/SolidColor/vertex.vp", "Tools/Shader/SolidColor/fragment.fp");
    BaseShader* diffuseShader = new BaseShader("Tools/Shader/Diffuse/vertex.vp", "Tools/Shader/Diffuse/fragment.fp");
    BaseShader* texture2dShader = new BaseShader("Tools/Shader/Texture2D/vertex.vp", "Tools/Shader/Texture2D/fragment.fp");

    BaseShader* texture2dArrayShader = new BaseShader("Tools/Shader/TextureArray/vertex.vp","Tools/Shader/TextureArray/fragment.fp");
    GLint texture2dArrayShader_iTime;

    BaseShader* cubeMapShader = new BaseShader("Tools/Shader/Cubemap/vertex.vp", "Tools/Shader/Cubemap/fragment.fp");
    BaseShader* textureSkybox = new BaseShader("Tools/Shader/SkyBox/vertex.vp","Tools/Shader/SkyBox/fragment.fp");

    BaseShader* textureSprite = new BaseShader("Tools/Shader/SpritePoint/vertex.vp", "Tools/Shader/SpritePoint/fragment.fp");
    GLint textureSprite_iSize;

    BaseShader* blurShader = new BaseShader("Tools/Shader/Blur/vertex.vp", "Tools/Shader/Blur/fragment.fp");

    BaseShader* tboShader = new BaseShader("Tools/Shader/TBO/vertex.vp", "Tools/Shader/TBO/fragment.fp");
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
    void InitBaseShader(BaseShader* shader);
    void InitBaseShaderParam(BaseShader* shader, const BaseShaderParam& param, bool print=false);
public:
    void UseSolidColor(const BaseShaderParam& param);
    void UseDiffuse(const BaseShaderParam& param);
    void UseTexture2d(const BaseShaderParam& param);
    void UseTextureArray(const BaseShaderParam& param, GLuint time);
    void UseCubeMap(const BaseShaderParam& param);
    void UseSkyBox(const BaseShaderParam& param);
    void UseSpritePoint(const BaseShaderParam& param, GLfloat size);
    void UseBlurShader(const BaseShaderParam& param);
    void UseTboShader(const BaseShaderParam& param, int maxWidth, int maxHeight);
    //创建shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
};

#endif // SHADERMGR__
