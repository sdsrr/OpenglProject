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
    STBloor,
    STMSAA,
    STGeometry,
    STGrassInstance,
    STFeedback,
    STWriteFeedback,
    STMax,
};

class BaseShader
{
public:
    GLuint id = -1;
    GLint mvMatrix;
    GLint mvpMatrix;
    GLint projectMatrix;
    GLint normalMatrix;
    GLint diffuseColor;
    GLint lightDirection;
    GLint cameraPosition;
    GLint environmentColor;
    GLint time;
    GLint colorMap[6];
    const char* vp = NULL;
    const char* fp = NULL;
    const char* gp = NULL;
public:
    BaseShader(const char* vp, const char* fp);
    BaseShader(const char* vp, const char* gp, const char* fp);
    BaseShader(){};
};

class BaseShaderParam
{
public:
    M3DVector4f environmentColor;
    M3DVector4f diffuseColor;
    M3DMatrix44f mvpMatrix;
    M3DMatrix44f mvMatrix;
    M3DMatrix44f projectMatrix;
    M3DMatrix33f normalMatrix;
    int deltatime;
    GLint colorMap[6];
    GLfloat lightDirection[3];
    GLfloat cameraPosition[4];
public:
    void SetDiffuseColor(M3DVector4f color);
    void SetEnvironmentColor(M3DVector4f color);
    void SetMVPMatrix(const M3DMatrix44f matrix);
    void SetMVMatrix(const M3DMatrix44f matrix);
    void SetProjectMatrix(const M3DMatrix44f matrix);
    void SetNormalMatrix(const M3DMatrix44f matrix);
    void SetCameraPosition(GLfloat x, GLfloat y, GLfloat z);
    void SetLightDirection(GLfloat x, GLfloat y, GLfloat z);
    void SetLightDirection(GLfloat position[3]);
};

class ShaderMgr
{
    typedef void (ShaderMgr::*VoidDeldgate)(ShaderType type);
private:
    BaseShader* shaderList[(int)STMax];
    std::map<ShaderType, VoidDeldgate> initfunctions;

    GLint grassShader_iInstance;
    GLint grassShader_iTime;

    GLfloat geometryShader_iDelta;

    GLint texture2dArrayShader_iTime;

    GLint textureSprite_iSize;


    GLint tboShader_iMaxWidth;
    GLint tboShader_iMaxHeight;

    GLint hdrShader_iExposure;

    BaseShader* bloorNormalShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/normal.fp");
    BaseShader* bloorBrightShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/bright.fp");
    BaseShader* bloorBlurShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/blur.fp");
    BaseShader* bloorMixShader = new BaseShader("Tools/Shader/Bloor/vertex.vp", "Tools/Shader/Bloor/mix.fp");
    GLuint bloorBrightShader_iBrightLimit;
    GLint bloorMixShader_iExposure;
    GLint bloorMixShader_iBlurLevel;
    GLint bloorBlurShader_iOffset;

public:
    static GLfloat white[];
    static GLfloat ondine[];


public:
    ShaderMgr();
    void InitShaders();
    void InitFunctions();
    void OnInit(int type=-1);
    void OnUnInit();
    void Update();

    void InitBloor(ShaderType type);
    void InitTextureSprite(ShaderType type);
    void InitTextureArray(ShaderType type);
    void InitTbo(ShaderType type);
    void InitFBO(ShaderType type);
    void InitHDR(ShaderType type);
    void InitGeometry(ShaderType type);
    void InitGrassInstance(ShaderType type);
    void InitBaseShader(ShaderType type);
    void InitBaseShader(BaseShader* shader);
    void InitBaseShaderParam(BaseShader* shader, const BaseShaderParam& param);
public:
    GLuint GetShaderId(ShaderType type);
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

    void WriteFeedbackBuffer(const BaseShaderParam& param);
    void UseBloorBase(const BaseShaderParam& param);
    void UseBloorBright(const BaseShaderParam& param, float brightLimit);
    void UseBloorBlur(const BaseShaderParam& param, float offset[25]);
    void UseBloorMix(const BaseShaderParam& param, float exposure, float blurLevel);
    void UseMsaa(const BaseShaderParam& param);
    void UseFeedbackBuffer(const BaseShaderParam& param);
    void DrawNormal(const BaseShaderParam& param, float delta);
    void DrawGrass(const BaseShaderParam& param, GLint instance, float time);
    //创建shader
    GLuint LoadShader(const char* vertex, const char* geometry, const char* fragment);
    //加载shader资源
    bool LoadShaderFile(const char* file, GLuint shader);
    void PrintBaseShader(BaseShader* shader, const BaseShaderParam* param);
};

#endif // SHADERMGR__
