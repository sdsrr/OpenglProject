#include "../Header/ShaderMgr.h"

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
}

void ShaderMgr::InitTbo()
{
    tboShader = LoadShader(Util::GetFullPath("Tools/Shader/TBO/vertex.vp"), Util::GetFullPath("Tools/Shader/TBO/fragment.fp"));
    tboShader_iMatrix = glGetUniformLocation(tboShader, "mvpMatrix");
    tboShader_iColorMap = glGetUniformLocation(tboShader, "colorMap");
    tboShader_iMaxWidth = glGetUniformLocation(tboShader, "maxWidth");
    tboShader_iMaxHeight = glGetUniformLocation(tboShader, "maxHeight");
}

void ShaderMgr::InitSolid()
{
    //单色
    solidShader = LoadShader(Util::GetFullPath("Tools/Shader/SolidColor/vertex.vp"), Util::GetFullPath("Tools/Shader/SolidColor/fragment.fp"));
    //若为-1表示未取得index可能变量未使用被优化掉了
    //printf("%d  %d  %d", textureSprite_iMatrix, textureSprite_iColor, textureSprite_iColorMap);
}

void ShaderMgr::InitDiffuse()
{
    //漫反射
    diffuseShader = LoadShader(Util::GetFullPath("Tools/Shader/Diffuse/vertex.vp"), Util::GetFullPath("Tools/Shader/Diffuse/fragment.fp"));
    diffuseShader_iMatrix = glGetUniformLocation(diffuseShader, "mvpMatrix");
    diffuseShader_iColor = glGetUniformLocation(diffuseShader, "color");
}

void ShaderMgr::InitTexture2d()
{
    //2d纹理
    texture2dShader = LoadShader(Util::GetFullPath("Tools/Shader/Texture2D/vertex.vp"), Util::GetFullPath("Tools/Shader/Texture2D/fragment.fp"));
    texture2dShader_iMatrix = glGetUniformLocation(texture2dShader, "mvpMatrix");
    texture2dShader_iColor = glGetUniformLocation(texture2dShader, "color");
    texture2dShader_iColorMap = glGetUniformLocation(texture2dShader, "colorMap");
}

void ShaderMgr::InitTextureArray()
{
    //2d纹理数组
    texture2dArrayShader = LoadShader(Util::GetFullPath("Tools/Shader/TextureArray/vertex.vp"), Util::GetFullPath("Tools/Shader/TextureArray/fragment.fp"));
    texture2dArrayShader_iMatrix = glGetUniformLocation(texture2dArrayShader, "mvpMatrix");
    texture2dArrayShader_iColor = glGetUniformLocation(texture2dArrayShader, "color");
    texture2dArrayShader_iColorMap = glGetUniformLocation(texture2dArrayShader, "colorMap");
    texture2dArrayShader_iTime = glGetUniformLocation(texture2dArrayShader, "time");
}

void ShaderMgr::InitCubemap()
{
    //cubemap
    textureCubeMapShader = LoadShader(Util::GetFullPath("Tools/Shader/Cubemap/vertex.vp"), Util::GetFullPath("Tools/Shader/Cubemap/fragment.fp"));
    textureCubeMapShader_iMatrix = glGetUniformLocation(textureCubeMapShader, "mvpMatrix");
    textureCubeMapShader_iColor = glGetUniformLocation(textureCubeMapShader, "color");
    textureCubeMapShader_iCubeMap = glGetUniformLocation(textureCubeMapShader, "cubeMap");
}

void ShaderMgr::InitTextureSkybox()
{
    //skybox
    textureSkybox = LoadShader(Util::GetFullPath("Tools/Shader/SkyBox/vertex.vp"), Util::GetFullPath("Tools/Shader/SkyBox/fragment.fp"));
    textureSkybox_iCubeMap = glGetUniformLocation(textureSkybox, "colorMap");
    textureSkybox_iMatrix = glGetUniformLocation(textureSkybox, "mvpMatrix");
}

void ShaderMgr::InitTextureSprite()
{
    //spritepoint
    textureSprite = LoadShader(Util::GetFullPath("Tools/Shader/SpritePoint/vertex.vp"), Util::GetFullPath("Tools/Shader/SpritePoint/fragment.fp"));
    textureSprite_iMatrix = glGetUniformLocation(textureSprite, "mvpMatrix");
    textureSprite_iColorMap = glGetUniformLocation(textureSprite, "colorMap");
    textureSprite_iSize = glGetUniformLocation(textureSprite, "size");
}

void ShaderMgr::InitBlur()
{
    //blur
    blurShader = LoadShader(Util::GetFullPath("Tools/Shader/Blur/vertex.vp"), Util::GetFullPath("Tools/Shader/Blur/fragment.fp"));
    blur_iMatrix = glGetUniformLocation(blurShader, "mvpMatrix");
    blurTexture[0] = glGetUniformLocation(blurShader, "texture00");
    blurTexture[1] = glGetUniformLocation(blurShader, "texture01");
    blurTexture[2] = glGetUniformLocation(blurShader, "texture02");
    blurTexture[3] = glGetUniformLocation(blurShader, "texture03");
    blurTexture[4] = glGetUniformLocation(blurShader, "texture04");
    blurTexture[5] = glGetUniformLocation(blurShader, "texture05");
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
    glDeleteProgram(solidShader);
    glDeleteProgram(diffuseShader);
    glDeleteProgram(texture2dShader);
    glDeleteProgram(texture2dArrayShader);
    glDeleteProgram(textureCubeMapShader);
    glDeleteProgram(textureSkybox);
    glDeleteProgram(textureSprite);
    glDeleteProgram(blurShader);
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

void ShaderMgr::UseSolidColor(M3DVector4f color)
{
    glUseProgram(solidShader);
}

void ShaderMgr::UseDiffuse(M3DVector4f color, const M3DMatrix44f mvpMatrix)
{
    //调用useprogram之后设置参数
    glUseProgram(diffuseShader);
    glUniformMatrix4fv(diffuseShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(diffuseShader_iColor, 1, color);
}

void ShaderMgr::UseDiffuse(M3DVector4f color, M3DMatrix44f mvpMatrix)
{
    //调用useprogram之后设置参数
    glUseProgram(diffuseShader);
    glUniformMatrix4fv(diffuseShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(diffuseShader_iColor, 1, color);
}

void ShaderMgr::UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit)
{
    glUseProgram(texture2dShader);
    glUniformMatrix4fv(texture2dShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(texture2dShader_iColor, 1, color);
    glUniform1i(texture2dShader_iColorMap, textureUnit);
}

void ShaderMgr::UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit, GLuint time)
{
    glUseProgram(texture2dArrayShader);
    glUniformMatrix4fv(texture2dArrayShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(texture2dArrayShader_iColor,1,color);
    glUniform1f(texture2dArrayShader_iTime, time);
    glUniform1i(texture2dShader_iColorMap, textureUnit);
}

void ShaderMgr::UseCubeMap(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint textureUnit)
{
    glUseProgram(textureCubeMapShader);
    glUniformMatrix4fv(textureCubeMapShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(textureCubeMapShader_iColor, 1, color);
    glUniform1i(textureCubeMapShader_iCubeMap, textureUnit);
}

void ShaderMgr::UseSkyBox(const M3DMatrix44f mvpMatrix, GLuint textureUnit)
{
    glUseProgram(textureSkybox);
    glUniform1i(textureSkybox_iCubeMap, textureUnit);
    glUniformMatrix4fv(textureSkybox_iMatrix, 1, GL_TRUE, mvpMatrix);
}

void ShaderMgr::UseSpritePoint(const M3DMatrix44f mvpMatrix, GLuint textureUnit, GLfloat size)
{
    glUseProgram(textureSprite);
    glUniform1i(textureSprite_iColorMap, textureUnit);
    glUniform1f(textureSprite_iSize, size);
    glUniformMatrix4fv(textureSprite_iMatrix, 1, GL_TRUE, mvpMatrix);
}

void ShaderMgr::UseBlurShader(const M3DMatrix44f mvpMatrix, int textureUnit)
{
    glUseProgram(blurShader);
    for (int i = 0 ; i < 6; i++)
        glUniform1i(blurTexture[i], textureUnit + i);
    glUniformMatrix4fv(blur_iMatrix, 1, GL_TRUE, mvpMatrix);
}

void ShaderMgr::UseTboShader(const M3DMatrix44f mvpMatrix, int maxWidth, int maxHeight, int textureUnit)
{
    glUseProgram(tboShader);
    glUniformMatrix4fv(tboShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform1i(tboShader_iColorMap, textureUnit);
    glUniform1i(tboShader_iMaxWidth, maxWidth);
    glUniform1i(tboShader_iMaxHeight, maxHeight);
}
