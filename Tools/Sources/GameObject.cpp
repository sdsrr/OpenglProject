#include "../Header/GameObject.h"
GObject::GObject()
{
    modelviewStack.LoadIdentity();
}

BatchGObject::BatchGObject(){}
void BatchGObject::Draw(){batch.Draw();}

TriangleGObject::TriangleGObject(){}
void TriangleGObject::Draw(){batch.Draw();}

TriangleStripGObject::TriangleStripGObject(int count) {vertexCnt = count;}
TriangleStripGObject::TriangleStripGObject(){}
void TriangleStripGObject::Draw()
{
    glBindVertexArray(vbo);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCnt);
}
