#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;

GLBatch leftPatch;
GLBatch rightPatch;
GLBatch upPatch;
GLBatch downPatch;


GLuint texture01;
GLuint texture02;
GLuint texture03;
char* texturePath01 = (char*)"Chapter5 AnisotropicTextureFiltering/Texture/brick.tga";
char* texturePath02 = (char*)"Chapter5 AnisotropicTextureFiltering/Texture/ceiling.tga";
char* texturePath03 = (char*)"Chapter5 AnisotropicTextureFiltering/Texture/floor.tga";

#define gl_linear 1
#define gl_nearest 2
#define gl_linear_mipmap_linear 3
#define gl_linear_mipmap_nearest 4
#define gl_nearest_mipmap_linear 5
#define gl_nearest_mipmap_nearest 6
#define open_anisotropy 7
#define close_anisotropy 8

GLFrustum frustum;
GLMatrixStack modelviewMatrix;
GLMatrixStack projectMatrix;
GLGeometryTransform transformPiple;

float whiteCol[] = {1,1,1,1};
float redCol[] = {1,0,0,1};
static void resize(int width, int height)
{
    frustum.SetPerspective(50, width/height, 0.1f, 100);
    glViewport(0, 0, width, height);
    glutPostRedisplay();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);

    shaderMgr.UseTexture2d(whiteCol, transformPiple.GetModelViewProjectionMatrix(), texture01);
    //glShaderMgr.UseStockShader(GLT_SHADER_FLAT, transformPiple.GetModelViewProjectionMatrix(), redCol);
    leftPatch.Draw();
    rightPatch.Draw();

    shaderMgr.UseTexture2d(whiteCol, transformPiple.GetModelViewProjectionMatrix(), texture02);
    upPatch.Draw();

    shaderMgr.UseTexture2d(whiteCol, transformPiple.GetModelViewProjectionMatrix(), texture03);
    downPatch.Draw();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    float speed = 0.5f;
    switch (key)
    {
    case 'a':
        modelviewMatrix.Translate(speed,0,0);
        break;
    case 'd':
        modelviewMatrix.Translate(-speed,0,0);
        break;
    case 'q':
        modelviewMatrix.Translate(0,0,-speed);
        break;
    case 'e':
        modelviewMatrix.Translate(0,0,speed);
        break;
    case 'z':
        break;
    case 'x':
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

void onStartUp()
{
    frustum.SetPerspective(50, 640/480, 0.1f, 100);
    transformPiple.SetMatrixStacks(modelviewMatrix, projectMatrix);
    projectMatrix.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewMatrix.Translate(0,0,-1);

    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();

    glGenTextures(1,&texture01);
    glBindTexture(GL_TEXTURE_2D, texture01);
    Util::LoadTGATexture(texturePath01, GL_LINEAR, GL_REPEAT);

    glGenTextures(1,&texture02);
    glBindTexture(GL_TEXTURE_2D, texture02);
    Util::LoadTGATexture(texturePath02, GL_LINEAR, GL_REPEAT);

    glGenTextures(1,&texture03);
    glBindTexture(GL_TEXTURE_2D, texture03);
    Util::LoadTGATexture(texturePath03, GL_LINEAR, GL_REPEAT);

    leftPatch.Begin(GL_TRIANGLES, 60, 1);
    for (int z = -10; z < 0; z++)
    {
        leftPatch.MultiTexCoord2f(0,0,0);
        leftPatch.Vertex3f(-1,-1,z);
        leftPatch.MultiTexCoord2f(0,1,0);
        leftPatch.Vertex3f(-1,-1,z-1);
        leftPatch.MultiTexCoord2f(0,0,1);
        leftPatch.Vertex3f(-1,1,z);

        leftPatch.MultiTexCoord2f(0,0,1);
        leftPatch.Vertex3f(-1,1,z);
        leftPatch.MultiTexCoord2f(0,1,0);
        leftPatch.Vertex3f(-1,-1,z-1);
        leftPatch.MultiTexCoord2f(0,1,1);
        leftPatch.Vertex3f(-1, 1,z-1);
    }
    leftPatch.End();

    rightPatch.Begin(GL_TRIANGLES, 60, 1);
    for (int z = -10; z < 0; z++)
    {
        rightPatch.MultiTexCoord2f(0,0,0);
        rightPatch.Vertex3f(1,-1,z);
        rightPatch.MultiTexCoord2f(0,1,0);
        rightPatch.Vertex3f(1,-1,z-1);
        rightPatch.MultiTexCoord2f(0,0,1);
        rightPatch.Vertex3f(1,1,z);

        rightPatch.MultiTexCoord2f(0,0,1);
        rightPatch.Vertex3f(1,1,z);
        rightPatch.MultiTexCoord2f(0,1,0);
        rightPatch.Vertex3f(1,-1,z-1);
        rightPatch.MultiTexCoord2f(0,1,1);
        rightPatch.Vertex3f(1, 1,z-1);
    }
    rightPatch.End();

    upPatch.Begin(GL_TRIANGLES, 60, 1);
    for (int z = -10; z < 0; z++)
    {
        upPatch.MultiTexCoord2f(0,0,0);
        upPatch.Vertex3f(1,1,z);
        upPatch.MultiTexCoord2f(0,1,0);
        upPatch.Vertex3f(1,1,z-1);
        upPatch.MultiTexCoord2f(0,0,1);
        upPatch.Vertex3f(-1,1,z);

        upPatch.MultiTexCoord2f(0,0,0);
        upPatch.Vertex3f(-1,1,z);
        upPatch.MultiTexCoord2f(0,1,0);
        upPatch.Vertex3f(1,1,z-1);
        upPatch.MultiTexCoord2f(0,1,1);
        upPatch.Vertex3f(-1,1,z-1);
    }
    upPatch.End();

    downPatch.Begin(GL_TRIANGLES, 60, 1);
    for (int z = -10; z < 0; z++)
    {
        downPatch.MultiTexCoord2f(0,0,0);
        downPatch.Vertex3f(1,-1,z);
        downPatch.MultiTexCoord2f(0,1,0);
        downPatch.Vertex3f(1,-1,z-1);
        downPatch.MultiTexCoord2f(0,0,1);
        downPatch.Vertex3f(-1,-1,z);

        downPatch.MultiTexCoord2f(0,0,0);
        downPatch.Vertex3f(-1,-1,z);
        downPatch.MultiTexCoord2f(0,1,0);
        downPatch.Vertex3f(1,-1,z-1);
        downPatch.MultiTexCoord2f(0,1,1);
        downPatch.Vertex3f(-1,-1,z-1);
    }
    downPatch.End();
}

void onShutup()
{
    shaderMgr.OnUnInit();
    glDeleteTextures(1, &texture01);
    glDeleteTextures(1, &texture02);
    glDeleteTextures(1, &texture03);
}

void changeTextureFilter(int textureId, int key)
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    switch(key)
    {
    case gl_linear:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        break;
    case gl_nearest:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        break;
    case gl_linear_mipmap_linear:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
        break;
    case gl_linear_mipmap_nearest:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
        break;
    case gl_nearest_mipmap_linear:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
        break;
    case gl_nearest_mipmap_nearest:
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
        break;
    case open_anisotropy:
        {
            float maxnum = 0;
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxnum);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxnum);
            printf("max anisotropy num %f\n", maxnum);
        }
        break;
    case close_anisotropy:
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
        break;
    }
}

void processMenu(int key)
{
    changeTextureFilter(texture01, key);
    changeTextureFilter(texture02, key);
    changeTextureFilter(texture03, key);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("anisotropic texture filtering");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glEnable(GL_DEPTH_TEST);

    //init menu
    glutCreateMenu(processMenu);
    glutAddMenuEntry("linear", gl_linear);
    glutAddMenuEntry("nearest", gl_nearest);
    glutAddMenuEntry("linear_mipmap_linear", gl_linear_mipmap_linear);
    glutAddMenuEntry("linear_mipmap_nearest", gl_linear_mipmap_nearest);
    glutAddMenuEntry("nearest_mipmap_linear", gl_nearest_mipmap_linear);
    glutAddMenuEntry("nearest_mipmap_nearest", gl_nearest_mipmap_nearest);
    glutAddMenuEntry("open anisotropy", open_anisotropy);
    glutAddMenuEntry("close anisotropy", close_anisotropy);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init fail...\n");
        return 0;
    }

    onStartUp();
    glutMainLoop();
    onShutup();

    return EXIT_SUCCESS;
}