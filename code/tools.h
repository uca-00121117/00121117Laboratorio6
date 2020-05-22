#include <fstream>
#include "string.h"

void obtenerDatos(istream &file,int nlines,int n,int mode,item* item_list){
    string line;
    file >> line;
    if(nlines==DOUBLELINE) file >> line;

    for(int i=0;i<n;i++){
        switch(mode){
        case INT_FLOAT:
            int e0; float r0;
            file >> e0 >> r0;
            item_list[i].setValues(0,0,0,e0,0,0,r0);
            break;
        case INT_FLOAT_FLOAT:
            int e; float r,rr;
            file >> e >> r >> rr;
            item_list[i].setValues(e,r,rr,0,0,0,0);
            break;
        case INT_INT_INT_INT:
            int e1,e2,e3,e4;
            file >> e1 >> e2 >> e3 >> e4;
            item_list[i].setValues(e1,0,0,e2,e3,e4,0);
            break;
        }
    }
}

void correctConditions(int n,condition *list,int *indices){
    for(int i=0;i<n;i++)
        indices[i] = list[i].getNode1();

    for(int i=0;i<n-1;i++){
        int pivot = list[i].getNode1();
        for(int j=i;j<n;j++)
            if(list[j].getNode1()>pivot)
                list[j].setNode1(list[j].getNode1()-1);
    }
}
    
void addExtension(char *newfilename,char *filename, string extension){
    int ori_length = strlen(filename);
    int ext_length = extension.length();
    int i;
    for(i=0;i<ori_length;i++)
        newfilename[i] = filename[i];
    for(i=0;i<ext_length;i++)
        newfilename[ori_length+i] = extension[i];
    newfilename[ori_length+i] = '\0';
}

void leerMallayCondiciones(mesh &m,char *filename){
    char inputfilename[150];
    ifstream file;
    float A,E,f;
    int nnodes,neltos,ndirich,nneu;

    addExtension(inputfilename,filename,".dat");
    file.open(inputfilename);

    file >> A >> E >> f;

    file >> nnodes >> neltos >> ndirich >> nneu;

    m.setParameters(A,E,f);
    m.setSizes(nnodes,neltos,ndirich,nneu);
    m.createData();

    obtenerDatos(file,SINGLELINE,nnodes,INT_FLOAT_FLOAT,m.getNodes());
    obtenerDatos(file,DOUBLELINE,neltos,INT_INT_INT_INT,m.getElements());
    obtenerDatos(file,DOUBLELINE,ndirich,INT_FLOAT,m.getDirichlet());
    obtenerDatos(file,DOUBLELINE,nneu,INT_FLOAT,m.getNeumann());

    file.close();

    correctConditions(ndirich,m.getDirichlet(),m.getDirichletIndices());
}

bool findIndex(int v, int s, int *arr){
    for(int i=0;i<s;i++)
        if(arr[i]==v) return true;
    return false;
}

void writeResults(mesh m,Vector T,char *filename){
    char outputfilename[150];
    int *dirich_indices = m.getDirichletIndices();
    condition *dirich = m.getDirichlet();
    ofstream file;

    addExtension(outputfilename,filename,".post.res");
    file.open(outputfilename);

    file << "GiD Post Results File 1.0\n";
    file << "Result \"Temperature\" \"Load Case 1\" 1 Scalar OnNodes\nComponentNames \"u\"\nValues\n";

    int Tpos = 0;
    int Dpos = 0;
    int n = m.getSize(NODES);
    for(int i=0;i<n;i++){
        if(findIndex(i+1,n,dirich_indices)){
            file << i+1 << " " << dirich[Dpos].getValue() << "\n";
            Dpos++;
        }else{
            file << i+1 << " " << T.at(Tpos) << "\n";
            Tpos++;
        }
    }

    file << "End values\n";

    file.close();
}
