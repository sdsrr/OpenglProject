#include "../Tools/Header/ShaderMgr.h"


ShaderMgr shaderMgr;
GLfloat vertexs[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

static void display(void)
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

    shaderMgr.UseSolidColor(0,1,0,0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();
}


void OnStartUp()
{
    shaderMgr.OnInit();
}

void OnShutUp()
{
    shaderMgr.OnUnInit();
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
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    //��ʼ��glew��
    if (glewInit() != GLEW_OK)
    {
        Util::printstring(1,  (char*)"Failed to initialize GLEW.\n");
        return -1;
    }
    OnStartUp();
    //��ѭ��
    glutMainLoop();
    OnShutUp();
    return EXIT_SUCCESS;
}