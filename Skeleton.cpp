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

void Skeleton::prepUniforms(GLuint program)
{
    if(GLEW_ARB_uniform_buffer_object)
    {
        block = glGetUniformBlockIndex(program, "JointBlock");
        glGenBuffers(GL_UNIFORM_BUFFER, &ubo);

        GLfloat buffer[numBones*7];
        int index = 0;

        // Fill buffer with all positions first
        for(int i = 0; i < numBones; ++i)
        {
            buffer[index]   = skeleton[i].position.x;
            buffer[index+1] = skeleton[i].position.y;
            buffer[index+2] = skeleton[i].position.z;
            index += 3;
        }

        // Fill remainder of buffer with orientations
        for(int i = 0; i < numBones; ++i)
        {
            buffer[index]   = skeleton[i].orientation.x;
            buffer[index+1] = skeleton[i].orientation.y;
            buffer[index+2] = skeleton[i].orientation.z;
            buffer[index+3] = skeleton[i].orientation.w;
            index += 4;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(GLfloat)*7*numBones, buffer, GL_STREAM_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glUniformBlockBinding(program, block, globalBind);
        glBindBufferRange(GL_UNIFORM_BUFFER, globalBind, ubo, 0, sizeof(GLfloat)*7*numBones);
    }
    else
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
