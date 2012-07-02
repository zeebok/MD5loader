#ifndef _MD5_H_
#define _MD5_H_

#include <stdio.h>
#include <string.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Skeleton.h"

class MD5
{
    private:
        struct Vertex
        {
            glm::vec2 uv;
            int weightIndex;
            int weightElem;
        };

        struct Tri
        {
            GLushort vert1, vert2, vert3;
        };

        struct Weight
        {
            int jointIndex;
            float bias;
            glm::vec3 position;
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
        int numVerts;
        int numTris;

        Skeleton* skel;
        Mesh* meshList;

        glm::vec3* jPos;
        glm::vec4* jOrt;

        char name[32];

        GLuint shaderProgram;
        GLuint vao, vbo, ibo;
        GLuint jpLoc, joLoc, mvpLoc;

        void printShaderLog(GLint shader);
        void prepModel(void);
        void renderGL2(void);

    public:
        MD5(void);
        ~MD5(void);

        bool load(const char* filename);
        void save(const char* filename);
        void render(glm::mat4 mvp);

        bool loadShader(const char* vshader, const char* fshader);

        // Usage count
        void add(void);
        void remove(void);
};

#endif
