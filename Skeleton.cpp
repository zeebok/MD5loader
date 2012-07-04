#include "Skeleton.h"

Skeleton::Skeleton(int num)
{
    currentBone = 0;
    numBones = num;
    skeleton = new Bone[num];
}

Skeleton::~Skeleton(void)
{
    delete skeleton;
}

bool Skeleton::addBone(Bone b)
{
    if(currentBone >= numBones)
        return false;

    skeleton[currentBone] = b;
    currentBone++;

    return true;
}

void Skeleton::prepUniforms(void)
{
    if(GLEW_VERSION_2_1)
    {
        // Is this needed or just perfor NUMBONES glUniform calls?
        position = new glm::vec3[numBones];
        orientation = new glm::vec4[numBones];

        for(int i = 0; i < numBones; ++i)
        {
            position[i] = skeleton[i].position;
            orientation[i] = skeleton[i].orientation;
        }
    }
    else
    {
        // TODO: Finish binding UBO
        glGenBuffers(GL_UNIFORM_BUFFER, &ubo);
    }
}

void Skeleton::draw(glm::mat4 mvp)
{
    glPointSize(4.f);
    glUseProgram(0);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(mvp));
    for(int i = 0; i < numBones; ++i)
    {
        // Draw joint position
        glColor3f(1.f, 0.f, 0.f);
        glBegin(GL_POINTS);
        glVertex3fv(glm::value_ptr(skeleton[i].position));
        glEnd();

        // Draw bone
        glColor3f(0.f, 1.f, 0.f);
        glBegin(GL_LINES);
        if(skeleton[i].parentIndex != -1)
        {
            glVertex3fv(glm::value_ptr(skeleton[i].position));
            glVertex3fv(glm::value_ptr(skeleton[skeleton[i].parentIndex].position));
        }
        glEnd();
    }
    glPopMatrix();
}