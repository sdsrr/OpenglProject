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
    STFBO,
    STHDR,
    STBLOOR,
    STMax,
};

class BaseShader
{
public:
    GLuint id = -1;
    GLint mvMatrix;
    GLint mvpMatrix;
    GLint normalMatrix;
    GLint diffuseColor;
    GLint lightDirection;
    GLint cameraPosition;
    GLint environmentColor;
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
    GLfloat lightDirection[3];
    GLfloat cameraPosition[4];
public:
    void SetDiffuseColor(M3DVector4f color);
    void SetEnvironmentColor(M3DVector4f color);
    void SetMVPMatrix(const M3DMatrix44f matrix);
    void SetMVMatrix(const M3DMatrix44f matrix);
    void SetNormalMatrix(const M3DMatrix44f matrix);
    void SetCameraPosition(GLfloat x, GLfloat y, GLfloat z);
    void SetLightDirection(GLfloat x, GLfloat y, GLfloat z);
    void SetLightDirection(GLfloat position[3]);
};

class ShaderMgr
{
    typedef void (ShaderMgr::*VoidDeldgate)();
private:
    std::map<ShaderType, VoidDeldgate> initfunctions;

    BaseShader* solidShader = new BaseShader("Tools/Shader/SolidColor/vertex.vp", "Tools/Shader/SolidColor/fragment.fp");
    BaseShader* diffuseShader = new BaseShader("Tools/Shader/Diffuse/vertex.vp", "Tools/Shader/Diffuse/fragment.fp");
    BaseShader* texture2dShader = new BaseShader("Tools/Shader/Texture2D/vertex.vp", "Tools/Shader/Texture2D/fragment.fp");
    BaseShader* cubeMapShader = new BaseShader("Tools/Shader/Cubemap/vertex.vp", "Tools/Shader/Cubemap/fragment.fp");
    BaseShader* textureSkybox = new BaseShader("Tools/Shader/SkyBox/vertex.vp","Tools/Shader/SkyBox/fragment.fp");
    BaseShader* blurShader = new BaseShader("Tools/Shader/Blur/vertex.vp", "Tools/Shader/Blur/fragment.fp");
    BaseShader* fboShader = new BaseShader("Tools/Shader/FBO/vertex.vp", "Tools/Shader/FBO/fragment.fp");

    BaseShader* texture2dArrayShader = new BaseShader("Tools/Shader/TextureArray/vertex.vp","Tools/Shader/TextureArray/fragment.fp");
    GLint texture2dArrayShader_iTime;

    BaseShader* textureSprite = new BaseShader("Tools/Shader/SpritePoint/vertex.vp", "Tools/Shader/SpritePoint/fragment.fp");
    GLint textureSprite_iSize;

    BaseShader* tboShader = new BaseShader("Tools/Shader/TBO/vertex.vp", "Tools/Shader/TBO/fragment.fp");
    GLint tboShader_iMaxWidth;
    GLint tboShader_iMaxHeight;

    BaseShader* hdrShader = new BaseShader("Tools/Shader/HDR/vertex.vp","Tools/Shader/HDR/fragment.fp");
    GLint hdrShader_iExposure;

    BaseShader* bloorNormalShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/normal.fp");
    BaseShader* bloorBrightShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/bright.fp");
    BaseShader* bloorBlurShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/blur.fp");
    BaseShader* bloorMixShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/mix.fp");

public:
    static GLfloat white[];
    static GLfloat ondine[];


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
    void InitFBO();
    void InitHDR();
    void InitBloor();
    void InitBaseShader(BaseShader* shader);
    void InitBaseShaderParam(BaseShader* shader, const BaseShaderParam& param);
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
    void DrawToFBO(const BaseShaderParam& param);
    void UseHDR(const BaseShaderParam& param, float exposure);

    void UseBloorNormal(const BaseShaderParam& param);
    void UseBloorBright(const BaseShaderParam& param);
    void UseBloorBlur(const BaseShaderParam& param);
    void UseBloorMix(const BaseShaderParam& param);

    //创建shader
    GLuint LoadShader(const char* vertex, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
    void PrintBaseShader(BaseShader* shader, const BaseShaderParam* param);
};

#endif // SHADERMGR__
