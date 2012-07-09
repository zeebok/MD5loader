#include "Mesh.h"

Mesh::~Mesh(void)
{
    delete vert;
    delete tri;
    delete weight;
}

void Mesh::setNumVert(int num)
{
    vert = new Vertex[num]();
    numVerts = num;
}

void Mesh::setNumTri(int num)
{
    tri = new Triangle[num]();
    numTris = num;
}

void Mesh::setNumWeight(int num)
{
    weight = new Weight[num]();
    numWeights = num;
}

int Mesh::getNumVert(void)
{
    return numVerts;
}

int Mesh::getNumTri(void)
{
    return numTris;
}

int Mesh::getNumWeight(void)
{
    return numWeights;
}

Mesh::Vertex Mesh::getVerts(int index)
{
    return vert[index];
}

Mesh::Triangle Mesh::getTris(int index)
{
    return tri[index];
}

Mesh::Weight Mesh::getWeight(int index)
{
    return weight[index];
}

bool Mesh::addVert(Vertex v, int index)
{
    if(index >= numVerts)
            return false;

    vert[index] = v;

    return true;
}

bool Mesh::addTri(Triangle t, int index)
{
    if(index >= numTris)
        return false;

    tri[index] = t;

    return true;
}

bool Mesh::addWeight(Weight w, int index)
{
    if(index >= numWeights)
        return false;

    weight[index] = w;

    return true;
}