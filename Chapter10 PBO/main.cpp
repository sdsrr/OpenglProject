#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"
#include "../Tools/Header/UtilTimer.h"
#include "../Tools/Header/GameObject.h"
#include "../Tools/Header/Camera.h"

UtilTimer timer;
GLfloat angle = 0;
BatchGObject triangle;
BatchGObject screenQuad;
TriangleGObject torus;

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr* shaderMgr;
NormalCamera normalCamera;

GLuint textures[7];
GLuint pixelBufferObj;
void* pixelBuffer;

M3DVector4f whiteCol = {1,1,1,1};
M3DVector4f redCol = {1,0,0,1};
GLboolean openPbo = false;

unsigned int GetCurTexture()
{
    static unsigned int index = 0;
    if (index >= 6)
        index = 0;
    return index++;
}

static void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0.4f, 1);

    //绘制torus使用texture0纹理单元，textureid之前绑定过，不改变就不用再设置
    glActiveTexture(GL_TEXTURE0);
    param.SetDiffuseColor(whiteCol);
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(torus.modelStack));
    param.colorMap[0] = 0;
    shaderMgr->UseTexture2d(param);
    torus.Draw();
    timer.start();

    if (openPbo)
    {
        ///读取帧缓存数据到pbo
        glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferObj);
        //截取的时候还未绘制triangle，所以截取内容没有triangle，就不会显示到triangle上
        glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        ///将pbo中数据设置到对应texture
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferObj);
        glActiveTexture(GL_TEXTURE1 + GetCurTexture());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        ///绘制triangle
        param.colorMap[0] = 1;
        param.colorMap[1] = 2;
        param.colorMap[2] = 3;
        param.colorMap[3] = 4;
        param.colorMap[4] = 5;
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(triangle.modelStack));
        shaderMgr->UseBlurShader(param);
        triangle.Draw();
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    }
    else
    {
        ///读取帧缓存数据到cpu buffer
        glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);

        ///将cpu buffer中数据设置到对应texture
        glActiveTexture(GL_TEXTURE1 + GetCurTexture());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);

        ///绘制triangle
        param.colorMap[0] = 1;
        param.colorMap[1] = 2;
        param.colorMap[2] = 3;
        param.colorMap[3] = 4;
        param.colorMap[4] = 5;
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix(triangle.modelStack));
        shaderMgr->UseBlurShader(param);
        triangle.Draw();
    }

    timer.end();
    //printf("use millisecond time %d \n", timer.msec()) ;
    Util::UpdateFrameRate();
    glutSwapBuffers();
}

static void Idle(void)
{
    glutPostRedisplay();
}

void OnStartUp()
{
    //init triangle
    triangle.batch.Begin(GL_TRIANGLES, 6, 6);
    triangle.batch.MultiTexCoord2f(0, 0, 0);
    triangle.batch.Vertex3f(0, 0, 0);
    triangle.batch.MultiTexCoord2f(0, 1, 0);
    triangle.batch.Vertex3f(1, 0, 0);
    triangle.batch.MultiTexCoord2f(0, 0, 1);
    triangle.batch.Vertex3f(0, 1, 0);

    triangle.batch.MultiTexCoord2f(0, 1, 0);
    triangle.batch.Vertex3f(1, 0, 0);
    triangle.batch.MultiTexCoord2f(0, 1, 1);
    triangle.batch.Vertex3f(1, 1, 0);
    triangle.batch.MultiTexCoord2f(0, 0, 1);
    triangle.batch.Vertex3f(0, 1, 0);
    triangle.batch.End();
    triangle.modelStack.Translate(0,0,-2);

    gltMakeTorus(torus.batch, 4, 1, 40, 40);
    torus.modelStack.Translate(0,0,-15);
    torus.modelStack.Rotate(angle+=2,0,1,0);

    glShaderMgr.InitializeStockShaders();
    shaderMgr = ShaderMgr::GetInstance();
    normalCamera.OnInit(640, 480, 50, 1, 2);

    //一个color有rgb3个byte
    int bufferSize = 640 * 480 * sizeof(unsigned int) * 3;
    pixelBuffer = malloc(bufferSize);
    memset(pixelBuffer, 0, bufferSize);

    //init textures
    glGenTextures(7, textures);
    //torus texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    Util::LoadTGATexture("Chapter10 PBO/Marble.tga", GL_LINEAR, GL_REPEAT);
    //blur texture
    for (int i = 0 ; i < 6; i++)
    {
        glActiveTexture(GL_TEXTURE1 + i);
        glBindTexture(GL_TEXTURE_2D, textures[1 + i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);
    }

    // init pixel buffer
    glGenBuffers(1, &pixelBufferObj);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferObj);
    glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, pixelBuffer, GL_DYNAMIC_COPY);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    gltCheckErrors();
}

void OnShutUp()
{
    shaderMgr->OnUnInit();
    normalCamera.OnUnInit();
    for (int i = 0 ; i < 7; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glDeleteTextures(7, textures);
    glDeleteBuffers(1, &pixelBufferObj);
    free(pixelBuffer);
}

void KeyboardFn(unsigned char key, int x, int y){normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y){normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y){normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h){normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("pbo");

    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK)
    {
        printf("init glew error...\n");
        return 0;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();
    return EXIT_SUCCESS;
}
