#include "Skeleton.h"

Skeleton::Skeleton(int num)
{
    numBones = num;
    skeleton = new Bone[num];
}

Skeleton::~Skeleton(void)
{
    delete skeleton;
}

bool Skeleton::addBone(Bone b)
{

}

void Skeleton::draw(void)
{
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glBegin(GL_LINES);
    for(int i = 0; i < numBones; ++i)
    {
        if(skeleton[i].parentID) != -1)
        {
            glVertex3fv(glm::value_ptr(skeleton[i].position));
            glVertex3fv(glm::value_ptr(skeleton[skeleton[i].parentID].position));
        }
    }
}

void Skeleton::setNumBones(int num)
{
    numBones = num;
    skeleton = new Bone[num];
}
