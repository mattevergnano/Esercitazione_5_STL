#pragma once
#include <iostream>
#include "PolygonalMesh.hpp"
namespace PolygonalLibrary{
    //importare mesh, controlla che tutte le importazioni siano andate a buon fine
    bool ImportMesh(PolygonalMesh& mesh);
    //importo punti
    bool ImportPoints(PolygonalMesh& mesh);
    //importo lati
    bool ImportEdges(PolygonalMesh& mesh);
    //importo poligoni
    bool ImportPolygons(PolygonalMesh& mesh);
    //test del programma
    int testProgram(PolygonalMesh& mesh);
}