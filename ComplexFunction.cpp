/* 
 * File:   ComplexFunction.cpp
 * Author: qfeuille
 *
 * Created on 25 August 2012, 17:29
 */

#include "ComplexFunction.h"

/*
 * 
 */
void ComplexSurface::setFunction() {

    bool success = false;
    std::string fname;
    while (!success) {
        std::cout << "Enter function: ";
        std::getline(std::cin, fname);
        f.createFunction(fname, fname, &success);
    }
}


void ComplexSurface::setSquare(glm::vec2 iside,glm::vec2 ilowleft) {
    side.x = iside.x;
    side.y = iside.y;
    lowleft.x = ilowleft.x;
    lowleft.y = ilowleft.y;
    //    pos.x=ipos.x;
    //    pos.y=ipos.y;
}

void ComplexSurface::fillHeightMap(const int N) {
    double x = lowleft.x;
    double y = lowleft.y;
    graph.resize(N * N * 2);
    complexT z;
    for (int i = 0; i < N; i++) {
        x += side.x / ((double) N);
        y = lowleft.y;
        for (int j = 0; j < N; j++) {
            y += side.y / ((double) N);
            z = f(complexT(x, y));
            graph[(i * N + j)*2] = z.real();
            graph[(i * N + j)*2 + 1] = z.imag();
        }
    }
}

void ComplexSurface::fillNormals(const int N) {
    double x = lowleft.x;
    double y = lowleft.y;
    normal.resize(N * N * 4);
    for (int i = 0; i < N; i++) {
        x += side.x / ((double) N);
        y = lowleft.y;
        for (int j = 0; j < N; j++) {
            y += side.y / ((double) N);

            normal[(i * N + j)*4] = diff([&] (double inx) {
                return f(std::complex<double>(inx, y)).real();
            }, x, 0.1);

            normal[(i * N + j)*4 + 1] = diff([&] (double iny) {
                return f(std::complex<double>(x, iny)).real();
            }, y, 0.1);

            normal[(i * N + j)*4 + 2] = diff([&] (double inx) {
                return f(std::complex<double>(inx, y)).imag();
            }, x, 0.1);

            normal[(i * N + j)*4 + 3] = diff([&] (double iny) {
                return f(std::complex<double>(x, iny)).imag();
            }, y, 0.1);
        }
    }
}

