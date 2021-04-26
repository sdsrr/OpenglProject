#include "../Header/ShaderMgr.h"

BaseShader::BaseShader(const char* vp, const char* fp)
{
    this->vp = vp;
    this->fp = fp;
}

BaseShader::BaseShader(const char* vp, const char* gp, const char* fp)
{
    this->vp = vp;
    this->fp = fp;
    this->gp = gp;
}

void BaseShaderParam::SetDiffuseColor(M3DVector3f color, float alpha) {memcpy(diffuseColor, color, 4*sizeof(float)); diffuseColor[3] = alpha;}
void BaseShaderParam::SetDiffuseColor(M3DVector4f color) {memcpy(diffuseColor, color, 4*sizeof(float));}
void BaseShaderParam::SetEnvironmentColor(M3DVector4f color){memcpy(environmentColor, color, 4*sizeof(float));}
void BaseShaderParam::SetModelMatrix(const M3DMatrix44f matrix){memcpy(mMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetModelMatrix(const float matrix[4][4]){memcpy(mMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetViewMatrix(const M3DMatrix44f matrix){memcpy(vMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetProjectMatrix(const M3DMatrix44f matrix){memcpy(pMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetLightDirection(GLfloat direction[3]){memcpy(lightDirection, direction, 3*sizeof(float));}
void BaseShaderParam::SetCameraPosition(GLfloat x, GLfloat y, GLfloat z)
{
    cameraPosition[0] = x;
    cameraPosition[1] = y;
    cameraPosition[2] = z;
}

void BaseShaderParam::SetLightDirection(GLfloat x, GLfloat y, GLfloat z)
{
    lightDirection[0] = x;
    lightDirection[1] = y;
    lightDirection[2] = z;
}

GLfloat ShaderMgr::black[] = {0,0,0,1};
GLfloat ShaderMgr::red[] = {1,0,0,1};
GLfloat ShaderMgr::white[] = {1,1,1,1};
GLfloat ShaderMgr::ondine[] = {64/255.0, 68/255.0, 10/255.0, 1};
ShaderMgr* ShaderMgr::_instance = NULL;

ShaderMgr* ShaderMgr::GetInstance()
{
    if (_instance == NULL)
    {
        _instance = new ShaderMgr();
        _instance->OnInit();
    }
    return _instance;
};

void ShaderMgr::InitBaseShader(ShaderType type)
{
    BaseShader* shader = shaderList[(int)type];
    InitBaseShader(shader);
}

void ShaderMgr::InitBaseShader(BaseShader* shader)
{
    shader->id = LoadShader(Util::GetFullPath(shader->vp), Util::GetFullPath(shader->gp), Util::GetFullPath(shader->fp));
    shader->mMatrix = glGetUniformLocation(shader->id, "mMatrix");
    shader->vMatrix = glGetUniformLocation(shader->id, "vMatrix");
    shader->pMatrix = glGetUniformLocation(shader->id, "pMatrix");
    shader->lightDirection = glGetUniformLocation(shader->id, "lightDirection");
    shader->diffuseColor = glGetUniformLocation(shader->id, "diffuseColor");
    shader->cameraPosition = glGetUniformLocation(shader->id, "cameraPosition");
    shader->environmentColor = glGetUniformLocation(shader->id, "environmentColor");
    shader->time = glGetUniformLocation(shader->id, "time");

    shader->colorMap[0] = glGetUniformLocation(shader->id, "colorMap00");
    shader->colorMap[1] = glGetUniformLocation(shader->id, "colorMap01");
    shader->colorMap[2] = glGetUniformLocation(shader->id, "colorMap02");
    shader->colorMap[3] = glGetUniformLocation(shader->id, "colorMap03");
    shader->colorMap[4] = glGetUniformLocation(shader->id, "colorMap04");
    shader->colorMap[5] = glGetUniformLocation(shader->id, "colorMap05");
}

ShaderMgr::ShaderMgr()
{
    InitShaders();
    InitFunctions();
}

GLShaderManager* ShaderMgr::GetStandardShaderMgr()
{
    return _standardShardMgr;
}

void ShaderMgr::InitShaders()
{
    shaderList[STSolid] = new BaseShader("Resource/Shader/SolidColor/vertex.vp", "Resource/Shader/SolidColor/fragment.fp");
    shaderList[STDiffuse] = new BaseShader("Resource/Shader/Diffuse/vertex.vp", "Resource/Shader/Diffuse/fragment.fp");
    shaderList[STTexture2d] = new BaseShader("Resource/Shader/Texture2D/vertex.vp", "Resource/Shader/Texture2D/fragment.fp");
    shaderList[STCubemap] = new BaseShader("Resource/Shader/Cubemap/vertex.vp", "Resource/Shader/Cubemap/fragment.fp");
    shaderList[STSkybox] = new BaseShader("Resource/Shader/SkyBox/vertex.vp","Resource/Shader/SkyBox/fragment.fp");
    shaderList[STBlur] = new BaseShader("Resource/Shader/Blur/vertex.vp", "Resource/Shader/Blur/fragment.fp");
    shaderList[STFBO] = new BaseShader("Resource/Shader/FBO/vertex.vp", "Resource/Shader/FBO/fragment.fp");
    shaderList[STMSAA] = new BaseShader("Resource/Shader/TexMsaa/vertex.vp", "Resource/Shader/TexMsaa/fragment.fp");
    shaderList[STFeedback] = new BaseShader("Resource/Shader/FeedBack/vertex.vp", "Resource/Shader/FeedBack/fragment.fp");
    shaderList[STWriteFeedback] = new BaseShader("Resource/Shader/FeedBack/vertex_out.vp", NULL);
    shaderList[STGrassInstance] = new BaseShader("Resource/Shader/GrassInstance/vertex.vp", "Resource/Shader/GrassInstance/fragment.fp");
    shaderList[STGeometry] = new BaseShader("Resource/Shader/GeometryNormal/vertex.vp", "Resource/Shader/GeometryNormal/geometry.gp", "Resource/Shader/GeometryNormal/fragment.fp");
    shaderList[STTextureArray] = new BaseShader("Resource/Shader/TextureArray/vertex.vp","Resource/Shader/TextureArray/fragment.fp");
    shaderList[STTextureSprite] = new BaseShader("Resource/Shader/SpritePoint/vertex.vp", "Resource/Shader/SpritePoint/fragment.fp");
    shaderList[STTBO] = new BaseShader("Resource/Shader/TBO/vertex.vp", "Resource/Shader/TBO/fragment.fp");
    shaderList[STHDR] = new BaseShader("Resource/Shader/HDR/vertex.vp","Resource/Shader/HDR/fragment.fp");
    shaderList[STFont] = new BaseShader("Resource/Shader/Font/vertex.vp","Resource/Shader/Font/fragment.fp");
    shaderList[STOutShadowmap] = new BaseShader("Resource/Shader/Shadowmap/vertex_outshadow.vp","Resource/Shader/Shadowmap/fragment_outshadow.fp");
    shaderList[STShadowmap] = new BaseShader("Resource/Shader/Shadowmap/vertex_shadow.vp","Resource/Shader/Shadowmap/fragment_shadow.fp");
    shaderList[STCameraBox] = new BaseShader("Resource/Shader/CameraBox/vertex_normal.vp","Resource/Shader/CameraBox/fragment_normal.fp");
    shaderList[STCameraLine] = new BaseShader("Resource/Shader/CameraBox/vertex_line.vp","Resource/Shader/CameraBox/fragment_line.fp");
    shaderList[STDeferredOut] = new BaseShader("Resource/Shader/DeferredRender/vertex_out.vp","Resource/Shader/DeferredRender/fragment_out.fp");
    shaderList[STDeferredIn] = new BaseShader("Resource/Shader/DeferredRender/vertex_in.vp","Resource/Shader/DeferredRender/fragment_in.fp");
    shaderList[STSphereLight] = new BaseShader("Resource/Shader/SphereLight/vertex.vp","Resource/Shader/SphereLight/fragment.fp");
    shaderList[STSSAO] = new BaseShader("Resource/Shader/SSAO/vertex.vp","Resource/Shader/SSAO/fragment.fp");
    shaderList[STSSAODeferredIn] = new BaseShader("Resource/Shader/SSAO/vertex_deferred_in.vp","Resource/Shader/SSAO/fragment_deferred_in.fp");
    shaderList[STCalcSSAO] = new BaseShader("Resource/Shader/SSAO/vertex_ssao.vp","Resource/Shader/SSAO/fragment_ssao.fp");
}

void ShaderMgr::InitFunctions()
{
    initfunctions[STSolid] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STDiffuse] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STTexture2d] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STTextureArray] = (VoidDeldgate)&ShaderMgr::InitTextureArray;
    initfunctions[STCubemap] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STSkybox] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STTextureSprite] = (VoidDeldgate)&ShaderMgr::InitTextureSprite;
    initfunctions[STBlur] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STTBO] = (VoidDeldgate)&ShaderMgr::InitTbo;
    initfunctions[STFBO] = (VoidDeldgate)&ShaderMgr::InitFBO;
    initfunctions[STHDR] = (VoidDeldgate)&ShaderMgr::InitHDR;
    initfunctions[STBloor] = (VoidDeldgate)&ShaderMgr::InitBloor;
    initfunctions[STMSAA] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STGeometry] = (VoidDeldgate)&ShaderMgr::InitGeometry;
    initfunctions[STGrassInstance] = (VoidDeldgate)&ShaderMgr::InitGrassInstance;
    initfunctions[STFeedback] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STWriteFeedback] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STFont] = (VoidDeldgate)&ShaderMgr::InitFont;
    initfunctions[STOutShadowmap] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STShadowmap] = (VoidDeldgate)&ShaderMgr::InitShadowmap;
    initfunctions[STCameraBox] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STCameraLine] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STDeferredOut] = (VoidDeldgate)&ShaderMgr::InitBaseShader;
    initfunctions[STDeferredIn] = (VoidDeldgate)&ShaderMgr::InitDeferredIn;
    initfunctions[STSphereLight] = (VoidDeldgate)&ShaderMgr::InitSphereLight;
    initfunctions[STSSAO] = (VoidDeldgate)&ShaderMgr::InitSSAO;
    initfunctions[STSSAODeferredIn] = (VoidDeldgate)&ShaderMgr::InitSSAODeferredIn;
    initfunctions[STCalcSSAO] = (VoidDeldgate)&ShaderMgr::InitSTCalcSSAO;
}

void ShaderMgr::InitSTCalcSSAO(ShaderType type)
{
    BaseShader* shader = shaderList[(int)type];
    InitBaseShader(shader);
    ssaoCalShader_iKernel = glGetUniformLocation(shader->id, "samples");
    ssaoCalShader_iScene = glGetUniformLocation(shader->id, "scene");
}

void ShaderMgr::InitSSAODeferredIn(ShaderType type)
{
    BaseShader* shader = shaderList[(int)type];
    InitBaseShader(shader);
    ssaoShader_iFarPlane = glGetUniformLocation(shader->id, "farPlane");
    ssaoShader_iNearPlane = glGetUniformLocation(shader->id, "nearPlane");
    glBindFragDataLocation(shader->id, 0, "vPosition");
    glBindFragDataLocation(shader->id, 1, "vNormal");
    glLinkProgram(shader->id);
}

void ShaderMgr::InitSSAO(ShaderType type)
{
    BaseShader* shader = shaderList[(int)type];
    InitBaseShader(shader);
}

void ShaderMgr::InitDeferredIn(ShaderType type)
{
    BaseShader* deferredShader = shaderList[(int)type];
    InitBaseShader(deferredShader);
    glBindFragDataLocation(deferredShader->id, 0, "gPosition");
    glBindFragDataLocation(deferredShader->id, 1, "gNormal");
    glLinkProgram(deferredShader->id);
}

void ShaderMgr::InitSphereLight(ShaderType type)
{
    BaseShader* lightShader = shaderList[(int)type];
    InitBaseShader(lightShader);
}

void ShaderMgr::InitShadowmap(ShaderType type)
{
    BaseShader* shadowShader = shaderList[(int)type];
    InitBaseShader(shadowShader);
    shadowmapShader_iLightMatrix = glGetUniformLocation(shadowShader->id, "lightMatrix");
}

void ShaderMgr::InitFont(ShaderType type)
{
    BaseShader* fontShader = shaderList[(int)type];
    InitBaseShader(fontShader);
    fontShader_icolor = glGetUniformLocation(fontShader->id, "color");
}

void ShaderMgr::InitGrassInstance(ShaderType type)
{
    BaseShader* grassShader = shaderList[(int)type];
    InitBaseShader(grassShader);
    grassShader_iInstance = glGetUniformLocation(grassShader->id, "instance");
    grassShader_iTime = glGetUniformLocation(grassShader->id, "time");
}

void ShaderMgr::InitGeometry(ShaderType type)
{
    BaseShader* geometryShader = shaderList[(int)type];
    InitBaseShader(geometryShader);
    geometryShader_iDelta = glGetUniformLocation(geometryShader->id, "delta");
}

void ShaderMgr::InitBloor(ShaderType type)
{
    InitBaseShader(bloorNormalShader);
    InitBaseShader(bloorBrightShader);
    InitBaseShader(bloorBlurShader);
    InitBaseShader(bloorMixShader);

    glBindFragDataLocation(bloorNormalShader->id, 0, "baseColor");
    glBindFragDataLocation(bloorBrightShader->id, 0, "brightColor");
    glBindFragDataLocation(bloorBlurShader->id, 0, "blurColor");
    bloorMixShader_iExposure = glGetUniformLocation(bloorMixShader->id, "exposure");
    bloorMixShader_iBlurLevel = glGetUniformLocation(bloorMixShader->id, "blurLevel");
    bloorBrightShader_iBrightLimit = glGetUniformLocation(bloorBrightShader->id, "brightLimit");
    bloorBlurShader_iOffset = glGetUniformLocation(bloorBlurShader->id, "offset");
}

void ShaderMgr::InitHDR(ShaderType type)
{
    BaseShader* hdrShader = shaderList[(int)type];
    InitBaseShader(hdrShader);
    hdrShader_iExposure = glGetUniformLocation(hdrShader->id, "exposure");
}

void ShaderMgr::InitFBO(ShaderType type)
{
    BaseShader* fboShader = shaderList[(int)type];
    InitBaseShader(fboShader);
    glBindFragDataLocation(fboShader->id,0,"originColor");
    glBindFragDataLocation(fboShader->id,1,"grayColor");
    glBindFragDataLocation(fboShader->id,2,"luminanceColor");
    glLinkProgram(fboShader->id);
    //PrintBaseShader(fboShader, NULL);
}

void ShaderMgr::InitTbo(ShaderType type)
{
    BaseShader* tboShader = shaderList[(int)type];
    InitBaseShader(tboShader);
    tboShader_iMaxWidth = glGetUniformLocation(tboShader->id, "maxWidth");
    tboShader_iMaxHeight = glGetUniformLocation(tboShader->id, "maxHeight");
}

void ShaderMgr::InitTextureArray(ShaderType type)
{
    //2d纹理数组
    BaseShader* texture2dArrayShader = shaderList[(int)type];
    InitBaseShader(texture2dArrayShader);
    texture2dArrayShader_iTime = glGetUniformLocation(texture2dArrayShader->id, "time");
}

void ShaderMgr::InitTextureSprite(ShaderType type)
{
    //spritepoint
    BaseShader* textureSprite = shaderList[(int)type];
    InitBaseShader(textureSprite);
    textureSprite_iSize = glGetUniformLocation(textureSprite->id, "size");
}

void ShaderMgr::OnInit(int type)
{
    for (int i = 0; i < STMax; i++)
    {
        if (type == -1 || (type & (1<<i)) > 0)
        {
            VoidDeldgate fn = initfunctions[(ShaderType)i];
            (this->*fn)((ShaderType)i);
            printf("init shader %d\n", i);
        }
    }
    _standardShardMgr = new GLShaderManager();
    _standardShardMgr->InitializeStockShaders();
}

void ShaderMgr::OnUnInit()
{
    for (int i = 0 ; i < (int)STMax; i++)
    {
        BaseShader* shader = shaderList[(int)i];
        glDeleteShader(shader->id);
    }
    glDeleteShader(bloorNormalShader->id);
    glDeleteShader(bloorBrightShader->id);
    glDeleteShader(bloorBlurShader->id);
    glDeleteShader(bloorMixShader->id);
}

bool ShaderMgr::LoadShaderFile(const char* filePath, GLuint shader)
{
    FILE* file = fopen(filePath, "r");
    if (file == NULL)
        return false;

    int index = 0;
    char buffer[4096]="";
    while(!feof(file))
    {
        char tmpBuf[256]="";
        fgets(tmpBuf, 256, file);
        strcpy(&buffer[index], tmpBuf);
        index += strlen(tmpBuf);
    }

    //需要大小与内容完全合适的buf
    char *source = (char*)malloc(strlen(buffer)+1);
    strcpy(source, buffer);
    //printf(source);

    glShaderSource(shader, 1, (char**)&source, NULL);
    fclose(file);
    free(source);
    return true;
}

GLuint ShaderMgr::LoadShader(const char* fileVertex, const char* fileGeometry, const char* fileFragment)
{
    GLuint geometryShader = -1;
    GLuint vertexShader = -1;
    GLuint fragmentShader = -1;

    // init vertex shader
    if (fileVertex != NULL)
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // load file
        if (LoadShaderFile(fileVertex, vertexShader) == false)
        {
            UtilPrint::PrintString(3, (char*)"the vertex shader at ", fileVertex, (char*)" not fond.");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(geometryShader);
            return 0;
        }
        // compile
        GLint result = GL_TRUE;
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            char logs[1024];
            glGetShaderInfoLog(vertexShader, 1024, NULL, logs);
            UtilPrint::PrintString(2, (char*)"vertext shader compile faile ", logs);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(geometryShader);
            return 0;
        }
    }

    // init geometry shader
    if (fileGeometry != NULL)
    {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        if (LoadShaderFile(fileGeometry, geometryShader) == false)
        {
            UtilPrint::PrintString(3, (char*)"the geometry shader at ", fileGeometry, (char*)" not fond.");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(geometryShader);
            return 0;
        }
        GLint result = GL_TRUE;
        glCompileShader(geometryShader);
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            char logs[1024];
            glGetShaderInfoLog(geometryShader, 1024, NULL, logs);
            UtilPrint::PrintString(2, (char*)"geometry shader compile faile ", logs);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            glDeleteShader(geometryShader);
            return 0;
        }
    }

    // init fragment shader
    if (fileFragment != NULL)
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (LoadShaderFile(fileFragment, fragmentShader) == false)
        {
            UtilPrint::PrintString(3, (char*)"the fragment shader at ", fileFragment, (char*)" not fond.");
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }
        GLint result = GL_TRUE;
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            char logs[1024];
            glGetShaderInfoLog(fragmentShader, 1024, NULL, logs);
            UtilPrint::PrintString(2, (char*)"fragment shader compile faile ", logs);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }
    }

    //link
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, geometryShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);

    //delete vertex/fragment shader
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
    glDeleteShader(fragmentShader);
    return program;
}

void ShaderMgr::InitBaseShaderParam(BaseShader* shader, const BaseShaderParam& param)
{
    if (shader != NULL)
    {
        glUseProgram(shader->id);
        //glUniformMatrix4fv param3:false列优先 true行优先(二位数组[][]为行优先，[]一维数据如M3DMatrix44f为列优先)
        glUniformMatrix4fv(shader->mMatrix, 1, GL_FALSE, param.mMatrix);
        glUniformMatrix4fv(shader->vMatrix, 1, GL_FALSE, param.vMatrix);
        glUniformMatrix4fv(shader->pMatrix, 1, GL_FALSE, param.pMatrix);
        glUniform3fv(shader->lightDirection, 1, param.lightDirection);
        glUniform4fv(shader->cameraPosition, 1, param.cameraPosition);
        glUniform4fv(shader->diffuseColor, 1, param.diffuseColor);
        glUniform4fv(shader->environmentColor, 1, param.environmentColor);
        glUniform1i(shader->time, param.deltatime);
        for (int i=0; i<6; i++)
            glUniform1i(shader->colorMap[i], param.colorMap[i]);
    }
}

void ShaderMgr::PrintBaseShader(BaseShader* shader, const BaseShaderParam* param)
{
    if (shader != NULL)
    {
        M3DMatrix44f identity;
        m3dLoadIdentity44(identity);
        printf("shader id %d\n", shader->id);

        printf("modelMatrix %d\n", shader->mMatrix);
        UtilPrint::PrintMatrix44f(param != NULL ? param->mMatrix : identity);
        printf("viewMatrix %d\n", shader->vMatrix);
        UtilPrint::PrintMatrix44f(param != NULL ? param->vMatrix : identity);
        printf("projectMatrix %d\n", shader->pMatrix);
        UtilPrint::PrintMatrix44f(param != NULL ? param->pMatrix : identity);

        printf("colorMap00 %d %d\n", shader->colorMap[0], param != NULL ? param->colorMap[0] : -1);
        printf("colorMap01 %d %d\n", shader->colorMap[1], param != NULL ? param->colorMap[1] : -1);
        printf("colorMap02 %d %d\n", shader->colorMap[2], param != NULL ? param->colorMap[2] : -1);
        printf("colorMap03 %d %d\n", shader->colorMap[3], param != NULL ? param->colorMap[3] : -1);
        printf("colorMap04 %d %d\n", shader->colorMap[4], param != NULL ? param->colorMap[4] : -1);
        printf("colorMap05 %d %d\n\n", shader->colorMap[5], param != NULL ? param->colorMap[5] : -1);
    }
}

void ShaderMgr::UseSolidColor(const BaseShaderParam& param)
{
    BaseShader* solidShader = shaderList[(int)STSolid];
    InitBaseShaderParam(solidShader, param);
}

void ShaderMgr::UseDiffuse(const BaseShaderParam& param)
{
    //调用useprogram之后设置参数
    BaseShader* diffuseShader = shaderList[(int)STDiffuse];
    InitBaseShaderParam(diffuseShader, param);
    PrintBaseShader(diffuseShader, &param);
}

void ShaderMgr::UseTexture2d(const BaseShaderParam& param)
{
    BaseShader* texture2dShader = shaderList[(int)STTexture2d];
    InitBaseShaderParam(texture2dShader, param);
}

void ShaderMgr::UseTextureArray(const BaseShaderParam& param, GLuint time)
{
    BaseShader* texture2dArrayShader = shaderList[(int)STTextureArray];
    InitBaseShaderParam(texture2dArrayShader, param);
    glUniform1f(texture2dArrayShader_iTime, time);
}

void ShaderMgr::UseCubeMap(const BaseShaderParam& param)
{
    BaseShader* cubeMapShader = shaderList[(int)STCubemap];
    InitBaseShaderParam(cubeMapShader, param);
}

void ShaderMgr::UseSkyBox(const BaseShaderParam& param)
{
    BaseShader* textureSkybox = shaderList[(int)STSkybox];
    InitBaseShaderParam(textureSkybox, param);
}

void ShaderMgr::UseSpritePoint(const BaseShaderParam& param, GLfloat size)
{
    BaseShader* textureSprite = shaderList[(int)STTextureSprite];
    InitBaseShaderParam(textureSprite, param);
    glUniform1f(textureSprite_iSize, size);
}

void ShaderMgr::UseBlurShader(const BaseShaderParam& param)
{
    BaseShader* blurShader = shaderList[(int)STBlur];
    InitBaseShaderParam(blurShader, param);
}

void ShaderMgr::UseTboShader(const BaseShaderParam& param, int maxWidth, int maxHeight)
{
    BaseShader* tboShader = shaderList[(int)STTBO];
    InitBaseShaderParam(tboShader, param);
    glUniform1i(tboShader_iMaxWidth, maxWidth);
    glUniform1i(tboShader_iMaxHeight, maxHeight);
}

void ShaderMgr::DrawToFBO(const BaseShaderParam& param)
{
    BaseShader* fboShader = shaderList[(int)STFBO];
    InitBaseShaderParam(fboShader, param);
}

void ShaderMgr::UseHDR(const BaseShaderParam& param, float exposure)
{
    BaseShader* hdrShader = shaderList[(int)STHDR];
    InitBaseShaderParam(hdrShader, param);
    glUniform1f(hdrShader_iExposure, exposure);
}

void ShaderMgr::UseBloorBase(const BaseShaderParam& param)
{
    InitBaseShaderParam(bloorNormalShader, param);
}

void ShaderMgr::UseBloorBright(const BaseShaderParam& param, float brightLimit)
{
    InitBaseShaderParam(bloorBrightShader, param);
    glUniform1f(bloorBrightShader_iBrightLimit, brightLimit);
}

void ShaderMgr::UseBloorBlur(const BaseShaderParam& param, float offset[25])
{
    InitBaseShaderParam(bloorBlurShader, param);
    glUniform2fv(bloorBlurShader_iOffset, 25, offset);
}

void ShaderMgr::UseBloorMix(const BaseShaderParam& param, float exposure, float blurLevel)
{
    InitBaseShaderParam(bloorMixShader, param);
    glUniform1f(bloorMixShader_iBlurLevel, blurLevel);
    glUniform1f(bloorMixShader_iExposure, exposure);
}

void ShaderMgr::UseMsaa(const BaseShaderParam& param)
{
    BaseShader* msaaShader = shaderList[(int)STMSAA];
    InitBaseShaderParam(msaaShader, param);
}

void ShaderMgr::DrawNormal(const BaseShaderParam& param, float delta)
{
    BaseShader* geometryShader = shaderList[(int)STGeometry];
    InitBaseShaderParam(geometryShader, param);
    glUniform1f(geometryShader_iDelta, delta);
}

void ShaderMgr::DrawGrass(const BaseShaderParam& param, GLint instance, float time)
{
    BaseShader* grassShader = shaderList[(int)STGrassInstance];
    InitBaseShaderParam(grassShader, param);
    glUniform1i(grassShader_iInstance, instance);
    glUniform1f(grassShader_iTime, time);
}

void ShaderMgr::UseFeedbackBuffer(const BaseShaderParam& param)
{
    BaseShader* feedbackShader = shaderList[(int)STFeedback];
    InitBaseShaderParam(feedbackShader, param);
}

void ShaderMgr::WriteFeedbackBuffer(const BaseShaderParam& param)
{
    BaseShader* writeFeedbackShader = shaderList[(int)STWriteFeedback];
    InitBaseShaderParam(writeFeedbackShader, param);
}

GLuint ShaderMgr::GetShaderId(ShaderType type)
{
    BaseShader* shader = shaderList[(int)type];
    return shader != NULL ? shader->id : -1;
}

void ShaderMgr::UseFont(const BaseShaderParam& param, float color[4])
{
    BaseShader* fontShader = shaderList[(int)STFont];
    InitBaseShaderParam(fontShader, param);
    //默认字体图片使用GL_TEXTURE31
    glUniform1i(fontShader->colorMap[0], 31);
    glUniform4fv(fontShader_icolor, 1, color);
}

void ShaderMgr::WriteToShadowmap(const BaseShaderParam& param)
{
    BaseShader* outShadowShader = shaderList[(int)STOutShadowmap];
    InitBaseShaderParam(outShadowShader, param);
}

void ShaderMgr::UseShadowmap(const BaseShaderParam& param, const M3DMatrix44f lightMat)
{
    BaseShader* shadowShader = shaderList[(int)STShadowmap];
    InitBaseShaderParam(shadowShader, param);
    glUniformMatrix4fv(shadowmapShader_iLightMatrix, 1, true, lightMat);
}

void ShaderMgr::UseCameraBox(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STCameraBox];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseCameraLine(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STCameraLine];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseDeferredOut(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STDeferredOut];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseDeferredIn(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STDeferredIn];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseSphereLight(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STSphereLight];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseSSAO(const BaseShaderParam& param)
{
    BaseShader* shader = shaderList[(int)STSSAO];
    InitBaseShaderParam(shader, param);
}

void ShaderMgr::UseSSAODeferred(const BaseShaderParam& param, float nearPlane, float farPlane)
{
    BaseShader* shader = shaderList[(int)STSSAODeferredIn];
    InitBaseShaderParam(shader, param);
    glUniform1f(ssaoShader_iFarPlane, farPlane);
    glUniform1f(ssaoShader_iNearPlane, nearPlane);
}

void ShaderMgr::UseCalcSSAO(const BaseShaderParam& param, float length, float width)
{
    BaseShader* shader = shaderList[(int)STCalcSSAO];
    InitBaseShaderParam(shader, param);
    glUniform2f(ssaoCalShader_iScene, length, width);
}
