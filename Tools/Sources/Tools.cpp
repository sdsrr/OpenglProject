#include "../Header/Tools.h"

const char* Util::projectPath = "E:/GameDev/Opengl/trunk/OpenglProject/";

void Util::printstring(int count, char* str, ...)
{
    char **address = &str;
    for (int i = 0; i < count; i++)
    {
        printf(*address);
        address = ++address;
    }
    printf("\n");
}

void Util::printmatrix44f(M3DMatrix44f matrix)
{
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n+4*m]<<" ";
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void Util::printmatrix44f(const M3DMatrix44f matrix)
{
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n+4*m]<<" ";
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void Util::LoadTGATexture(const char* filepath, GLenum filter, GLenum wrapMode)
{
    GLenum eFormat;
    GLint width, height, components;
    GLbyte* pBits = gltReadTGABits(Util::GetFullPath(filepath), &width, &height, &components, &eFormat);
    if (pBits == NULL)
    {
        printf("load %s failed...\n", filepath);
        return;
    }
    //环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    //过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
    //printf("path:%s width:%d  height:%d", filepath, width, height);
    free(pBits);

    if(filter == GL_LINEAR_MIPMAP_LINEAR ||
       filter == GL_LINEAR_MIPMAP_NEAREST ||
       filter == GL_NEAREST_MIPMAP_LINEAR ||
       filter == GL_NEAREST_MIPMAP_NEAREST)
        glGenerateMipmap(GL_TEXTURE_2D);
}

void Util::LoadTGATextureArray(const char* filepath[], GLint count, GLenum filter, GLenum wrapMode)
{
    //环绕模式
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, wrapMode);
    //过滤方式
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, filter);

    //glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 64, 64, count, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

    for (int i = 0 ; i < count; i++)
    {
        GLenum eFormat;
        GLint width, height, components;
        GLbyte* pBits = gltReadTGABits(Util::GetFullPath(filepath[i]), &width, &height, &components, &eFormat);
        if (pBits == NULL)
        {
            printf("load %s failed...\n", filepath[i]);
            return;
        }
        //printf("path:%s width:%d  height:%d format:%d \n", filepath[i], width, height, eFormat);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, eFormat, GL_UNSIGNED_BYTE, pBits);

        free(pBits);
    }
}

void Util::LoadTGACubemap(const char* filepath[], GLenum magFilter, GLenum minFilter, GLenum wrapMode)
{
    GLenum cube[] =
    {
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    };

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);

    for(int i = 0; i < 6; i++)
    {
        GLenum eFormat;
        GLint width, height, component;
        GLbyte* pBits = gltReadTGABits(Util::GetFullPath(filepath[i]), &width, &height, &component, &eFormat);
        if (pBits == NULL)
        {
            printf("load %s failed...\n", filepath[i]);
            return;
        }
        glTexImage2D(cube[i], 0, component, width, height, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
        free(pBits);
    }
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

char* Util::GetFullPath(const char* relativePath)
{
    static int index;
    static char fullPath[256][256];
    strcpy(fullPath[index], projectPath);
    strcat(fullPath[index], relativePath);
    //printf("full path %s\n", fullPath[index]);
    return fullPath[index++];
}
