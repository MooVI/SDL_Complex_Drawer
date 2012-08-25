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
       
         
       /*  for(int i = 0; i < N; i++) {

    for(int j = 0; j < N; j++) {

      float x = (i - N / 2) / (N / 2.0);

      float y = (j - N / 2) / (N / 2.0);

      float d = hypotf(x, y) * 4.0;

      float z = (1 - d * d) * expf(d * d / -2.0);

      graph[i][j] = roundf(z * 127 + 128);

    }

  }*/
        const int N=512;
        terrain.setFunction();
        terrain.setSquare(glm::vec2 (1,1),glm::vec2(-0.5,-0.5));
        terrain.fillHeightMap(N);
        camera.setUpCamera(glm::vec3 (0,1,0), 
                [&](double x, double y){return terrain.getHeight(x,y,fclock.getTotalTime())+0.1;});
           
        resources.make2DTexture();
        glTexImage2D(
                GL_TEXTURE_2D, // target
                0, // level, 0 = base, no minimap,
                GL_RG32F, // internalformat
                N, // width
                N, // height
                0, // border, always 0 in OpenGL ES
                GL_RG, // format]
                GL_FLOAT, // type
                terrain.getGraphData()
                );
        
        // Create an array for 101 * 101 vertices
        glm::vec2 vertices[Nvert][Nvert];
        for (int i = 0; i < Nvert; i++) {
            for (int j = 0; j < Nvert; j++) {
                vertices[i][j].x = (double)(j - halfNvert) / (double) halfNvert;
                vertices[i][j].y = (double) (i - halfNvert) / (double) halfNvert;
            }
        }
        resources.makevBuffer(GL_ARRAY_BUFFER, vertices, sizeof (vertices));

        GLushort indices[Nvertm1 * Nvertm1 * 6];
        int i = 0;
        // Triangles
        for (int y = 0; y <Nvertm1; y++) {
            for (int x = 0; x < Nvertm1; x++) {
                indices[i++] = y * Nvert + x;
                indices[i++] = y * Nvert + x + 1;
                indices[i++] = (y + 1) * Nvert + x + 1;

                indices[i++] = y * Nvert + x;
                indices[i++] = (y + 1) * Nvert + x + 1;
                indices[i++] = (y + 1) * Nvert + x;
            }
        }
        resources.makeeBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof (indices));
        attributes.position = resources.bindAttribute("position");
        uniforms.textureTransform = resources.bindUniform("textureTransform");
        uniforms.vertexTransform = resources.bindUniform("vertexTransform");
        uniforms.time= resources.bindUniform("time");
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
        glm::mat4 vertex_transform = projection * view *model;
        glm::mat4 texture_transform = glm::mat4(1.0f);

        glUniformMatrix4fv(uniforms.vertexTransform, 1, GL_FALSE, glm::value_ptr(vertex_transform));
        glUniformMatrix4fv(uniforms.textureTransform, 1, GL_FALSE, glm::value_ptr(texture_transform));
        glUniform1f (uniforms.time,fclock.getTotalTime());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /* Set texture interpolation mode */

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glUseProgram(resources.getProgram());
        glBindBuffer(GL_ARRAY_BUFFER, resources.getvBuffer());
        glVertexAttribPointer(
                attributes.position, /* attribute */
                2, /* size */
                GL_FLOAT, /* type */
                GL_FALSE, /* normalized? */
                0, /* stride */
                (void*) 0 /* array buffer offset */
                );
        glEnableVertexAttribArray(attributes.position);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.geteBuffer());
        glDrawElements(GL_TRIANGLES,Nvertm1 * Nvertm1 * 6, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(attributes.position);

        /* Draw it to the screen */
        SDL_GL_SwapBuffers();
        fclock.printfps();
        return ( true);
    }
        

