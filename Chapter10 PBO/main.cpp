#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

GLBatch   triangle;

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLuint textures[6];
GLuint pixelBufferObj;
void* pixelBuffer;

GLfloat redCol[] = {1,0,0,1};

unsigned int index = 0;
unsigned int GetTexture()
{
    if (index >= 6)
        index = 0;
    return index++;
}

static void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);

    //读取帧缓存数据到pbo
    //glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferObj);
    glReadPixels(0, 0, 640, 480, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);
    //glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    //将pbo数据设置到对应texture
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferObj);
    //glActiveTexture(GL_TEXTURE0 + GetTexture());
    //glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelBuffer);
    //glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    //glShaderMgr.UseStockShader(GLT_SHADER_FLAT, normalCamera.GetModelviewprojectMatrix(), redCol);
    shaderMgr.UseBlurShader(normalCamera.GetModelviewprojectMatrix());
    triangle.Draw();

    glutSwapBuffers();
}


static void Idle(void)
{
    glutPostRedisplay();
}

void OnStartUp()
{
    //init triangle
    triangle.Begin(GL_TRIANGLES, 6, 1);
    triangle.MultiTexCoord2f(0, 0, 0);
    triangle.Vertex3f(0, 0, 0);
    triangle.MultiTexCoord2f(0, 1, 0);
    triangle.Vertex3f(1, 0, 0);
    triangle.MultiTexCoord2f(0, 0, 1);
    triangle.Vertex3f(0, 1, 0);

    triangle.MultiTexCoord2f(0, 1, 0);
    triangle.Vertex3f(1, 0, 0);
    triangle.MultiTexCoord2f(0, 1, 1);
    triangle.Vertex3f(1, 1, 0);
    triangle.MultiTexCoord2f(0, 0, 1);
    triangle.Vertex3f(0, 1, 0);
    triangle.End();

    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit((int)STBlur);
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
    modelviewStack->Translate(0,0,-2);

    //一个color有rgb3个byte
    int bufferSize = 640 * 480 * sizeof(unsigned int) * 3;
    void* data = malloc(bufferSize);
    pixelBuffer = malloc(bufferSize);
    memset(data, 0, bufferSize);
    memset(pixelBuffer, 0, bufferSize);

    // init textures
    glGenTextures(6, textures);
    for (int i = 0 ; i < 6; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    // init pixel buffer
    glCreateBuffers(1, &pixelBufferObj);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pixelBufferObj);
    glBufferData(GL_PIXEL_PACK_BUFFER, bufferSize, pixelBuffer, GL_DYNAMIC_COPY);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    gltCheckErrors();
}

void OnShutUp()
{
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
    for (int i = 0 ; i < 6; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glDeleteTextures(6, textures);
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
