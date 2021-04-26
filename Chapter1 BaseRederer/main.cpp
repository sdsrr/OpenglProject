#include "../Common/Header/ShaderMgr.h"
#include "../Common/Header/Tools.h"
#include "../Common/Header/Camera.h"
#include "../Common/Header/UtilPrint.h"

BaseShaderParam param;
ShaderMgr* shaderMgr;
GLMatrixStack modelStack;
GLBatch triangle;

GLfloat vertexs[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

M3DVector4f color = {0,1,0,1};

static void Resize(int width, int height)
{
    glViewport(0, 0, width/2.0, height/2.0);
}

static void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    triangle.Begin(GL_TRIANGLES, 3);
    triangle.CopyVertexData3f(vertexs);
    triangle.End();
    GLShaderManager* standardMgr = shaderMgr->GetStandardShaderMgr();
    standardMgr->UseStockShader(GLT_SHADER_IDENTITY, color);
    triangle.Draw();
    glutSwapBuffers();
}

static void KeyMove(unsigned char key, int x, int y)
{
    float step = 0.1;
    float offsetX = 0;
    float offsetY = 0;
    if (key == 'w')
        offsetY += step;
    if (key == 's')
        offsetY -= step;
    if (key == 'a')
        offsetX -= step;
    if (key == 'd')
        offsetX += step;
    vertexs[0] += offsetX;
    vertexs[3] += offsetX;
    vertexs[6] += offsetX;
    vertexs[1] += offsetY;
    vertexs[4] += offsetY;
    vertexs[7] += offsetY;
    glutPostRedisplay();
}

void OnStartUp()
{
    shaderMgr = ShaderMgr::GetInstance();
}

void OnShutDown()
{
    shaderMgr->OnUnInit();
}

//ʹ�ô�ɫ����һ��������
int main(int argc, char *argv[])
{
    //��ʼ��glut
    glutInit(&argc, argv);
    //���ô��ڴ�С
    glutInitWindowSize(640, 480);
    //���ô��ڳ�ʼλ��
    glutInitWindowPosition(10,10);
    //����ʹ�õĻ�����
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //��������
    glutCreateWindow("base renderer");
    //ע�ắ��
    glutReshapeFunc(Resize);
    glutDisplayFunc(Display);
    glutKeyboardFunc(KeyMove);
    //��ʼ��glew��
    if (glewInit() != GLEW_OK)
    {
        UtilPrint::PrintString(1,  (char*)"Failed to initialize GLEW.\n");
        return -1;
    }
    OnStartUp();
    //��ѭ��
    glutMainLoop();
    OnShutDown();
    return EXIT_SUCCESS;
}
