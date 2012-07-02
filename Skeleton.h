#ifndef _SKELETON_H_
#define _SKELETON_H_

#include <glm/glm.hpp>
#include <glm/type_ptr.hpp>

class Skeleton
{
    public:
        struct Bone
        {
            char name[32];
            int parentID;
            glm::vec3 position;
            glm::vec4 orientation;
        };
        Skeleton(int num);
        virtual ~Skeleton(void);

        virtual bool addBone(Bone b);
        virtual void draw(void);
        virtual void setNumBones(int num);

    protected:
        char name[32];
        int skeletonID;
        int numBones;
        Bone* skeleton;
};

#endif
