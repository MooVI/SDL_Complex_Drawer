/* 
 * File:   main.cpp
 * Author: qfeuille
 *
 * Created on 26 July 2012, 17:36
 */


#include "SDLDrawer.h"
#include "ComplexFunction.h"
/*
 * 
 */
int main() {
    
    SDLDrawer window;
    window.init_SDL(800,800,16,1,1,1,"Test Window",60);
    window.message_Loop();
    

    return 0;
}

