#include "../Header/GameObject.h"
GObject::GObject()
{
    modelStack.LoadIdentity();
}

BatchGObject::BatchGObject(){}
void BatchGObject::Draw(){batch.Draw();}

void BatchGObject::CreateTriangle(float length, float width, int textureUnit)
{
    batch.Begin(GL_TRIANGLES, 6, 1);
    batch.MultiTexCoord2f(0,0,0);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(0,0,0);
    batch.MultiTexCoord2f(0,1,0);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(length,0,0);
    batch.MultiTexCoord2f(0,0,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(0,width,0);

    batch.MultiTexCoord2f(0,0,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(0,width,0);
    batch.MultiTexCoord2f(0,1,0);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(length,0,0);
    batch.MultiTexCoord2f(0,1,1);
    batch.Normal3f(0,0,1);
    batch.Vertex3f(length,width,0);
    batch.End();
}

void BatchGObject::CreateCube(float length, float width, float height, int textureUnit)
{
    batch.Begin(GL_TRIANGLES, 36, 1);
    {
        //back
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(0,0,0);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(0,height,0);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(length,0,0);

        //
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(length,0, 0);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(0,height,0);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,0,1);
        batch.Vertex3f(length,height,0);
    }

    {
        //font
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(0,0,width);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(length,0,width);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(0,height,width);

        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(0,height,width);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(length,0,width);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,0,-1);
        batch.Vertex3f(length,height,width);
    }

    {
        //left
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,0,0);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,0,width);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,height,0);

        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,height,0);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,0,width);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(-1,0,0);
        batch.Vertex3f(0,height,width);
    }
    {
        //right
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,0,0);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,height,0);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,0,width);


        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,0,width);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,height,0);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(1,0,0);
        batch.Vertex3f(length,height,width);
    }
    {
        //top
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(0,height,0);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(0,height,width);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(length,height,width);

        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(0,height,0);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(length,height,width);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,1,0);
        batch.Vertex3f(length,height,0);
    }
    {
        //bottom
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(0,0,0);
        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(length,0,width);
        batch.MultiTexCoord2f(0,0,1);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(0,0,width);

        batch.MultiTexCoord2f(0,1,1);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(length,0,width);
        batch.MultiTexCoord2f(0,0,0);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(0,0,0);
        batch.MultiTexCoord2f(0,1,0);
        batch.Normal3f(0,-1,0);
        batch.Vertex3f(length,0,0);
    }
    batch.End();
}


TriangleGObject::TriangleGObject(){}
void TriangleGObject::Draw(){batch.Draw();}

TriangleStripGObject::TriangleStripGObject(int count) {vertexCnt = count;}
TriangleStripGObject::TriangleStripGObject(){}
void TriangleStripGObject::Draw()
{
    glBindVertexArray(vbo);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexCnt);
}
