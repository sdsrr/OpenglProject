#include "../Header/ShaderMgr.h"

BaseShader::BaseShader(const char* vp, const char* fp)
{
    this->vp = vp;
    this->fp = fp;
}

void BaseShaderParam::SetDiffuseColor(M3DVector4f color) {memcpy(diffuseColor, color, 4*sizeof(float));}
void BaseShaderParam::SetEnvironmentColor(M3DVector4f color){memcpy(environmentColor, color, 4*sizeof(float));}
void BaseShaderParam::SetMVPMatrix(const M3DMatrix44f matrix){memcpy(mvpMatrix, matrix, 16*sizeof(float));}
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

GLfloat ShaderMgr::white[] = {1,1,1,1};
GLfloat ShaderMgr::ondine[] = {64/255.0, 68/255.0, 10/255.0, 1};

ShaderMgr::ShaderMgr()
{
    initfunctions[STSolid] = (VoidDeldgate)&ShaderMgr::InitSolid;
    initfunctions[STDiffuse] = (VoidDeldgate)&ShaderMgr::InitDiffuse;
    initfunctions[STTexture2d] = (VoidDeldgate)&ShaderMgr::InitTexture2d;
    initfunctions[STTextureArray] = (VoidDeldgate)&ShaderMgr::InitTextureArray;
    initfunctions[STCubemap] = (VoidDeldgate)&ShaderMgr::InitCubemap;
    initfunctions[STSkybox] = (VoidDeldgate)&ShaderMgr::InitTextureSkybox;
    initfunctions[STTextureSprite] = (VoidDeldgate)&ShaderMgr::InitTextureSprite;
    initfunctions[STBlur] = (VoidDeldgate)&ShaderMgr::InitBlur;
    initfunctions[STTBO] = (VoidDeldgate)&ShaderMgr::InitTbo;
    initfunctions[STFBO] = (VoidDeldgate)&ShaderMgr::InitFBO;
    initfunctions[STHDR] = (VoidDeldgate)&ShaderMgr::InitHDR;
    initfunctions[STBLOOR] = (VoidDeldgate)&ShaderMgr::InitBloor;

}

void ShaderMgr::InitBaseShader(BaseShader* shader)
{
    shader->id = LoadShader(Util::GetFullPath(shader->vp), Util::GetFullPath(shader->fp));
    shader->mvpMatrix = glGetUniformLocation(shader->id, "mvpMatrix");
    shader->mvMatrix = glGetUniformLocation(shader->id, "mvMatrix");
    shader->normalMatrix = glGetUniformLocation(shader->id, "normalMatrix");
    shader->lightDirection = glGetUniformLocation(shader->id, "lightDirection");
    shader->diffuseColor = glGetUniformLocation(shader->id, "diffuseColor");
    shader->cameraPosition = glGetUniformLocation(shader->id, "cameraPosition");
    shader->environmentColor = glGetUniformLocation(shader->id, "environmentColor");

    shader->colorMap[0] = glGetUniformLocation(shader->id, "colorMap00");
    shader->colorMap[1] = glGetUniformLocation(shader->id, "colorMap01");
    shader->colorMap[2] = glGetUniformLocation(shader->id, "colorMap02");
    shader->colorMap[3] = glGetUniformLocation(shader->id, "colorMap03");
    shader->colorMap[4] = glGetUniformLocation(shader->id, "colorMap04");
    shader->colorMap[5] = glGetUniformLocation(shader->id, "colorMap05");
}

void ShaderMgr::InitBloor()
{
    InitBaseShader(bloorNormalShader);
    InitBaseShader(bloorBrightShader);
    InitBaseShader(bloorBlurShader);
    InitBaseShader(bloorMixShader);

    glBindFragDataLocation(bloorNormalShader->id, 0, "baseColor");
    glBindFragDataLocation(bloorBrightShader->id, 0, "brightColor");
    glBindFragDataLocation(bloorBlurShader->id, 0, "blurColor");
    bloorMixShader_iExposure = glGetUniformLocation(bloorMixShader->id, "exposure");
    bloorBrightShader_iBrightLimit = glGetUniformLocation(bloorBrightShader->id, "brightLimit");
}

void ShaderMgr::InitHDR()
{
    InitBaseShader(hdrShader);
    hdrShader_iExposure = glGetUniformLocation(hdrShader->id, "exposure");
}

void ShaderMgr::InitFBO()
{
    InitBaseShader(fboShader);
    glBindFragDataLocation(fboShader->id,0,"originColor");
    glBindFragDataLocation(fboShader->id,1,"grayColor");
    glBindFragDataLocation(fboShader->id,2,"luminanceColor");
    glLinkProgram(fboShader->id);
    //PrintBaseShader(fboShader, NULL);
}

void ShaderMgr::InitTbo()
{
    InitBaseShader(tboShader);
    tboShader_iMaxWidth = glGetUniformLocation(tboShader->id, "maxWidth");
    tboShader_iMaxHeight = glGetUniformLocation(tboShader->id, "maxHeight");
}

void ShaderMgr::InitSolid()
{
    //单色
    InitBaseShader(solidShader);
    //若为-1表示未取得index可能变量未使用被优化掉了
    //printf("%d  %d  %d", textureSprite_iMatrix, textureSprite_iColor, textureSprite_iColorMap);
}

void ShaderMgr::InitDiffuse()
{
    //漫反射
    InitBaseShader(diffuseShader);
}

void ShaderMgr::InitTexture2d()
{
    //2d纹理
    InitBaseShader(texture2dShader);
}

void ShaderMgr::InitTextureArray()
{
    //2d纹理数组
    InitBaseShader(texture2dArrayShader);
    texture2dArrayShader_iTime = glGetUniformLocation(texture2dArrayShader->id, "time");
}

void ShaderMgr::InitCubemap()
{
    //cubemap
    InitBaseShader(cubeMapShader);
}

void ShaderMgr::InitTextureSkybox()
{
    //skybox
    InitBaseShader(textureSkybox);
}

void ShaderMgr::InitTextureSprite()
{
    //spritepoint
    InitBaseShader(textureSprite);
    textureSprite_iSize = glGetUniformLocation(textureSprite->id, "size");
}

void ShaderMgr::InitBlur()
{
    //blur
    InitBaseShader(blurShader);
}

void ShaderMgr::OnInit(int type)
{
    for (int i = 0; i < STMax; i++)
    {
        if (type == -1 || (type & (1<<i)) > 0)
        {
            VoidDeldgate fn = initfunctions[(ShaderType)i];
            (this->*fn)();
            printf("init shader %d\n", i);
        }
    }
}

void ShaderMgr::OnUnInit()
{
    glDeleteProgram(solidShader->id);
    glDeleteProgram(diffuseShader->id);
    glDeleteProgram(texture2dShader->id);
    glDeleteProgram(texture2dArrayShader->id);
    glDeleteProgram(cubeMapShader->id);
    glDeleteProgram(textureSkybox->id);
    glDeleteProgram(textureSprite->id);
    glDeleteProgram(blurShader->id);
    glDeleteProgram(tboShader->id);
    glDeleteProgram(fboShader->id);
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

GLuint ShaderMgr::LoadShader(const char* fileVertex, const char* fileFragment)
{
    // create vertex/fragment shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // load file
    if (LoadShaderFile(fileVertex, vertexShader) == false)
    {
        Util::PrintString(3, (char*)"the vertex shader at ", fileVertex, (char*)" not fond.");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    if (LoadShaderFile(fileFragment, fragmentShader) == false)
    {
        Util::PrintString(3, (char*)"the fragment shader at ", fileFragment, (char*)" not fond.");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    // compile
    glCompileShader(fragmentShader);
    glCompileShader(vertexShader);
    GLint result01 = GL_TRUE;
    GLint result02 = GL_TRUE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result01);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result02);
    if (result01 == GL_FALSE)
    {
        char logs[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, logs);
        Util::PrintString(2, (char*)"vertext shader compile faile ", logs);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    if (result02 == GL_FALSE)
    {
        char logs[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, logs);
        Util::PrintString(2, (char*)"fragment shader compile faile ", logs);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    //link
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    //delete vertex/fragment shader
    glDeleteShader(vertexShader);
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
        glUniformMatrix3fv(shader->normalMatrix, 1, GL_TRUE, param.normalMatrix);
        glUniform3fv(shader->lightDirection, 1, param.lightDirection);
        glUniform4fv(shader->cameraPosition, 1, param.cameraPosition);
        glUniform4fv(shader->diffuseColor, 1, param.diffuseColor);
        glUniform4fv(shader->environmentColor, 1, param.environmentColor);
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
    InitBaseShaderParam(solidShader, param);
}

void ShaderMgr::UseDiffuse(const BaseShaderParam& param)
{
    //调用useprogram之后设置参数
    InitBaseShaderParam(diffuseShader, param);
}

void ShaderMgr::UseTexture2d(const BaseShaderParam& param)
{
    InitBaseShaderParam(texture2dShader, param);
}

void ShaderMgr::UseTextureArray(const BaseShaderParam& param, GLuint time)
{
    InitBaseShaderParam(texture2dArrayShader, param);
    glUniform1f(texture2dArrayShader_iTime, time);
}

void ShaderMgr::UseCubeMap(const BaseShaderParam& param)
{
    InitBaseShaderParam(cubeMapShader, param);
}

void ShaderMgr::UseSkyBox(const BaseShaderParam& param)
{
    InitBaseShaderParam(textureSkybox, param);
}

void ShaderMgr::UseSpritePoint(const BaseShaderParam& param, GLfloat size)
{
    InitBaseShaderParam(textureSprite, param);
    glUniform1f(textureSprite_iSize, size);
}

void ShaderMgr::UseBlurShader(const BaseShaderParam& param)
{
    InitBaseShaderParam(blurShader, param);
}

void ShaderMgr::UseTboShader(const BaseShaderParam& param, int maxWidth, int maxHeight)
{
    InitBaseShaderParam(tboShader, param);
    glUniform1i(tboShader_iMaxWidth, maxWidth);
    glUniform1i(tboShader_iMaxHeight, maxHeight);
}

void ShaderMgr::DrawToFBO(const BaseShaderParam& param)
{
    InitBaseShaderParam(fboShader, param);
}

void ShaderMgr::UseHDR(const BaseShaderParam& param, float exposure)
{
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

void ShaderMgr::UseBloorBlur(const BaseShaderParam& param)
{
    InitBaseShaderParam(bloorBlurShader, param);
}

void ShaderMgr::UseBloorMix(const BaseShaderParam& param, float exposure)
{
    InitBaseShaderParam(bloorMixShader, param);
    glUniform1f(bloorMixShader_iExposure, exposure);
}
