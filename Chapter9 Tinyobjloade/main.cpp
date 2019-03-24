#include "../Tools/Header/Tools.h"
#include "../Tools/Header/ShaderMgr.h"
#include "../Tools/Header/Tinyobjloader.h"
#include "../Tools/Header/Trackball.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>


float maxExtent;
float bmin[3], bmax[3];
int width = 768;
int height = 768;

double prevMouseX, prevMouseY;
bool mouseLeftPressed;
bool mouseMiddlePressed;
bool mouseRightPressed;
float curr_quat[4];
float prev_quat[4];
float eye[3], lookat[3], up[3];

const char* modelName = Util::GetFullPath("Tools/Models/OBJ/regr01.obj");
Tinyobjloader modelObj;

static void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 0.01f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  width = w;
  height = h;
}

static void key(unsigned char key, int x, int y)
{
    // Move camera
    float mv_x = 0, mv_y = 0, mv_z = 0;
    if (key == 'w')
      mv_x += 1;
    else if (key == 's')
      mv_x += -1;
    else if (key == 'a')
      mv_y += 1;
    else if (key == 'd')
      mv_y += -1;
    else if (key == 'q')
      mv_z += 1;
    else if (key == 'e')
      mv_z += -1;
}

static void mouseClick(int button, int action, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (action == GLUT_DOWN) {
      mouseLeftPressed = true;
      trackball(prev_quat, 0.0, 0.0, 0.0, 0.0);
    } else if (action == GLUT_UP) {
      mouseLeftPressed = false;
    }
  }
  if (button == GLUT_RIGHT_BUTTON) {
    if (action == GLUT_DOWN) {
      mouseRightPressed = true;
    } else if (action == GLUT_UP) {
      mouseRightPressed = false;
    }
  }
  if (button == GLUT_MIDDLE_BUTTON) {
    if (action == GLUT_DOWN) {
      mouseMiddlePressed = true;
    } else if (action == GLUT_UP) {
      mouseMiddlePressed = false;
    }
  }
}

static void motionFunc(int mouse_x, int mouse_y)
{
  float rotScale = 1.0f;
  float transScale = 2.0f;

  if (mouseLeftPressed)
  {
    trackball(prev_quat, rotScale * (2.0f * prevMouseX - width) / (float)width,
              rotScale * (height - 2.0f * prevMouseY) / (float)height,
              rotScale * (2.0f * mouse_x - width) / (float)width,
              rotScale * (height - 2.0f * mouse_y) / (float)height);

    add_quats(prev_quat, curr_quat, curr_quat);
  }
  else if(mouseMiddlePressed)
  {
    eye[0] -= transScale * (mouse_x - prevMouseX) / (float)width;
    lookat[0] -= transScale * (mouse_x - prevMouseX) / (float)width;
    eye[1] += transScale * (mouse_y - prevMouseY) / (float)height;
    lookat[1] += transScale * (mouse_y - prevMouseY) / (float)height;
  }
  else if (mouseRightPressed)
  {
    eye[2] += transScale * (mouse_y - prevMouseY) / (float)height;
    lookat[2] += transScale * (mouse_y - prevMouseY) / (float)height;
  }

  // Update mouse point
  prevMouseX = mouse_x;
  prevMouseY = mouse_y;
}

static void onStartup()
{
  trackball(curr_quat, 0, 0, 0, 0);

  eye[0] = 0.0f;
  eye[1] = 0.0f;
  eye[2] = 3.0f;

  lookat[0] = 0.0f;
  lookat[1] = 0.0f;
  lookat[2] = 0.0f;

  up[0] = 0.0f;
  up[1] = 1.0f;
  up[2] = 0.0f;

  //load model data
  modelObj.LoadObjAndConvert(bmin, bmax, modelName);
  maxExtent = 0.5f * (bmax[0] - bmin[0]);
  if (maxExtent < 0.5f * (bmax[1] - bmin[1]))
    maxExtent = 0.5f * (bmax[1] - bmin[1]);
  if (maxExtent < 0.5f * (bmax[2] - bmin[2]))
    maxExtent = 0.5f * (bmax[2] - bmin[2]);
}

static void onShutup()
{


}

static void display(void)
{
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    // camera & rotate
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    GLfloat mat[4][4];
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0], up[1], up[2]);
    build_rotmatrix(mat, curr_quat);
    glMultMatrixf(&mat[0][0]);

    // Fit to -1, 1
    glScalef(1.0f / maxExtent, 1.0f / maxExtent, 1.0f / maxExtent);
    // Centerize object.
    glTranslatef(-0.5 * (bmax[0] + bmin[0]), -0.5 * (bmax[1] + bmin[1]), -0.5 * (bmax[2] + bmin[2]));
    modelObj.Draw();
    glutSwapBuffers();
}

static void idle(void)
{
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
  glutInitWindowSize(640,480);
  glutInitWindowPosition(10,10);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("model tinyobjloade");

  glutReshapeFunc(resize);
  glutDisplayFunc(display);
  glutKeyboardFunc(key);
  glutIdleFunc(idle);
  glutMotionFunc(motionFunc);
  glutMouseFunc(mouseClick);

  if (glewInit() != GLEW_OK)
  {
    std::cerr << "Failed to initialize GLEW." << std::endl;
    return -1;
  }

  onStartup();
  glutMainLoop();
  onShutup();
}
