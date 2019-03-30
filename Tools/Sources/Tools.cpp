#include "../Header/Tools.h"

const char* Util::projectPath = "E:/GameDev/Opengl/trunk/OpenglProject/";

void Util::PrintString(int count, char* str, ...)
{
    char **address = &str;
    for (int i = 0; i < count; i++)
    {
        printf(*address);
        address = ++address;
    }
    printf("\n");
}

void Util::PrintMatrix44f(M3DMatrix44f matrix)
{
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n+4*m]<<" ";
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

void Util::PrintMatrix44f(const M3DMatrix44f matrix)
{
    for (int n = 0; n < 4; n++)
    {
        for (int m = 0; m < 4; m++)
            std::cout<<matrix[n+4*m]<<" ";
        std::cout<<'\n';
    }
    std::cout<<'\n';
}

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
    /*
    if(filter == GL_LINEAR_MIPMAP_LINEAR ||
       filter == GL_LINEAR_MIPMAP_NEAREST ||
       filter == GL_NEAREST_MIPMAP_LINEAR ||
       filter == GL_NEAREST_MIPMAP_NEAREST)
    */
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

void Util::CheckErrors(std::string desc)
{
    GLenum e = glGetError();
    if (e != GL_NO_ERROR)
    {
        fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
        exit(20);
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



void NormalCamera::OnInit(float w, float h, float fov, float moveSp, float roateSp)
{
    this->fov = fov;
    this->height = h;
    this->width = w;
    this->moveSpeed = moveSp;
    this->roateSpeed = roateSp;

    frustum.SetPerspective(fov, width/height, 0.1f, 1000);
    modelviewStack.LoadIdentity();
    projectStack.LoadIdentity();
    transformPiple.SetMatrixStacks(modelviewStack, projectStack);
}

void NormalCamera::MouseClick(int button, int action, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion=MMLeftMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion = MMRightMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    if (button == GLUT_MIDDLE_BUTTON)
    {
        if (action == GLUT_DOWN) {
            mouseMotion = MMMedMousePress;
        }
        else if (action == GLUT_UP) {
            mouseMotion = MMNone;
        }
    }
    prevMouseX = x;
    prevMouseY = y;
}

void NormalCamera::KeyboardFn(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        projectStack.Translate(moveSpeed,0,0);
        break;
    case 'd':
        projectStack.Translate(-moveSpeed,0,0);
        break;
    case 'w':
        projectStack.Translate(0,-moveSpeed,0);
        break;
    case 's':
        projectStack.Translate(0,moveSpeed,0);
        break;
    case 'q':
        projectStack.Translate(0,0,-moveSpeed);
        break;
    case 'e':
        projectStack.Translate(0,0,moveSpeed);
        break;
    }
    glutPostRedisplay();
}

void NormalCamera::MotionFunc(int mouse_x, int mouse_y)
{
    float deltaX = mouse_x - prevMouseX;
    float deltaY = mouse_y - prevMouseY;

    if (mouseMotion == MMRightMousePress)
    {
        projectStack.Rotate(deltaX/roateSpeed, 0, 1, 0);
        projectStack.Rotate(deltaY/roateSpeed, 1, 0, 0);
    }
    //printf("%d  %d\n", mouse_x, mouse_y);
    prevMouseX = mouse_x;
    prevMouseY = mouse_y;
}

void NormalCamera::Resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    frustum.SetPerspective(fov, width/height, 0.1f, 1000);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    glutPostRedisplay();
}

const M3DMatrix44f& NormalCamera::GetModelviewprojectMatrix()
{
    const M3DMatrix44f& matrix = transformPiple.GetModelViewProjectionMatrix();
    return matrix;
}

const M3DMatrix44f& NormalCamera::GetModeviewMatrix()
{
    const M3DMatrix44f& matrix = transformPiple.GetModelViewMatrix();
    return matrix;
}

GLMatrixStack* NormalCamera::GetModelviewStack()
{
    return &modelviewStack;
}

void NormalCamera::OnUnInit()
{

}
