#ifndef _MESH_H_
#define _MESH_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh
{
    public:
        struct Vertex
        {
            glm::vec2 uv;
            int weightIndex;
            int weightElem;
        };

        struct Triangle
        {
            GLushort v1, v2, v3;
        };

        struct Weight
        {
            int jointIndex;
            float value;
            glm::vec3 position;
        };

        struct Material
        {
            // Texture and material data (lighting, etc)
        };

        virtual ~Mesh(void);

        /* Setters and Getters */
        virtual void setNumVert(int num);
        virtual void setNumTri(int num);
        virtual void setNumWeight(int num);
        virtual int getNumVert(void);
        virtual int getNumTri(void);
        virtual int getNumWeight(void);
        virtual Vertex getVerts(int index);
        virtual Triangle getTris(int index);
        virtual Weight getWeight(int index);

        virtual bool addVert(Vertex v, int index);
        virtual bool addTri(Triangle t, int index);
        virtual bool addWeight(Weight w, int index);

    private:
        char material[128];
        Vertex* vert;
        Triangle* tri;
        Weight* weight;
        int numVerts;
        int numTris;
        int numWeights;
};

#endif