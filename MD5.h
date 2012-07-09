#ifndef _MD5_H_
#define _MD5_H_

#include <stdio.h>
#include <string.h>
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Skeleton.h"
#include "Mesh.h"

class MD5
{
    private:
        int count; // How many instances in use
        int numJoints;
        int numMeshes;
        int numVerts;
        int numTris;

        Skeleton* skel;
        Mesh* meshList;

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
