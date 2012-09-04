#ifndef _SKELETON_H_
#define _SKELETON_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Skeleton
{
    public:
        struct Bone
        {
            char name[32];
            int parentIndex;
            glm::vec3 position;
            glm::vec4 orientation;
        };
        Skeleton(int num);
        virtual ~Skeleton(void);

        virtual bool addBone(Bone b);
        virtual void prepUniforms(GLuint program);
        virtual void draw(glm::mat4 mvp);

    protected:
        char name[32];
        int skeletonID;
        int numBones;
        int currentBone;
        Bone* skeleton;
        GLuint ubo, block, globalBind;

        // For OpenGL 2.1 uniform compatability only
        glm::vec3* position;
        glm::vec4* orientation;
};

#endif
