#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;
namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh){
    ImportPoints(mesh);
    ImportEdges(mesh);
    ImportPolygons(mesh);
    return true;
}
bool ImportPoints(PolygonalMesh& mesh){
    fstream myfile("./Cell0Ds.csv");
    if(myfile.fail())
        return false;

    string line;
    string tmp;
    list<string> listLines; //lista di stringhe per salvare le righe del file (devo avere prima la dimensione totale)

    while(getline(myfile,line))
        listLines.push_back(line);

    //chiudo il file
    myfile.close();

    //rimuovo la riga di intestazione
    listLines.pop_front();

    //salvo il numero di celle totali
    mesh.NumCell0Ds=listLines.size();


    //stampo errore se non ho celle 0D
    if(mesh.NumCell0Ds == 0)
    {
        cerr << "Non ci sono celle 0D" << endl;
        return false;
    }
    //riservo in memoria il numero di celle totali
    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3,mesh.NumCell0Ds);
    
    for (string& line : listLines){
        istringstream s(line);


        unsigned int id;
        unsigned int marker;

        getline(s,tmp,';');
        id = stoi(tmp);
        mesh.Cell0DsId.push_back(id);
        getline(s,tmp,';');
        marker = stoi(tmp);
        getline(s,tmp,';');
        mesh.Cell0DsCoordinates(0,id) = stod(tmp);
        getline(s,tmp,';');
        mesh.Cell0DsCoordinates(1,id) = stod(tmp);


        ///memorizzo i marker
        if(marker != 0)
        {
            const auto it = mesh.MarkerCell0Ds.find(marker);
            if(it == mesh.MarkerCell0Ds.end())
            {
                mesh.MarkerCell0Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
            mesh.NumMarkerCell0Ds += 1;
        };

    };
    return true;
};

bool ImportEdges(PolygonalMesh& mesh)
{
    fstream myfile;
    myfile.open("./Cell1Ds.csv");
    if(myfile.fail())
        return false;
    
    list<string> listLines;
    string line;
    while (getline(myfile, line))
        listLines.push_back(line);

    myfile.close();

    // tolgo intestazione
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();
    //controllo ce ne siano

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    //riservo spazio in memoria
    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(3, mesh.NumCell1Ds);

    for (string& line : listLines){
        istringstream s(line);

        string tmp;
        unsigned int id;
        unsigned int marker;

        getline(s,tmp,';');
        id = stoi(tmp);
        mesh.Cell1DsId.push_back(id);
        getline(s,tmp,';');
        marker = stoi(tmp);
        getline(s,tmp,';');
        mesh.Cell1DsExtrema(0,id) = stoi(tmp);
        getline(s,tmp,';');
        mesh.Cell1DsExtrema(1,id) = stoi(tmp);

        if(marker != 0)
        {
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end())
            {
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
            mesh.NumMarkerCell1Ds += 1;
        }
    };
    return true;
};

bool ImportPolygons(PolygonalMesh& mesh)
{
    fstream myfile;
    myfile.open("./Cell2Ds.csv");
    if(myfile.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(myfile, line))
        listLines.push_back(line);

    myfile.close();

    // tolgo intestazione
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();
    //controllo ce ne siano

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    //riservo spazio in memoria
    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (string& line : listLines){
        istringstream s(line);

        string tmp;
        unsigned int id;
        unsigned int marker;
        unsigned int numVert;
        unsigned int numEdges;

        getline(s,tmp,';');
        id = stoi(tmp);
        mesh.Cell2DsId.push_back(id);
        getline(s,tmp,';');
        marker = stoi(tmp);
        getline(s,tmp,';');
        numVert = stoi(tmp);
        mesh.Cell2DsNumEdges.push_back(numVert);
        vector<unsigned int> vertices;
        vertices.reserve(numVert);
        
        for(unsigned int i = 0; i < numVert;i++){
            getline(s,tmp,';');
            vertices.push_back(stoi(tmp));
        };
        mesh.Cell2DsVertices.push_back(vertices);
        getline(s,tmp,';');
        numEdges = stoi(tmp);
        vector<unsigned int> edges;
        edges.reserve(numEdges);
        for(unsigned int i = 0; i < numEdges;i++){
            getline(s,tmp,';');
            edges.push_back(stoi(tmp));
        };
        mesh.Cell2DsEdges.push_back(edges);

        if(marker != 0)
        {
            const auto it = mesh.MarkerCell2Ds.find(marker);
            if(it == mesh.MarkerCell2Ds.end())
            {
                mesh.MarkerCell2Ds.insert({marker, {id}});
            }
            else
            {
                it->second.push_back(id);
            }
            mesh.NumMarkerCell2Ds += 1;
        }
    }
    
    return true;
}
int testProgram(PolygonalMesh& mesh)
{
    //marker, controllo le dimensioni e confronto con il numero di marker non 0
    unsigned int num0 = 0;
    for(const auto& it : mesh.MarkerCell0Ds){
        for(const auto& it1 : it.second){
            num0 += 1;
        }
    }
    if(num0 != mesh.NumMarkerCell0Ds)
        cout << "Errore nel caricamento dei marker" << endl;
    

    unsigned int num1 = 0;
    for(const auto& it : mesh.MarkerCell1Ds){
        for(const auto& it1 : it.second){
            num1 += 1;
        }
    }
    if(num1 != mesh.NumMarkerCell1Ds)
        cout << "Errore nel caricamento dei marker" << endl;


    unsigned int num2 = 0;
    for(const auto& it : mesh.MarkerCell2Ds){
        for(const auto& it1 : it.second){
            num2 += 1;
        }
    }
    if(num2 != mesh.NumMarkerCell2Ds)
        cout << "Errore nel caricamento dei marker" << endl;
    
    
    //edge
    unsigned int extr1 = 0;
    unsigned int extr2 = 0;
    for(unsigned int id = 0; id<mesh.NumCell1Ds;id ++){
        extr1 = mesh.Cell1DsExtrema(0,id); //id primo estremo
        extr2 = mesh.Cell1DsExtrema(1,id); //id secondo estremo
        double x1 = mesh.Cell0DsCoordinates(0,extr1);
        double y1 = mesh.Cell0DsCoordinates(1,extr1);
        double x2 = mesh.Cell0DsCoordinates(0,extr2);
        double y2 = mesh.Cell0DsCoordinates(1,extr2);
        double dist = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
        if(dist < 1e-12){
            cout << "Errore, segmento di lunghezza nulla!" << endl;
            return 1;
        };
    };

    //area
    
    for(unsigned int i = 0; i < mesh.NumCell2Ds;i++){
        vector vertici = mesh.Cell2DsVertices[i];
        unsigned int numVert = mesh.Cell2DsNumEdges[i];
        double x0 = mesh.Cell0DsCoordinates(0,vertici[0]);
        double y0 = mesh.Cell0DsCoordinates(1,vertici[0]);
        double xn = mesh.Cell0DsCoordinates(0,vertici[numVert-1]);
        double yn = mesh.Cell0DsCoordinates(1,vertici[numVert-1]);
        double area = x0*yn - xn*y0;
        for(unsigned int j = 0; j < vertici.size()-1;j++)
        {
            double x1 = mesh.Cell0DsCoordinates(0,vertici[j]);
            double y1 = mesh.Cell0DsCoordinates(1,vertici[j]);
            double x2 = mesh.Cell0DsCoordinates(0,vertici[j+1]);
            double y2 = mesh.Cell0DsCoordinates(1,vertici[j+1]);
            area += x1*y2 - x2*y1;
        } ;
        area = abs(area) * 0.5;
        if(area < 1e-12){
            cout << "Errore, poligono di area nulla!" << endl;
            return 1;
        };
    }

    return 0;
}
}