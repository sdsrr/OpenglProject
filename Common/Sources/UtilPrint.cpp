#include "../Header/UtilPrint.h"

void UtilPrint::PrintBuffer(GLenum type, int bufferSize)
{
    GLfloat* buffer = (GLfloat*)malloc(sizeof(float)*bufferSize);
    glGetBufferSubData(type, 0, sizeof(float)*bufferSize, buffer);
    for (int i = 0; i < bufferSize; i++)
        printf("%.0f ", buffer[i]);
    printf("\n");
    free(buffer);
}

void UtilPrint::PrintString(int count, char* str, ...)
{
    char **address = &str;
    for (int i = 0; i < count; i++)
    {
        printf(*address);
        address = ++address;
    }
    printf("\n");
}

void UtilPrint::PrintMatrix44f(M3DMatrix44f matrix)
{
    /*
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n*4+m]<<" ";
        std::cout<<'\n';
    }
    */
    for (int i=0; i< 16; i++)
        std::cout<<matrix[i]<<" ";
    std::cout<<'\n';
}

void UtilPrint::PrintMatrix44f(const M3DMatrix44f matrix)
{
    /*
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n*4+m]<<" ";
        std::cout<<'\n';
    }
    */
    for (int i=0; i< 16; i++)
        std::cout<<matrix[i]<<" ";
    std::cout<<'\n';
}

void UtilPrint::PrintVector3f(M3DVector3f vector, std::string ext)
{
    std::cout<<ext;
    for (int n = 0; n < 3; n++)
    {
        std::cout<<vector[n]<<" ";
    }
    std::cout<<'\n';
}
