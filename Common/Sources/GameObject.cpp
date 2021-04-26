#include "../Header/GameObject.h"


YoyoMoveAction::YoyoMoveAction(float speed, M3DVector3f offset)
{
    _speed = speed;
    memset(_dir, 0, 3);
    memset(_pos, 0, 3);
    memcpy(_offset, offset, 3*sizeof(float));
}

void YoyoMoveAction::Run(GLMatrixStack* modelStack)
{
    if (_pos[0] < -_offset[0] || _pos[0] > _offset[0])
        _dir[0] *= -1;
    if (_pos[1] < -_offset[1] || _pos[1] > _offset[1])
        _dir[1] *= -1;
    if (_pos[1] < -_offset[1] || _pos[1] > _offset[0])
        _dir[1] *= -1;
    _pos[0] = _dir[0] * _speed;
    _pos[1] = _dir[1] * _speed;
    _pos[2] = _dir[2] * _speed;
    modelStack->Translate(_pos[0], _pos[1], _pos[2]);
}

void SequenceAction::Run(GLMatrixStack* modelStack)
{
    for (std::list<Action>::iterator child = _children.begin(); child != _children.end(); ++child)
        child->Run(modelStack);
}


GObject::GObject()
{
    modelStack.LoadIdentity();
}

void GObject::RunAction()
{
    if (_action != NULL)
        _action->Run(&modelStack);
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
