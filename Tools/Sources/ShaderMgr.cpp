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

void BaseShaderParam::SetDiffuseColor(M3DVector4f color) {memcpy(diffuseColor, color, 4*sizeof(float));}
void BaseShaderParam::SetEnvironmentColor(M3DVector4f color){memcpy(environmentColor, color, 4*sizeof(float));}
void BaseShaderParam::SetMVPMatrix(const M3DMatrix44f matrix){memcpy(mvpMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetProjectMatrix(const M3DMatrix44f matrix){memcpy(projectMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetMVMatrix(const M3DMatrix44f matrix){memcpy(mvMatrix, matrix, 16*sizeof(float));}
void BaseShaderParam::SetNormalMatrix(const M3DMatrix44f matrix){memcpy(normalMatrix, matrix, 16*sizeof(float));};
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
    shader->mvpMatrix = glGetUniformLocation(shader->id, "mvpMatrix");
    shader->mvMatrix = glGetUniformLocation(shader->id, "mvMatrix");
    shader->projectMatrix = glGetUniformLocation(shader->id, "projectMatrix");
    shader->normalMatrix = glGetUniformLocation(shader->id, "normalMatrix");
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

void ShaderMgr::InitShaders()
{
    shaderList[STSolid] = new BaseShader("Tools/Shader/SolidColor/vertex.vp", "Tools/Shader/SolidColor/fragment.fp");
    shaderList[STDiffuse] = new BaseShader("Tools/Shader/Diffuse/vertex.vp", "Tools/Shader/Diffuse/fragment.fp");
    shaderList[STTexture2d] = new BaseShader("Tools/Shader/Texture2D/vertex.vp", "Tools/Shader/Texture2D/fragment.fp");
    shaderList[STCubemap] = new BaseShader("Tools/Shader/Cubemap/vertex.vp", "Tools/Shader/Cubemap/fragment.fp");
    shaderList[STSkybox] = new BaseShader("Tools/Shader/SkyBox/vertex.vp","Tools/Shader/SkyBox/fragment.fp");
    shaderList[STBlur] = new BaseShader("Tools/Shader/Blur/vertex.vp", "Tools/Shader/Blur/fragment.fp");
    shaderList[STFBO] = new BaseShader("Tools/Shader/FBO/vertex.vp", "Tools/Shader/FBO/fragment.fp");
    shaderList[STMSAA] = new BaseShader("Tools/Shader/TexMsaa/vertex.vp", "Tools/Shader/TexMsaa/fragment.fp");
    shaderList[STFeedback] = new BaseShader("Tools/Shader/FeedBack/vertex.vp", "Tools/Shader/FeedBack/fragment.fp");
    shaderList[STWriteFeedback] = new BaseShader("Tools/Shader/FeedBack/vertex_out.vp", NULL);
    shaderList[STGrassInstance] = new BaseShader("Tools/Shader/GrassInstance/vertex.vp", "Tools/Shader/GrassInstance/fragment.fp");
    shaderList[STGeometry] = new BaseShader("Tools/Shader/GeometryNormal/vertex.vp", "Tools/Shader/GeometryNormal/geometry.gp", "Tools/Shader/GeometryNormal/fragment.fp");
    shaderList[STTextureArray] = new BaseShader("Tools/Shader/TextureArray/vertex.vp","Tools/Shader/TextureArray/fragment.fp");
    shaderList[STTextureSprite] = new BaseShader("Tools/Shader/SpritePoint/vertex.vp", "Tools/Shader/SpritePoint/fragment.fp");
    shaderList[STTBO] = new BaseShader("Tools/Shader/TBO/vertex.vp", "Tools/Shader/TBO/fragment.fp");
    shaderList[STHDR] = new BaseShader("Tools/Shader/HDR/vertex.vp","Tools/Shader/HDR/fragment.fp");
    shaderList[STFont] = new BaseShader("Tools/Shader/Font/vertex.vp","Tools/Shader/Font/fragment.fp");
    shaderList[STOutShadowmap] = new BaseShader("Tools/Shader/Shadowmap/vertex_outshadow.vp","Tools/Shader/Shadowmap/fragment_outshadow.fp");
    shaderList[STShadowmap] = new BaseShader("Tools/Shader/Shadowmap/vertex_shadow.vp","Tools/Shader/Shadowmap/fragment_shadow.fp");
    shaderList[STCameraBox] = new BaseShader("Tools/Shader/CameraBox/vertex_normal.vp","Tools/Shader/CameraBox/fragment_normal.fp");
    shaderList[STCameraLine] = new BaseShader("Tools/Shader/CameraBox/vertex_line.vp","Tools/Shader/CameraBox/fragment_line.fp");
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
            Util::PrintString(3, (char*)"the vertex shader at ", fileVertex, (char*)" not fond.");
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
            Util::PrintString(2, (char*)"vertext shader compile faile ", logs);
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
            Util::PrintString(3, (char*)"the geometry shader at ", fileGeometry, (char*)" not fond.");
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
            Util::PrintString(2, (char*)"geometry shader compile faile ", logs);
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
            Util::PrintString(3, (char*)"the fragment shader at ", fileFragment, (char*)" not fond.");
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
            Util::PrintString(2, (char*)"fragment shader compile faile ", logs);
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
        glUniformMatrix4fv(shader->mvpMatrix, 1, GL_TRUE, param.mvpMatrix);
        glUniformMatrix4fv(shader->mvMatrix, 1, GL_TRUE, param.mvMatrix);
        glUniformMatrix4fv(shader->projectMatrix, 1, GL_TRUE, param.projectMatrix);
        glUniformMatrix3fv(shader->normalMatrix, 1, GL_TRUE, param.normalMatrix);
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

        printf("mvpMatrix %d\n", shader->mvpMatrix);
        Util::PrintMatrix44f(param != NULL ? param->mvpMatrix : identity);

        printf("mvMatrix %d\n", shader->mvMatrix);
        Util::PrintMatrix44f(param != NULL ? param->mvMatrix : identity);

        printf("normalMatrix %d\n", shader->normalMatrix);
        Util::PrintMatrix44f(param != NULL ? param->normalMatrix : identity);

        printf("projectMatrix %d\n", shader->projectMatrix);
        Util::PrintMatrix44f(param != NULL ? param->projectMatrix : identity);

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
