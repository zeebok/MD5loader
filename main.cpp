#include <GL/glew.h>
#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MD5.h"

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* mainWin;
    mainWin = SDL_CreateWindow("MD5 Loader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

    SDL_GLContext context;
    context = SDL_GL_CreateContext(mainWin);

    glewInit();

    glClearColor(0.0, 0.0, 0.0, 1.0);

    MD5* model = new MD5();
//    if(!GLEW_ARB_uniform_buffer_object)
//        model->loadShader("vertshader.glsl", "fragshader.glsl");
//    else
//        model->loadShader("vertshader3.glsl", "fragshader3.glsl");

    if(!model->load("boblampclean.md5mesh", "vertshader3.glsl", "fragshader3.glsl"))
        return -1;

    bool running = true;
    SDL_Event event;

    glm::mat4 pmat = glm::perspective(45.0f, 4.f/3, 0.1f, 1000.0f);

    while(running)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                        running = false;
                    break;
            }
        }

        (model->* model->render)(glm::rotate(glm::translate(pmat, glm::vec3(0.f, -25.f, -150.f)), -90.f, glm::vec3(1.f, 0.f, 0.f)));

        SDL_GL_SwapWindow(mainWin);
    }

    delete model;

    SDL_DestroyWindow(mainWin);
    SDL_GL_DeleteContext(context);

    SDL_Quit();

    return 0;
}
