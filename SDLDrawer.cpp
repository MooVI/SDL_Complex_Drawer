/* 
 * File:   SDLDrawer.cpp
 * Author: qfeuille
 * 
 * Created on 04 August 2012, 19:57
 */

#include "SDLDrawer.h"



constexpr int Nvert=101;
        constexpr int halfNvert=(Nvert-1)/2;
        const int Nvertm1=Nvert-1;

int SDLDrawer::makeResources() {
    //Comment this out before debugging!
    //SDL_WM_GrabInput(SDL_GRAB_ON);
    resources.makeShader(GL_VERTEX_SHADER, "vshader.vert");
    resources.makeShader(GL_FRAGMENT_SHADER, "fshader.frag");
    resources.makeProgram();
    terrain.buildSurface(glm::vec2(1, 1),
            glm::vec2(-0.5, -0.5),
            512, 101,
            resources.getProgram());
    Material surfaceMaterial;
    surfaceMaterial.shininess = 0.3;
    surfaceMaterial.colour = glm::vec4 (1.0,0.0,0.0,1.0);
    surfaceMaterial.specular = glm::vec4 (1.0,1.0,1.0,1.0);
    terrain.setMaterial(surfaceMaterial);
    camera.setUpCamera(glm::vec3(0, 1, 0),
            [&](double x, double y) {
        return terrain.getHeight(x, y, fclock.getTotalTime()) + 0.1;});
    uniforms.projection = resources.bindUniform("projection");
    uniforms.modelview = resources.bindUniform("modelview");
    uniforms.time = resources.bindUniform("time");
};
    
        int SDLDrawer::drawGLScene() {
        GLuint timeElapsed=fclock.getTimeElapsed();
        fclock.addTimeElapsed (timeElapsed);
        while (fclock.isTimeAvailable(timestep)){
            
            
            
        }
        camera.update(timeElapsed);
        
        fclock.resetStart();
        /* Clear The Screen And The Depth Buffer */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 model;
        model = camera.getCameraMatrix();
        glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 projection = glm::perspective(45.0f, screenRatio, 0.1f, 10.0f);
        glm::mat4 modelview = view *model;

        glUniformMatrix4fv(uniforms.modelview, 1, GL_FALSE, glm::value_ptr(modelview));
        glUniformMatrix4fv(uniforms.projection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f (uniforms.time,fclock.getTotalTime());
        terrain.render();
        /* Draw it to the screen */
        SDL_GL_SwapBuffers();
        fclock.printfps();
        return ( true);
    }
        

