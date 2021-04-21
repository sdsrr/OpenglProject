#include "../Common/Header/ShaderMgr.h"
#include "../Common/Header/Tools.h"
#include "../Common/Header/Camera.h"
#include "../Common/Header/UtilPrint.h"

BaseShaderParam param;
ShaderMgr* shaderMgr;
GLMatrixStack modelStack;

GLfloat vertexs[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

M3DVector4f color = {0,1,0,0};

static void Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

static void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //����vbo���붥����Ϣ
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    //���Ͷ����������ݽ�����ʽ
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);
    param.SetDiffuseColor(color);
    //param.SetMVPMatrix(modelStack.GetMatrix());
    shaderMgr->UseSolidColor(param);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
}


void OnStartUp()
{
    shaderMgr = ShaderMgr::GetInstance();
}

void OnShutUp()
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
    //��ʼ��glew��
    if (glewInit() != GLEW_OK)
    {
        UtilPrint::PrintString(1,  (char*)"Failed to initialize GLEW.\n");
        return -1;
    }
    OnStartUp();
    //��ѭ��
    glutMainLoop();
    OnShutUp();
    return EXIT_SUCCESS;
}
