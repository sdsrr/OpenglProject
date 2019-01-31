#include "../Tools/Header/ShaderMgr.h"

GLShaderManager shaderMgr_;
ShaderMgr shaderMgr;
GLBatch triangleBathch;

GLuint textureID;
GLuint textureArrayID;
GLuint texturesID[3];
const char* textures[] = {"E:/GameDev/Opengl/trunk/Chapter3 UseTexture/Texture/floor.tga",
                        "E:/GameDev/Opengl/trunk/Chapter3 UseTexture/Texture/brick.tga",
                        "E:/GameDev/Opengl/trunk/Chapter3 UseTexture/Texture/ceiling.tga"};

GLfloat vertexs[]={-0.5f,0,0, 0.5f,0,0, 0,0.5f,0};
GLfloat texcoord[]={ 0.0f,0.0f, 1.0f,0.0f, 0.5f,1.0f };
M3DVector4f color={244.0f/255, 49.0f/255, 166.0f/255 ,0.0f};

GLuint time;
GLFrustum frustum;
GLMatrixStack modelviewMatrixStack;
GLMatrixStack projectMatrixStack;
GLGeometryTransform tranformPipeline;

static void resize(int width, int height)
{
    glViewport(0,0,width,height);
    frustum.SetPerspective(50, width/height,1, 100);
    glutPostRedisplay();
}

static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1,1,1,1);
    //use texture2d
    //shaderMgr.UseTexture2d(color, tranformPipeline.GetModelViewProjectionMatrix(), textureID);

    //use texture2d array
    time = (time+1)%3;
    printf("%d", time);
    shaderMgr.UseTextureArray(color, tranformPipeline.GetModelViewProjectionMatrix(), textureArrayID, time);
    triangleBathch.Draw();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    float speed = 0.5f;
    switch(key)
    {
    case 'a':
        modelviewMatrixStack.Translate(speed, 0, 0);
        break;
    case 'd':
        modelviewMatrixStack.Translate(-speed, 0, 0);
        break;
    case 'w':
        modelviewMatrixStack.Translate(0, -speed, 0);
        break;
    case 's':
        modelviewMatrixStack.Translate(0, speed, 0);
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}



void OnStartUp()
{
    //init shadermgr
    shaderMgr_.InitializeStockShaders();
    shaderMgr.OnInit();

    //load texture
    glGenTextures(1, &textureID);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    Util::LoadTGATexture(textures[2], GL_LINEAR, GL_CLAMP_TO_EDGE);

    //load texture array
    glGenTextures(1, &textureArrayID);
    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
    Util::LoadTGATextureArray(textures, 3, GL_LINEAR, GL_CLAMP_TO_EDGE);


    //load triangle
    triangleBathch.Begin(GL_TRIANGLES, 3, 1);

    triangleBathch.CopyVertexData3f(vertexs);
    triangleBathch.CopyTexCoordData2f(texcoord, 0);
 /*
    triangleBathch.MultiTexCoord2f(0, 0, 0);
    triangleBathch.Vertex3f(-0.5f, 0, 0);

    triangleBathch.MultiTexCoord2f(0, 1, 0);
    triangleBathch.Vertex3f(0.5f, 0, 0);

    triangleBathch.MultiTexCoord2f(0, 0.5f, 1);
    triangleBathch.Vertex3f(0, 0.5f, 0);
*/
    triangleBathch.End();

    //init matrix
    tranformPipeline.SetMatrixStacks(modelviewMatrixStack, projectMatrixStack);
    frustum.SetPerspective(50, 640/480, 1, 100);
    projectMatrixStack.LoadMatrix(frustum.GetProjectionMatrix());
    modelviewMatrixStack.Translate(0,0,-3);
}

void OnShutUp()
{
    glDeleteTextures(1, &textureID);
    glDeleteTextures(1, texturesID);
    shaderMgr.OnUnInit();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutCreateWindow("use texture");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    if (glewInit() != GLEW_OK)
    {
        printf("glew init failed..\n");
        return 0;
    }

    OnStartUp();
    glutMainLoop();
    OnShutUp();

    return EXIT_SUCCESS;
}
