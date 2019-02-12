#include "../Tools/Header/ShaderMgr.h"

GLShaderManager glShaderMgr;
ShaderMgr shaderMgr;
GLBatch cubeBatch;
GLTriangleBatch sphereBatch;

GLFrame camera;
GLFrustum frustum;
GLMatrixStack modelviewMatrixStack;
GLMatrixStack projectMatrixStack;
GLGeometryTransform tranformPipeline;

const float color[]={1,0,0,0};

GLuint textureId;
const char* cubemap[] =
{
    "Tools/Texture/Cubemap/neg_x.tga",
    "Tools/Texture/Cubemap/pos_x.tga",
    "Tools/Texture/Cubemap/neg_y.tga",
    "Tools/Texture/Cubemap/pos_y.tga",
    "Tools/Texture/Cubemap/neg_z.tga",
    "Tools/Texture/Cubemap/pos_z.tga",
};

static void resize(int width, int height)
{
    glViewport(0, 0, width, height);
    frustum.SetPerspective(50, width/height, 0.1f, 100);
    glutPostRedisplay();
}

static void display(void)
{
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glShaderMgr.UseStockShader(GLT_SHADER_FLAT, tranformPipeline.GetModelViewProjectionMatrix(), color);

    M3DMatrix44f mvpMatrix;
    M3DMatrix44f cameraRoateMatrix;
    camera.GetCameraMatrix(cameraRoateMatrix, true);
    camera.GetCameraMatrix(mvpMatrix);

    /*
        先绘制其他物体，再绘制skybox，并在skybox时设置box的深度为1
        这种方式相比先绘制skybox再绘制其他物体，减少了部分skybox被绘制后又被其他物体遮挡的消耗
    */

    //draw sphere
    modelviewMatrixStack.PushMatrix();
        modelviewMatrixStack.MultMatrix(mvpMatrix);
        glShaderMgr.UseStockShader(GLT_SHADER_FLAT,tranformPipeline.GetModelViewProjectionMatrix(), color);
        sphereBatch.Draw();
    modelviewMatrixStack.PopMatrix();

    // draw skybox only use camera roate matrix
    modelviewMatrixStack.PushMatrix();
        modelviewMatrixStack.MultMatrix(cameraRoateMatrix);
        shaderMgr.UseSkyBox(tranformPipeline.GetModelViewProjectionMatrix(), textureId);
        glDisable(GL_CULL_FACE);
        cubeBatch.Draw();
        glEnable(GL_CULL_FACE);
    modelviewMatrixStack.PopMatrix();

    glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void key(unsigned char key, int x, int y)
{
    float speed = 0.5f;
    switch(key)
    {
    case 'a':
        camera.MoveRight(speed);
        break;
    case 'd':
        camera.MoveRight(-speed);
        break;
    case 's':
        camera.MoveUp(-speed);
        break;
    case 'w':
        camera.MoveUp(speed);
        break;
    case 'e':
        camera.MoveForward(-speed);
        break;
    case 'q':
        camera.MoveForward(speed);
        break;

    case 'z':
        camera.RotateLocalY(speed/2);
        break;
    case 'x':
        camera.RotateLocalY(-speed/2);
        break;
    case 'c':
        camera.RotateLocalX(speed/2);
        break;
    case 'v':
        camera.RotateLocalX(-speed/2);
        break;
    }
    glutPostRedisplay();
}

void onStartUp()
{
    glShaderMgr.InitializeStockShaders();
    shaderMgr.OnInit();

    tranformPipeline.SetMatrixStacks(modelviewMatrixStack, projectMatrixStack);
    frustum.SetPerspective(50, 640/480, 0.1f, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewMatrixStack.Translate(0,0,0);

    // init cube
    gltMakeCube(cubeBatch, 1);
    gltMakeSphere(sphereBatch, 0.2f, 30, 30);
    camera.MoveForward(-1);

    //init texture
    glActiveTexture(0);
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    Util::LoadTGACubemap(cubemap, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);

}

void onShutUp()
{
    glDeleteTextures(1, &textureId);
    shaderMgr.OnUnInit();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("sky box");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init failed..\n");
        return 0;
    }

    onStartUp();
    glutMainLoop();
    onShutUp();

    return EXIT_SUCCESS;
}
