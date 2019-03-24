#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

ShaderMgr shaderMgr;
GLfloat vertexs[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

M3DVector4f color = {0,1,0,0};

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //创建vbo传入顶点信息
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    //解释顶点数组数据解析方式
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0);

    shaderMgr.UseSolidColor(color);
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

//使用纯色绘制一个三角形
int main(int argc, char *argv[])
{
    //初始化glut
    glutInit(&argc, argv);
    //设置窗口大小
    glutInitWindowSize(640, 480);
    //设置窗口初始位置
    glutInitWindowPosition(10,10);
    //设置使用的缓冲区
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //创建窗口
    glutCreateWindow("base renderer");
    //注册函数
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    //初始化glew库
    if (glewInit() != GLEW_OK)
    {
        Util::printstring(1,  (char*)"Failed to initialize GLEW.\n");
        return -1;
    }
    OnStartUp();
    //主循环
    glutMainLoop();
    OnShutUp();
    return EXIT_SUCCESS;
}
