#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tools.h"

BaseShaderParam param;
GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

GLuint query;
GLuint texture;
GLfloat angle;
GLuint vaos[2];
GLuint vbo_triangle;
GLuint vbo_feedback;
GLuint obj_feedback;

void Display(void)
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelviewStack->PushMatrix();
    modelviewStack->Translate(0, 0, -5);
    modelviewStack->Rotate(angle+=3, 0, 1, 0);
        param.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
        param.SetMVMatrix(normalCamera.GetModelviewMatrix());
        param.SetNormalMatrix(normalCamera.GetNormalMatrix());
        param.SetDiffuseColor(ShaderMgr::white);
        param.colorMap[0] = 0;


        //1.绑定feedback对象obj_feedback,将数据写入到绑定的缓冲区中obj_feedback
        glEnable(GL_RASTERIZER_DISCARD);
        glBindVertexArray(vaos[0]);
        //要在glBeginTransformFeedback前切换shader
        shaderMgr.WriteFeedbackBuffer(param);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, obj_feedback);

        glBeginTransformFeedback(GL_TRIANGLES);
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glEndTransformFeedback();
        glFlush();

        //2.打印feedback buff中数据
        GLfloat feedback[20];
        glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);
        for (int i = 0; i < 20; i++)
            printf("%f ", feedback[i]);
        printf("\n");

        GLint result = 0;
        glGetQueryObjectiv(query, GL_QUERY_RESULT, &result);
        printf("render vertex count: %d\n", result);

        //3.将obj_feedback作为arraybuff，从中获取顶点数据
        glDisable(GL_RASTERIZER_DISCARD);
        glBindVertexArray(vaos[1]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_feedback);

        shaderMgr.UseFeedbackBuffer(param);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    modelviewStack->PopMatrix();

    glutSwapBuffers();
}

const char* verying_names[] = {"outVertex", "outCol", "outTexcoord"};
GLfloat vertices[]={0,0,0,1, 1,1,
                    1,0,0,1, 0,1,
                    0,1,0,1, 1,0,
                    1,1,0,1, 0,0};

void OnStartUp()
{
    //init query
    glGenQueries(1, &query);

    //init camera
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();

    //init texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    Util::LoadJPGTexture(Util::GetFullPath("Chapter19 FeedBackBuffer/pig.jpg"), GL_LINEAR, GL_REPEAT);

    //init triangle
    glGenVertexArrays(2, &vaos[0]);
    glGenBuffers(1, &vbo_triangle);
    glBindVertexArray(vaos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4*sizeof(float)));
    glEnableVertexAttribArray(3);

    //init feedback buffer object
    GLuint program = shaderMgr.GetShaderId(STWriteFeedback);
    glTransformFeedbackVaryings(program, 3, verying_names, GL_INTERLEAVED_ATTRIBS);//
    glLinkProgram(program);
    glUseProgram(program);

    glBindVertexArray(vaos[1]);
    glGenBuffers(1, &vbo_feedback);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_feedback);
    glBufferData(GL_ARRAY_BUFFER, 60*sizeof(float), NULL, GL_DYNAMIC_COPY);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float)*4));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(sizeof(float)*8));
    glEnableVertexAttribArray(2);

    //将vbo_feedback作为feedback的buffer
    glCreateTransformFeedbacks(1, &obj_feedback);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, obj_feedback);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo_feedback);
}

void OnShutUp()
{
    glDeleteVertexArrays(2, vaos);
    glDeleteBuffers(1, &vbo_feedback);
    glDeleteBuffers(1, &vbo_triangle);
    glDeleteTextures(1, &texture);
    glDeleteTransformFeedbacks(1, &obj_feedback);
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
    glutCreateWindow("transform feedback buffer");

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
