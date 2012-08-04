/* 
 * File:   SDLDrawer.cpp
 * Author: qfeuille
 * 
 * Created on 04 August 2012, 19:57
 */

#include "SDLDrawer.h"

int SDLDrawer::makeResources() {
        //Comment this out before debugging!
        //SDL_WM_GrabInput(SDL_GRAB_ON);
        
        camera.setUpCamera(0,-0.4,0);
        
        resources.makeShader(GL_VERTEX_SHADER, "vshader.vert");
        resources.makeShader(GL_FRAGMENT_SHADER, "fshader.frag");
        resources.makeProgram();
        CasUnaryFunction f;
        bool success=false;
        while (!success){
        std::string fname;
        std::cout<<"Enter function: ";
        std::getline(std::cin,fname);
        f.createFunction(fname,fname,&success);
        }
        std::complex<double> five=(5,0);
        std::complex<double> one=(1,-1);
        std::complex<double> answer=one*five;
        // Create our datapoints, store it as bytes
        constexpr int N =2048;
        GLbyte graph[N][N];
        double boundx=4;
        double boundy=4;
         double x = -boundx/ 2.0; 
         double y = -boundy/2.0;
        for (int i = 0; i < N; i++) {
            x+= boundx/ ((double) N);
            y = -boundy/2.0;
            for (int j = 0; j < N; j++) {
                
                y+= boundy/ ((double) N);
                double z = f (std::complex<double> (x,y)).real();
                graph[i][j] = roundf(z * 127 + 128);
            }
        }
        resources.make2DTexture();
        glTexImage2D(
                GL_TEXTURE_2D, // target
                0, // level, 0 = base, no minimap,
                GL_LUMINANCE, // internalformat
                N, // width
                N, // height
                0, // border, always 0 in OpenGL ES
                GL_LUMINANCE, // format]
                GL_UNSIGNED_BYTE, // type
                graph
                );
        constexpr int Nvert=101;
        constexpr int halfNvert=(Nvert-1)/2;
        // Create an array for 101 * 101 vertices
        glm::vec2 vertices[Nvert][Nvert];
        for (int i = 0; i < Nvert; i++) {
            for (int j = 0; j < Nvert; j++) {
                vertices[i][j].x = (j - 50) / 50.0;
                vertices[i][j].y = (i - 50) / 50.0;
            }
        }
        resources.makevBuffer(GL_ARRAY_BUFFER, vertices, sizeof (vertices));

        GLushort indices[100 * 100 * 6];
        int i = 0;
        // Triangles
        for (int y = 0; y < 100; y++) {
            for (int x = 0; x < 100; x++) {
                indices[i++] = y * 101 + x;
                indices[i++] = y * 101 + x + 1;
                indices[i++] = (y + 1) * 101 + x + 1;

                indices[i++] = y * 101 + x;
                indices[i++] = (y + 1) * 101 + x + 1;
                indices[i++] = (y + 1) * 101 + x;
            }
        }
        resources.makeeBuffer(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof (indices));
        attributes.position = resources.bindAttribute("position");
        uniforms.textureTransform = resources.bindUniform("textureTransform");
        uniforms.vertexTransform = resources.bindUniform("vertexTransform");
    };
    
        int SDLDrawer::drawGLScene() {
        GLuint timeElapsed=fclock.getTimeElapsed();
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
        glDrawElements(GL_TRIANGLES, 100 * 100 * 6, GL_UNSIGNED_SHORT, 0);

        glDisableVertexAttribArray(attributes.position);

        /* Draw it to the screen */
        SDL_GL_SwapBuffers();
        fclock.printfps();
        return ( true);
    }

