#include "../Header/ShaderMgr.h"

void ShaderMgr::OnInit()
{
    //单色
    solidShader = LoadShader(Util::GetFullPath("Tools/Shader/SolidColor/vertex.vp"), Util::GetFullPath("Tools/Shader/SolidColor/fragment.fp"));

    //漫反射
    diffuseShader = LoadShader(Util::GetFullPath("Tools/Shader/Diffuse/vertex.vp"), Util::GetFullPath("Tools/Shader/Diffuse/fragment.fp"));
    diffuseShader_iMatrix = glGetUniformLocation(diffuseShader, "mvpMatrix");
    diffuseShader_iColor = glGetUniformLocation(diffuseShader, "color");

    //2d纹理
    texture2dShader = LoadShader(Util::GetFullPath("Tools/Shader/Texture2D/vertex.vp"), Util::GetFullPath("Tools/Shader/Texture2D/fragment.fp"));
    texture2dShader_iMatrix = glGetUniformLocation(texture2dShader, "mvpMatrix");
    texture2dShader_iColor = glGetUniformLocation(texture2dShader, "color");
    texture2dShader_iColorMap = glGetUniformLocation(texture2dShader, "colorMap");

    //2d纹理数组
    texture2dArrayShader = LoadShader(Util::GetFullPath("Tools/Shader/TextureArray/vertex.vp"), Util::GetFullPath("Tools/Shader/TextureArray/fragment.fp"));
    texture2dArrayShader_iMatrix = glGetUniformLocation(texture2dArrayShader, "mvpMatrix");
    texture2dArrayShader_iColor = glGetUniformLocation(texture2dArrayShader, "color");
    texture2dArrayShader_iColorMap = glGetUniformLocation(texture2dArrayShader, "colorMap");
    texture2dArrayShader_iTime = glGetUniformLocation(texture2dArrayShader, "time");

    //cubemap
    textureCubeMapShader = LoadShader(Util::GetFullPath("Tools/Shader/Cubemap/vertex.vp"), Util::GetFullPath("Tools/Shader/Cubemap/fragment.fp"));
    textureCubeMapShader_iMatrix = glGetUniformLocation(textureCubeMapShader, "mvpMatrix");
    textureCubeMapShader_iColor = glGetUniformLocation(textureCubeMapShader, "color");
    textureCubeMapShader_iCubeMap = glGetUniformLocation(textureCubeMapShader, "cubeMap");

    //若为-1表示未取得index可能变量未使用被优化掉了
    //printf("%d  %d  %d", textureCubeMapShader_iMatrix, textureCubeMapShader_iColor, textureCubeMapShader_iCubeMap);
}

void ShaderMgr::OnUnInit()
{
    glDeleteProgram(solidShader);
    glDeleteProgram(diffuseShader);
    glDeleteProgram(texture2dShader);
    glDeleteProgram(texture2dArrayShader);
    glDeleteProgram(textureCubeMapShader);
}

bool ShaderMgr::LoadShaderFile(const char* filePath, GLuint shader)
{
    FILE* file = fopen(filePath, "r");
    if (file == NULL)
        return false;

    int index = 0;
    char buffer[4096]={' '};
    while(!feof(file))
    {
        char tmpBuf[64]={' '};
        fgets(tmpBuf, 64, file);
        strcpy(&buffer[index], tmpBuf);
        index += strlen(tmpBuf);
    }

    //需要大小与内容完全合适的buf
    char *source = (char*)malloc(strlen(buffer));
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
        Util::printstring(3, (char*)"the vertex shader at ", fileVertex, (char*)" not fond.");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    if (LoadShaderFile(fileFragment, fragmentShader) == false)
    {
        Util::printstring(3, (char*)"the fragment shader at ", fileFragment, (char*)" not fond.");
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
        Util::printstring(2, (char*)"vertext shader compile faile ", logs);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }
    if (result02 == GL_FALSE)
    {
        char logs[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, logs);
        Util::printstring(2, (char*)"fragment shader compile faile ", logs);
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

void ShaderMgr::UseTexture2d(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture)
{
    glUseProgram(texture2dShader);
    glUniformMatrix4fv(texture2dShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(texture2dShader_iColor, 1, color);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(texture2dShader_iColorMap, 0);
}

void ShaderMgr::UseTextureArray(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint texture, GLuint time)
{
    glUseProgram(texture2dArrayShader);
    glUniformMatrix4fv(texture2dArrayShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(texture2dArrayShader_iColor,1,color);
    glUniform1f(texture2dArrayShader_iTime, time);

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glUniform1i(texture2dShader_iColorMap, 0);
}

void ShaderMgr::UseCubeMap(M3DVector4f color, const M3DMatrix44f mvpMatrix, GLuint cubeMap)
{
    glUseProgram(textureCubeMapShader);
    glUniformMatrix4fv(textureCubeMapShader_iMatrix, 1, GL_TRUE, mvpMatrix);
    glUniform4fv(textureCubeMapShader_iColor, 1, color);

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
    glUniform1i(textureCubeMapShader_iCubeMap, 0);
}
