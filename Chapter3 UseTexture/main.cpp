#include "../Tools/Header/ShaderMgr.h"

GLShaderManager shaderMgr_;
ShaderMgr shaderMgr;
GLBatch triangleBathch;
GLBatch cubeBatch;
BaseShaderParam shaderParam;

GLuint textureID;
GLuint textureArrayID;
GLuint texturesID[29];
GLuint textureCubemap;

const char* textures[] =
{
    "Chapter3 UseTexture/Texture/moon00.tga",
    "Chapter3 UseTexture/Texture/moon01.tga",
    "Chapter3 UseTexture/Texture/moon02.tga",
    "Chapter3 UseTexture/Texture/moon03.tga",
    "Chapter3 UseTexture/Texture/moon04.tga",
    "Chapter3 UseTexture/Texture/moon05.tga",
    "Chapter3 UseTexture/Texture/moon06.tga",
    "Chapter3 UseTexture/Texture/moon07.tga",
    "Chapter3 UseTexture/Texture/moon08.tga",
    "Chapter3 UseTexture/Texture/moon09.tga",
    "Chapter3 UseTexture/Texture/moon10.tga",
    "Chapter3 UseTexture/Texture/moon11.tga",
    "Chapter3 UseTexture/Texture/moon12.tga",
    "Chapter3 UseTexture/Texture/moon13.tga",
    "Chapter3 UseTexture/Texture/moon14.tga",
    "Chapter3 UseTexture/Texture/moon15.tga",
    "Chapter3 UseTexture/Texture/moon16.tga",
    "Chapter3 UseTexture/Texture/moon17.tga",
    "Chapter3 UseTexture/Texture/moon18.tga",
    "Chapter3 UseTexture/Texture/moon19.tga",
    "Chapter3 UseTexture/Texture/moon20.tga",
    "Chapter3 UseTexture/Texture/moon21.tga",
    "Chapter3 UseTexture/Texture/moon22.tga",
    "Chapter3 UseTexture/Texture/moon23.tga",
    "Chapter3 UseTexture/Texture/moon24.tga",
    "Chapter3 UseTexture/Texture/moon25.tga",
    "Chapter3 UseTexture/Texture/moon26.tga",
    "Chapter3 UseTexture/Texture/moon27.tga",
    "Chapter3 UseTexture/Texture/moon28.tga",
};

const char* cubemap[] =
{
    "Tools/Texture/Cubemap/neg_x.tga",
    "Tools/Texture/Cubemap/pos_x.tga",
    "Tools/Texture/Cubemap/neg_y.tga",
    "Tools/Texture/Cubemap/pos_y.tga",
    "Tools/Texture/Cubemap/neg_z.tga",
    "Tools/Texture/Cubemap/pos_z.tga",
};



GLfloat vertexs[]={-0.5f,0,0, 0.5f,0,0, -0.5f,1.0f,0};
GLfloat texcoord[]={ 0.0f,0.0f, 1.0f,0.0f, 0,1.0f };
M3DVector4f color={244.0f/255, 49.0f/255, 166.0f/255 ,0.0f};

GLuint time;
NormalCamera normalCamera;
GLMatrixStack* modelviewStack;

static void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1,1,1,1);
    //消除cubemap采样可能出现的缝隙
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    //use texture2d
    //glBindTexture(GL_TEXTURE_2D, textureID);
    //shaderMgr.UseTexture2d(color, tranformPipeline.GetModelViewProjectionMatrix(), 0);

    //use texture2d array
    /*
    GLuint tick = (time++)/16%29;
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
    shaderMgr.UseTextureArray(color, tranformPipeline.GetModelViewProjectionMatrix(), 0, tick);
    triangleBathch.Draw();
    */

    //use cubemap
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubemap);
    shaderParam.SetMVPMatrix(normalCamera.GetModelviewprojectMatrix());
    shaderParam.SetDiffuseColor(color);
    shaderParam.colorMap[0] = 0;
    shaderMgr.UseCubeMap(shaderParam);
    cubeBatch.Draw();

    glutSwapBuffers();
}

void KeyboardFn(unsigned char key, int x, int y){normalCamera.KeyboardFn(key, x, y);}
void MouseClick(int button, int action, int x, int y){normalCamera.MouseClick(button, action, x, y);}
void MotionFunc(int mouse_x, int mouse_y){normalCamera.MotionFunc(mouse_x, mouse_y);}
void Resize(int w, int h){normalCamera.Resize(w, h);}
void Idle(void){glutPostRedisplay();}

void OnStartUp()
{
    //init shadermgr
    shaderMgr_.InitializeStockShaders();
    shaderMgr.OnInit();

    //load texture
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    Util::LoadTGATexture(textures[0], GL_LINEAR, GL_CLAMP_TO_EDGE);

    //load texture array
    glGenTextures(1, &textureArrayID);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArrayID);
    Util::LoadTGATextureArray(textures, 29, GL_LINEAR, GL_CLAMP_TO_EDGE);

    //load cubemap
    glGenTextures(1, &textureCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureCubemap);
    Util::LoadTGACubemap(cubemap, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);

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

    gltMakeCube(cubeBatch, 1);

    //init camera
    normalCamera.OnInit(640, 480, 50, 1, 2);
    modelviewStack = normalCamera.GetModelviewStack();
    modelviewStack->Translate(0,0,-3);
}

void OnShutUp()
{
    glDeleteTextures(1, &textureID);
    glDeleteTextures(29, texturesID);
    shaderMgr.OnUnInit();
    normalCamera.OnUnInit();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("use texture");
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(KeyboardFn);
    glutReshapeFunc(Resize);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseClick);


    glEnable(GL_DEPTH_TEST);
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
