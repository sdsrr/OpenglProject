#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"

ShaderMgr shaderMgr;
GLShaderManager glShaderMgr;

GLFrustum frustum;
GLMatrixStack modelviewStack;
GLMatrixStack projectStack;
GLGeometryTransform transformPiple;

GLBatch rectangle;
float color[] = {0.5f,0,0,1};

GLuint vao;
GLuint vbo;

GLuint _vbo;
GLuint ebo;
float vertexs[] = {
    1,0,0, 1,0,0, 0,0,1,//λ�á�����ɫ��������
    2,0,0, 0,1,0, 0,0,1,
    1,1,0, 0,0,1, 0,0,1
};

float vertex_data[] = {
    2,0,0, 1,0,0, //λ��_��ɫ
    3,0,0, 0,1,0,
    3,1,0, 0,0,1,
    2,1,0, 0,1,0,
};
unsigned int vertex_elem[] = {
    0,1,2, 0,2,3
};

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(50, width/height, 0.1f, 100);
    glutPostRedisplay();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);

    shaderMgr.UseDiffuse(color, transformPiple.GetModelViewProjectionMatrix());
    rectangle.Draw();

    glBindVertexArray(vao);
    //ʹ��vao+vbo
    //glDrawArrays(GL_TRIANGLES, 0, 3);

    //ʹ��vao+vbo+ebo
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    Util::executeKeyFn(key, x, y, modelviewStack);
}

static void idle(void)
{
    glutPostRedisplay();
}

void InitVBO()
{
    //����vbo
    glGenBuffers(1, & vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    //���ö���λ��
    //shader��location��Ӧ��ţ��������ͳ��ȣ�������������(������vec3 = 3��float), ����(��һ�鶥�����ݼ�����ȣ��м����3��float��ɫ��3��float����), ��ʼλ��ƫ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //���ö�����ɫ
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);
    //���ö��㷨��
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(sizeof(float)*6));
    glEnableVertexAttribArray(2);
}

void InitEBO()
{
    //ebo+vbo
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    //����λ��
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //������ɫ
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(sizeof(float)*3));
    glEnableVertexAttribArray(1);

    glGenBuffers(1,&ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_elem), vertex_elem, GL_STATIC_DRAW);
}

void onShutup()
{
    shaderMgr.OnUnInit();
}

void onStartup()
{
    shaderMgr.OnInit();
    glShaderMgr.InitializeStockShaders();

    frustum.SetPerspective(50, 640/480, 0.1f, 100);
    transformPiple.SetMatrixStacks(modelviewStack, projectStack);
    projectStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewStack.Translate(0,0,-6);

    //init rectangle
    rectangle.Begin(GL_TRIANGLES, 3, 1);
    rectangle.Color4f(1,0,0,1);
    rectangle.Vertex3f(0,0,0);
    rectangle.Color4f(0,1,0,1);
    rectangle.Vertex3f(1,0,0);
    rectangle.Color4f(0,0,1,1);
    rectangle.Vertex3f(0,1,0);
    rectangle.End();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //InitVBO();
    InitEBO();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("vbo");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init fail...\n");
        return 0;
    }

    onStartup();
    glutMainLoop();
    onShutup();

    return EXIT_SUCCESS;
}