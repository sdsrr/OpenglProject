#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;
BaseShaderParam param;

GLint tickCount = 0;
GLfloat angle;
GLBatch triangle;

GLuint normalFbo;
GLuint normalTexture;
GLuint brightFbo;
GLuint brightTexture;
GLuint blurFbo;
GLuint blurTexture;


void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelviewStack->PushMatrix();
    modelviewStack->Translate(0, 0, -2.5);
    modelviewStack->Rotate(angle, 0, 1, 0);
    param.SetNormalMatrix(normalCamera.GetNormalMatrix());
    param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    param.SetMVMatrix(normalCamera.GetModelviewMatrix());
    param.colorMap[0] = 0;

    //正常渲染到texture[0]
    //对texture[0]曝光获得高亮部分,输出到texture[1]
    //对texture[1]多次模糊输出到texture[2]
    //将三张texture混合输出到屏幕
    shaderMgr.UseBloor(param);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    triangle.Draw();
    modelviewStack->PopMatrix();

    glutSwapBuffers();
}

void OnStartUp()
{
    //init cube
    gltMakeCube(triangle, 1);

    //init hdr texture
    int width,height=0;
    glGenTextures(1, textures);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    Util::LoadOpenEXR(Util::GetFullPath("Chapter15 Bloor/Ocean.exr"), GL_LINEAR, GL_REPEAT, width, height);

    //init camera,shadermgr
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit(1<<STHDR | 1<<STTexture2d | 1<<STBLOOR);
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
}

void OnShutUp()
{
    glDeleteTextures(1, textures);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
}

void Idle(void) {glutPostRedisplay();}
void KeyboardFn(unsigned char key, int x, int y) {normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y) {normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y) {normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h) {normalCamera.Resize(w, h);}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("hdr bloor");


    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init error...\n");
        return 0;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();

    return EXIT_SUCCESS;
}
