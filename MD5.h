#ifndef _MD5_H_
#define _MD5_H_

#include <glm/glm.hpp>
#include <GL/glew.h>

class MD5
{
    private:
        struct Vertex
        {
            float u, v;
            int weightIndex;
            int weightElem;
        };

        struct Tri
        {
            int vert1, vert2, vert3;
        };

        struct Weight
        {
            int jointIndex;
            float bias;
            float x, y, z;
        };

        struct Joint
        {
            char name[64];
            int parentIndex;
            float x, y, z;
            float ox, oy, oz, ow;
        };

        struct Mesh
        {
            char material[128];
            Vertex* vertex;
            Tri* tri;
            Weight* weight;
            int numVerts;
            int numTris;
            int numWeights;
        };

        int count; // How many instances in use
        int numJoints;
        int numMeshes;

        Joint* jointList;
        Mesh* meshList;

        int vbo,ibo;
        void prepModel(void);
        void renderCompat(void);

    public:
        bool load(const char* filename);
        void save(const char* filename);
        void render(glm::mat4 mvp);

        // Usage count
        void add(void);
        void remove(void);
};

#endif