/* 
 * File:   ComplexClass.h
 * Author: qfeuille
 *
 * Created on 23 August 2012, 01:02
 */

#ifndef COMPLEXFUNCTION_H
#define	COMPLEXFUNCTION_H
#include "CAS IO.h"
#include "GLResources.h"
#include <complex>
#include "Plotting/Plotter/Plotter.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>


template<typename T>
    inline T maxAbs (const T& x, const T& y){
        return fabs(x) > fabs(y) ? x: y;
    }

template<typename T=double, int tabSize=10>
class Ridders { 
    const T shrink,shrinksq;
    const T large=1e100;
    const T safety;
    T  tab [tabSize] [tabSize];
public:
    template<typename FunctPtr>
    T operator () (FunctPtr f,const T x,T h){
        if (h==0) return 0;
        T ret; T error;
        tab [0][0]=(f(x+h)-f(x-h))/(2.0*h);
        error=large;
        for (int i=1;i<tabSize;i++){
            h /= shrink;
            tab [0][i] = (f(x+h)-f(x-h))/(2.0*h);
            T fac = shrinksq;
            for (int j=1;j<=i;j++){
             tab [j][i] =( tab [j-1][i]*fac - tab[j-1][i-1]) /(fac-1.0);
             fac*=shrinksq;
             T newerror = maxAbs (tab[j][i]-tab[j-1][i],tab[j][i]-tab[j-1][i-1]);
             if (newerror<= error)
             {error =newerror;
             ret = tab[j][i];
             }
            }
            if (fabs(tab[i][i]-tab[i-1][i-1])>= safety*error) break;
        }
        return ret;
    }
    Ridders (T ishrink=1.4 ,T isafety=2.0): shrink (ishrink),safety(isafety), shrinksq(ishrink*ishrink){
        
                
    }
};


struct Attributes {
        glm::vec2 position; 
        glm::vec4 normal; 
        
    };



class ComplexSurface {
    typedef std::complex<double> complexT;
    Ridders<double> diff;
    CasUnaryFunction f;
    glm::dvec2 side,lowleft;
    GLint Nvertices;
    float rate =0.001;
    struct{
        GLint position;
        GLint normal;
    } attributes;
    struct {
         GLint colour; 
        GLint specular; 
        GLint shininess; 
    } uniforms;
    std::vector<Attributes> vertices;
    Material material;
    void setFunction() ;
    void setSquare(glm::vec2 iside, glm::vec2 ilowleft) ;
    void buildHeightMap(const int N) ;
    GLResources resources;
    void bindVariables ();
    void buildVertices (const int Nvertices);
    void buildIndices (const int Nvertices);
public:
    
    
    void setMaterial (Material imaterial){
        material=imaterial;
    }
    double getHeight (double y, double x, double t);
    void buildSurface (glm::vec2 iside, glm::vec2 ilowleft, int, int,GLuint program);
    void render ();
    
};




inline double ComplexSurface::getHeight (double y, double x, double t){
        complexT z(side.x*(x+1.0)/2+lowleft.x,side.y*(y+1.0)/2+lowleft.y);
        z=f(z);
        return (z.real()*sin(t*rate)+z.imag()*cos(t*rate));
    }
#endif	/* COMPLEXFUNCTION_H */

