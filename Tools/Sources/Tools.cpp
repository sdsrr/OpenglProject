#include "../Header/Tools.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* Util::projectPath = "E:/GameDev/Opengl/trunk/OpenglProject/";

bool Util::CompareMatrix(const M3DMatrix44f matrixa, const M3DMatrix44f matrixb)
{
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
        {
            if (matrixa[n+4*m] != matrixb[n*4+m])
                return false;
        }
    }
    return true;
}

void Util::RandomVector3(M3DVector3f& vector, int maxX, int maxY, int maxZ)
{
    vector[0] = random(maxX);
    vector[1] = random(maxY);
    vector[2] = random(maxZ);
}

void Util::RandomCol(M3DVector3f& color)
{
    color[0] = random(5000)/5000.0f;
    color[1] = random(5000)/5000.0f;
    color[2] = random(5000)/5000.0f;
}

void Util::LoadJPGTexture(const char* filepath, GLenum filter, GLenum wrapMode)
{
    int width,height,comp;
    unsigned char* pBits = stbi_load(filepath, &width, &height, &comp, STBI_default);
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

    if (comp == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pBits);
    else if (comp == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pBits);
    else
        printf("error comp load %s failed...\n", filepath);
    printf("load %s success width=%d height=%d comp=%d\n", filepath, width, height, comp);
    stbi_image_free(pBits);
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

    if(filter == GL_LINEAR_MIPMAP_LINEAR || filter == GL_LINEAR_MIPMAP_NEAREST || filter == GL_NEAREST_MIPMAP_LINEAR || filter == GL_NEAREST_MIPMAP_NEAREST)
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

void Util::LoadVector3f(M3DVector3f vector, float x, float y, float z)
{
    vector[0] = x;
    vector[1] = y;
    vector[2] = z;
}

void Util::LoadOpenEXR(char*filepath, GLenum filter, GLenum wrapMode, int& texWidth, int& texHeight)
{
    Imf::Array2D<Imf::Rgba> pixels;
    Imf::RgbaInputFile file(filepath, 1);
    Imath::Box2i dw = file.dataWindow();

    texWidth  = dw.max.x - dw.min.x + 1;
    texHeight = dw.max.y - dw.min.y + 1;

    pixels.resizeErase (texHeight, texWidth);

    file.setFrameBuffer (&pixels[0][0] - dw.min.x - dw.min.y * texWidth, 1, texWidth);
    file.readPixels (dw.min.y, dw.max.y);

    GLfloat* texels = (GLfloat*)malloc(texWidth * texHeight * 3 * sizeof(GLfloat));
    GLfloat* pTex = texels;

    // Copy OpenEXR into local buffer for loading into a texture
    for (unsigned int v = 0; v < (unsigned int)texHeight; v++)
    {
        for (unsigned int u = 0; u < (unsigned int)texWidth; u++)
        {
            Imf::Rgba texel = pixels[texHeight - v - 1][u];
            pTex[0] = texel.r;
            pTex[1] = texel.g;
            pTex[2] = texel.b;

            pTex += 3;
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, texWidth, texHeight, 0, GL_RGB, GL_FLOAT, texels);
    free(texels);
}

char* Util::GetFullPath(const char* relativePath)
{
    if (relativePath == NULL)
        return NULL;
    static int index;
    static char fullPath[256][256];
    strcpy(fullPath[index], projectPath);
    strcat(fullPath[index], relativePath);
    //printf("full path %s\n", fullPath[index]);
    return fullPath[index++];
}

void Util::CheckErrors(std::string desc)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
        exit(20);
    }
}

void Util::UpdateFrameRate()
{
	static int iFrames = 0;
	static CStopWatch frameTimer;

    // Reset the stopwatch on first time
    if(iFrames == 0)
    {
        frameTimer.Reset();
        iFrames++;
    }
    // Increment the frame count
    iFrames++;

    // Do periodic frame rate calculation
    if (iFrames == 101)
    {
        float fps = 100.0f / frameTimer.GetElapsedSeconds();
        frameTimer.Reset();
        iFrames = 1;
        printf("%.1f fps\n", fps);
    }
}

bool Util::FileExists(const std::string& abs_filename)
{
    bool ret;
    FILE* fp = fopen(abs_filename.c_str(), "rb");
    if (fp)
    {
        ret = true;
        fclose(fp);
    }
    else
    {
        ret = false;
    }
    return ret;
}

std::string Util::GetBaseDir(const std::string& filepath)
{
    if (filepath.find_last_of("/\\") != std::string::npos)
        return filepath.substr(0, filepath.find_last_of("/\\"));
    return "";
}

float Util::Lerp(float a, float b, float c)
{
    return b * c + (1 - c) * a;
}

void Util::NormalizeVector(vec3 &v)
{
    float len2 = v.v[0] * v.v[0] + v.v[1] * v.v[1] + v.v[2] * v.v[2];
    if (len2 > 0.0f)
    {
        float len = sqrtf(len2);
        v.v[0] /= len;
        v.v[1] /= len;
        v.v[2] /= len;
    }
}

void Util::Roate(vec2& v, float angle)
{
    float cos = std::cos(angle/180*PI);
    float sin = std::sin(angle/180*PI);
    float x = v.v[0]*cos + v.v[1]*sin;
    float y = -v.v[0]*sin + v.v[1]*cos;
    v.v[0] = x;
    v.v[1] = y;
}

void Util::CheckFBO()
{
    GLenum fboStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch (fboStatus)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        printf("fbo work....\n");
        break;
    case GL_FRAMEBUFFER_UNDEFINED:
        printf("窗口不存在\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        printf("绑定点映射错误\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        printf("至少要有一个rbo绑定到fbo\n");
        break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
        printf("rbo数据格式与绑定点不匹配\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        printf("确保每个绑定点的采样数量相同\n");
        break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        printf("确保每个绑定点的层次数量相同\n");
        break;
    default:
        printf("error id %d \n", fboStatus);
        break;
    }
}

void Util::MoveVector(M3DVector3f& result, const M3DVector3f& origin, const M3DVector3f& direction, GLfloat distance)
{
    M3DVector3f tmp;
    m3dCopyVector3(tmp, direction);
    m3dScaleVector3(tmp, distance);
    m3dAddVectors3(result, tmp, origin);
}

void Util::CalcNormal(M3DVector3f& result, const M3DVector3f& origin, const M3DVector3f& p1, const M3DVector3f& p2, const M3DVector3f& p3)
{
    M3DVector3f dir1,dir2,dir3;
    m3dSubtractVectors3(dir1, p1, origin);
    m3dSubtractVectors3(dir2, p2, origin);
    m3dSubtractVectors3(dir3, p3, origin);
    m3dAddVectors3(result,dir1,dir2);
    m3dAddVectors3(result,result,dir3);
    m3dNormalizeVector3(result);
    m3dScaleVector3(result,-1);
}

void Util::MakePlaneBatch(GLBatch& batch, float width, float height)
{
    batch.Begin(GL_TRIANGLES,6,1);
    batch.MultiTexCoord2f(0,0,0);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(0,0,0);

    batch.MultiTexCoord2f(0,1,0);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(width,0,0);

    batch.MultiTexCoord2f(0,0,1);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(0,0,height);

    batch.MultiTexCoord2f(0,0,1);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(0,0,height);

    batch.MultiTexCoord2f(0,1,0);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(width,0,0);

    batch.MultiTexCoord2f(0,1,1);
    batch.Normal3f(0,1,0);
    batch.Vertex3f(width,0,height);
    batch.End();
}
