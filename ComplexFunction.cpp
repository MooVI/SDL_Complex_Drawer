/* 
 * File:   ComplexFunction.cpp
 * Author: qfeuille
 *
 * Created on 25 August 2012, 17:29
 */

#include <vector>

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

void ComplexSurface::setSquare(glm::vec2 iside, glm::vec2 ilowleft) {
    side.x = iside.x;
    side.y = iside.y;
    lowleft.x = ilowleft.x;
    lowleft.y = ilowleft.y;
    //    pos.x=ipos.x;
    //    pos.y=ipos.y;
}

void ComplexSurface::buildHeightMap(const int N) {
    double x = lowleft.x;
    double y = lowleft.y;
    std::vector<GLfloat> graph(N * N * 2);
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
            &graph.front()
            );
}



void ComplexSurface::buildSurface(
        const glm::vec2 iside,
        const glm::vec2 ilowleft,
        const int Ntexture,
        const int Nvert,
        GLuint program) {
    this->resources.addProgram(program);
    this->setFunction();
    this->setSquare(iside, ilowleft);
    this->buildHeightMap(Ntexture);
    this->buildVertices(Nvert);
    this->buildIndices(Nvert);
    this->bindVariables();
    Nvertices = Nvert;

}

void ComplexSurface::buildIndices(const int Nvert) {
    const int Nvertm1(Nvert - 1);
    std::vector<GLushort> indices(Nvertm1 * Nvertm1 * 6);
    int i = 0;
    // Triangles
    for (int y = 0; y < Nvertm1; y++) {
        for (int x = 0; x < Nvertm1; x++) {
            indices[i++] = y * Nvert + x;
            indices[i++] = y * Nvert + x + 1;
            indices[i++] = (y + 1) * Nvert + x + 1;

            indices[i++] = y * Nvert + x;
            indices[i++] = (y + 1) * Nvert + x + 1;
            indices[i++] = (y + 1) * Nvert + x;
        }
    }
    resources.makeeBuffer(GL_ELEMENT_ARRAY_BUFFER, &indices.front(), indices.size() * sizeof (GLushort));


}

void ComplexSurface::buildVertices(const int Nvert) {
    // Create an array for 101 * 101 vertices
    const int halfNvert((Nvert - 1) / 2);
    vertices.resize(Nvert * Nvert);
    for (int i = 0; i < Nvert; i++) {
        for (int j = 0; j < Nvert; j++) {
            vertices[i * Nvert + j].position.x = (double) (j - halfNvert) / (double) halfNvert;
            vertices[i * Nvert + j].position.y = (double) (i - halfNvert) / (double) halfNvert;

            vertices[i * Nvert + j].normal.w == diff(
                    [&] (double inx) {
                return f(complexT(inx, vertices[i * Nvert + j].position.y)).real();
            }
            , vertices[i * Nvert + j].position.x, 0.1);

            vertices[i * Nvert + j].normal.x == diff(
                    [&] (double iny) {
                return f(complexT(vertices[i * Nvert + j].position.x, iny)).real();
            }
            , vertices[i * Nvert + j].position.y, 0.1);

            vertices[i * Nvert + j].normal.y == diff(
                    [&] (double inx) {
                return f(complexT(inx, vertices[i * Nvert + j].position.y)).imag();
            }
            , vertices[i * Nvert + j].position.x, 0.1);

            vertices[i * Nvert + j].normal.z == diff(
                    [&] (double iny) {
                return f(complexT(vertices[i * Nvert + j].position.x, iny)).imag();
            }
            , vertices[i * Nvert + j].position.y, 0.1);
        }
    }

    resources.makevBuffer(GL_ARRAY_BUFFER, &vertices.front(), vertices.size() * sizeof (Attributes));


}

void ComplexSurface::bindVariables() {
    attributes.position = resources.bindAttribute("position");
    attributes.normal   = resources.bindAttribute("normal");
    uniforms.colour     = resources.bindUniform("material.colour");
    uniforms.shininess  = resources.bindUniform("material.shininess");
    uniforms.specular   = resources.bindUniform("material.specular");
}

void ComplexSurface::render() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    /* Set texture interpolation mode */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glUseProgram(resources.getProgram());
    glBindBuffer(GL_ARRAY_BUFFER, resources.getvBuffer());
    glUniform1f (uniforms.shininess, material.shininess);
    glUniform4fv (uniforms.colour,1, glm::value_ptr(material.colour));
    glUniform4fv (uniforms.specular,1, glm::value_ptr(material.specular));
    glVertexAttribPointer(
            attributes.position, /* attribute */
            2, /* size */
            GL_FLOAT, /* type */
            GL_FALSE, /* normalized? */
            sizeof (Attributes), /* stride */
            (void*) offsetof (Attributes, position) /* array buffer offset */
            );
    glVertexAttribPointer(
            attributes.normal, /* attribute */
            4, /* size */
            GL_FLOAT, /* type */
            GL_FALSE, /* normalized? */
            sizeof (Attributes), /* stride */
            (void*) offsetof (Attributes, normal) /* array buffer offset */
            );
    glEnableVertexAttribArray(attributes.position);
    glEnableVertexAttribArray(attributes.normal);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resources.geteBuffer());
    glDrawElements(GL_TRIANGLES, (Nvertices - 1) * (Nvertices - 1) * 6, GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(attributes.normal);
    glDisableVertexAttribArray(attributes.position);




}