#pragma once

#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{
    struct PolygonalMesh{

        //numero di celle di ogni dimensione
        unsigned int NumCell0Ds = 0;
        unsigned int NumCell1Ds = 0;
        unsigned int NumCell2Ds = 0;

        //vettori contenenti gli id delle celle
        vector<unsigned int> Cell0DsId = {};
        vector<unsigned int> Cell1DsId = {};
        vector<unsigned int> Cell2DsId = {};

        //coordinate punti
        MatrixXd Cell0DsCoordinates = {};
        //estremi segmenti
        MatrixXi Cell1DsExtrema = {};
        //poligoni
        //vertici
        vector<vector<unsigned int>> Cell2DsVertices = {};
        //lati
        vector<vector<unsigned int>> Cell2DsEdges = {};
        //numero di lati
        vector<unsigned int> Cell2DsNumEdges = {};

        //marker
        map<unsigned int, list<unsigned int>> MarkerCell0Ds = {};
        map<unsigned int, list<unsigned int>> MarkerCell1Ds = {};
        map<unsigned int, list<unsigned int>> MarkerCell2Ds = {};

        //non-zero marker
        unsigned int NumMarkerCell0Ds = 0;
        unsigned int NumMarkerCell1Ds = 0;
        unsigned int NumMarkerCell2Ds = 0;
    };
}