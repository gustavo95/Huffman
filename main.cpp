#include "HuffTree.h"
#include "File.h"
#include "AList.h"

#include <iostream>
#include <QDebug>
#include <QString>

using namespace std;

//pega o nome do arquivo do local especificado;
char* getFileName(char* file){
    int aux = 0;
    string fileN;

    for(int i = strlen(file)-1; i >= 0; i--){
        if(file[i] == *"\\"){
            aux = i+1;
            i = -1;
        }
    }

    if(aux == 0){
        fileN = file;
    }
    else{
        for(int i = aux; i < strlen(file); i++){
            fileN += file[i];
        }
    }

    char* charAux = new char[fileN.size()+1];
    strcpy(charAux, fileN.c_str());
    strcat(charAux,"\0");

    return charAux;
}

//verifica se o nome do arquivo e .huuf
bool isHuff(char* name){
    if(name[strlen(name)-1] == *"f" && name[strlen(name)-2] == *"f" && name[strlen(name)-3] == *"u" &&name[strlen(name)-4] == *"h" && name[strlen(name)-5] == *"."){
        return true;
    }
    else{
        return false;
    }
}

// nome do arquivo para .huff
char* toHuff(char* file){
    char* name = getFileName(file);
    string huffName;

    if(!isHuff(name)){
        int aux = 0;

        for(int i = strlen(name)-1; i >= 0; i--){
            if(name[i] == *"."){
                aux = i;
                i = -1;
            }
        }

        for(int i = 0; i < aux; i++){
            huffName += name[i];
        }
        huffName += ".huff";

    }else{
        huffName = name;
    }

    char* aux = new char[huffName.size()+1];
    strcpy(aux, huffName.c_str());
    strcat(aux,"\0");

    return aux;
}

// compacta o arquivo
void HuffCompac(char* fileToCompac, char* huffName){
    char* fileName = getFileName(fileToCompac);
    File file;

    if(file.readCompac(fileToCompac)){
        HuffTree *tree = new HuffTree;
        tree->buildHuffTree(file.getNodeList());
        tree->treeRepresentation(tree->getRoot());
        file.writeCompac(tree->getDictionary(), fileToCompac, fileName, huffName, tree->getTree());
    }
}

//descompactar arquivo
void HuffUncomp(char* fileToUncomp, string destiny){
    File file;
    char* aux = new char[destiny.size()+1];

    if(file.readUncomp(fileToUncomp)){
        HuffTree *tree = new HuffTree;
        tree->rebuildTree(file.getTreeRepre());
        strcpy(aux, destiny.c_str());
        strcat(aux,"\0");
        file.writeUncomp(fileToUncomp, aux, tree->getRoot());
    }
}

int main(int argc, char *argv[])
{

    if(argc == 5 && strcmp(argv[1],"-c") == 0 && strcmp(argv[3],"-o") == 0 && isHuff(argv[4])){
        HuffCompac(argv[2], argv[4]);
    }
    else if(argc == 3 && strcmp(argv[1],"-c") == 0){
        HuffCompac(argv[2], toHuff(argv[2]));

    }
    else if(argc == 2 && isHuff(argv[1])){
        HuffUncomp(argv[1], "");
    }
    else if(argc == 4 && isHuff(argv[1]) && strcmp(argv[2],"-d") == 0){
        string aux = argv[3];
        aux += "\\";
        HuffUncomp(argv[1], aux);
    }
    else{
        cout << endl;
        cout << "huffman -c arquivo_origem.x -o     Comprime o arquivo arquivo_origem.x e gera" << endl;
        cout << "arquivo_de_saida.huff              o arquivo de saida  arquivo_de_saida.huff" << endl << endl;

        cout << "huffman -c arquivo.x               Comprime o arquivo arquivo.x e gera o" << endl;
        cout << "                                   arquivo de saida  arquivo.huff, mantendo" << endl;
        cout << "                                   o nome do arquivo original" << endl << endl;

        cout << "huffman arquivo.huff               Descomprime o arquivo arquivo.huff na pasta" << endl;
        cout << "                                   local com o nome original" << endl << endl;

        cout << "huffman arquivo.huff -d            Descomprime o arquivo arquivo.huff na pasta " << endl;
        cout << "/home/user/destino                 /home/user/destino com o nome original" << endl << endl;
    }

    return 0;
}

