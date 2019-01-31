#include "../Header/Tools.h"


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
    GLbyte* pBits = gltReadTGABits(filepath, &width, &height, &components, &eFormat);
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
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, 512, 512, count, 0, GL_BGR, GL_UNSIGNED_BYTE, NULL);

    for (int i = 0 ; i < count; i++)
    {
        GLenum eFormat;
        GLint width, height, components;
        GLbyte* pBits = gltReadTGABits(filepath[i], &width, &height, &components, &eFormat);
        if (pBits == NULL)
        {
            printf("load %s failed...\n", filepath[i]);
            return;
        }
        //printf("path:%s width:%d  height:%d format:%d", filepath[i], width, height, eFormat);
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_BGR, GL_UNSIGNED_BYTE, pBits);

        free(pBits);
    }
}
