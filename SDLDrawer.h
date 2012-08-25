/* 
 * File:   SDLDrawer.h
 * Author: qfeuille
 *
 * Created on 04 August 2012, 19:57
 */

#ifndef SDLDRAWER_H
#define	SDLDRAWER_H
#include "SDLlib.h"
#include "ComplexFunction.h"
#include "SurfaceCamera.h"
class SDLDrawer : public SDLBase {

    struct {
        GLuint position;
    } attributes;

    struct {
        GLuint vertexTransform;
        GLuint textureTransform;
        GLuint time;
    } uniforms;
    
    GLuint timestep = 16;
    SurfaceCamera camera;
    ComplexSurface terrain;
public:

    //Initialisation function
    int makeResources();
    //Drawing function
    int drawGLScene();
    void handleMouseMotion(const SDL_MouseMotionEvent& motion);
    SDLDrawer() {
    };
};

inline void SDLDrawer::handleMouseMotion(const SDL_MouseMotionEvent& motion) {
    camera.handleMouseMotion(motion);
    centreMouse();
}; 
       
#endif	/* SDLDRAWER_H */

