#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
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
        }
    }
    
    return true;
}

}