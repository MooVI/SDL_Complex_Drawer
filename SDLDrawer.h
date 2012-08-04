/* 
 * File:   SDLDrawer.h
 * Author: qfeuille
 *
 * Created on 04 August 2012, 19:57
 */

#ifndef SDLDRAWER_H
#define	SDLDRAWER_H
#include "SDLlib.h"
#include "CAS IO.h"
#include <complex>
class SDLDrawer : public SDLBase {

    struct {
        GLuint position;
    } attributes;

    struct {
        GLuint vertexTransform;
        GLuint textureTransform;
    } uniforms;
    GLuint timestep = 16;
    FlyingCamera camera;
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

