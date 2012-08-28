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
        void renderGL2(glm::mat4 mvp);
        void renderGL3(glm::mat4 mvp);

    public:
        MD5(void);
        ~MD5(void);

        bool load(const char* filename);
        bool load(const char* filename, const char* vshader, const char* fshader);
        bool loadShader(const char* vshader, const char* fshader);
        void save(const char* filename);

        void (MD5::*render)(glm::mat4 mvp);

        // Usage count
        void add(void);
        void remove(void);
};

#endif
