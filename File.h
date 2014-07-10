#ifndef FILE_H
#define FILE_H

#include "AList.h"
#include "HuffTree.h"

#include <fstream>
#include <iostream>
#include <QBitArray>

using namespace std;
typedef unsigned char BYTE;

class File{
private:
    AList *nodeList;

    QBitArray *bitArray;
    int baSize;
    int trash;
    ofstream huffFile;

    char* originalName;
    string treeRepre;
    int treeSize;
    int nameSize;

public:

    //*-----------*Compactacao*-----------*

    //le o arquivo e cria a lista de folhas
    bool readCompac(char* file){
        ifstream myFile;
        myFile.open(file, std::ios::in | std::ios::binary);

        if (myFile.is_open()){

            nodeList = new AList(256);

            myFile.seekg(0, ios::end);
            int size = myFile.tellg();
            myFile.seekg(0, ios::beg);

            while (size > 0) {
                BYTE x;
                x = myFile.get();

                HuffNode *node = new HuffNode(x, 1);

                // add node na lista
                if(nodeList->length() == 0){
                    nodeList->append(*node);
                }
                else{
                    for(int i = 0; i < nodeList->length(); i++){
                        nodeList->moveToPos(i);
                        HuffNode *nodeAux = new HuffNode;
                        *nodeAux = nodeList->getValue();
                        BYTE y = nodeAux->contet();

                        if(x == y){
                            nodeAux->freqPlus();
                            nodeList->remove();
                            nodeList->insert(*nodeAux);
                            i = nodeList->length();
                        }
                        else if(i == nodeList->length() -1){
                            nodeList->append(*node);
                            i = nodeList->length();
                        }
                        delete(nodeAux);
                    }
                }
                size--;
                delete(node);
            }

            return true;
        }
        else{
            cout << "Erro!" << endl;
            return false;
        }

        myFile.close();
    }

    // retorna a lista de folhas
    AList* getNodeList(){
        return nodeList;
    }


    //escreve a compactacao
    void writeCompac(AList *dictionary, char* file, char* fileName, char* huffName, string tree){
        bitArray = new QBitArray(8);
        baSize = 0;
        trash = 0;

        bitArray->fill(false);

        ifstream myFile;

        myFile.open(file, std::ios::in | std::ios::binary);

        if (myFile.is_open()){

            myFile.seekg(0, ios::end);
            int size = myFile.tellg();
            myFile.seekg(0, ios::beg);

            huffFile.open(huffName, std::ios::out | std::ios::binary);

            huffFile.seekp(tree.size() + strlen(fileName) + 2);

            while (size > 0) {
                BYTE x;
                x = myFile.get();

                HuffNode *aux = new HuffNode;

                for(int i = 0; i < dictionary->length(); i++){
                    dictionary->moveToPos(i);
                    *aux = dictionary->getValue();

                    if(x == aux->contet()){
                        for(int j = 0; j < aux->getBitArray().size(); j++){
                            bitArray->setBit(baSize, aux->getBitArray().at(j));
                            baSize++;

                            if(baSize == 8){
                                huffFile.put(baToChar());
                                bitArray->fill(false);
                                baSize = 0;
                            }
                        }
                    }
                }
                size--;
            }

            if(baSize != 0){
                huffFile.put(baToChar());
                trash = 8 - baSize;
                bitArray->fill(false);
                baSize = 0;
            }

            this->header(fileName,tree);

            huffFile.close();

            cout << "Comprimido!" << endl;
        }
        else{
            cout << "Erro!" << endl;
        }


        myFile.close();

    }


    // transforma bitarray para char
    BYTE baToChar(){
        BYTE y;
        for(int i = 0; i < 8; i++){
            y <<= 1;
            y += bitArray->at(i);
        }
        return y;
    }

    //cria e escreve o cabecalho
    void header(char* fileName, string tree){
        bitArray->fill(false);
        int treeSize = tree.size() - 1;

        treeSize = intToBa(treeSize, 7, 0);

        huffFile.seekp(1);

        huffFile.put(baToChar());
        bitArray->fill(false);

        if(treeSize > 0){
            treeSize = intToBa(treeSize, 7, 3);
        }

        trash = intToBa(trash, 2, 0);

        huffFile.seekp(0);
        huffFile.put(baToChar());
        bitArray->fill(false);

        int nameSize = strlen(fileName);

        nameSize = intToBa(nameSize, 7, 0);

        huffFile.seekp(2);
        huffFile.put(baToChar());
        bitArray->fill(false);

        huffFile.seekp(3);
        for(int i = 0; i < strlen(fileName); i++){
            huffFile.put(fileName[i]);
        }

        huffFile.seekp(3 + strlen(fileName));
        for(int i = 1; i < tree.size(); i++){
            huffFile.put(tree[i]);
        }
    }

    //transforma inteiro para bitarray
    int intToBa(int num, int from, int to){
        for(int i = from; i >= to; i--){
            bitArray->setBit(i,(num%2));
            num = num/2;
        }
        return num;
    }



    //*-----------*Descompactacao*-----------*

    //le arquivo huff
    bool readUncomp(char* file){
        ifstream myFile;
        myFile.open(file, std::ios::in | std::ios::binary);

        if(myFile.is_open()){

            bitArray = new QBitArray(16);

            int trashTreeSize;

            myFile.seekg(0);
            trashTreeSize = myFile.get();

            myFile.seekg(1);
            treeSize = myFile.get();

            myFile.seekg(2);
            nameSize = myFile.get();

            intToBa(treeSize, 15, 8);
            intToBa(trashTreeSize, 7, 0);

            trash = baToInt(2, 0);
            treeSize = baToInt(15, 3);

            originalName = new char[nameSize];
            for(int i = 3; i <= nameSize + 2; i++){
                myFile.seekg(i);
                originalName[i-3] = myFile.get();
            }

            for(int i = nameSize + 3; i <= treeSize + nameSize + 2; i++){
                myFile.seekg(i);
                treeRepre += myFile.get();
            }

            return true;
        }
        else{
            cout << "Erro" << endl;
            return false;
        }
    }

    string getTreeRepre(){
        return treeRepre;
    }

    //converte bitarray para inteiro
    int baToInt(int from, int to){
        int j = 1;
        int num = 0;
        for(int i = from; i >= to; i--){
            num = num + (j * (int)bitArray->at(i));
            j = j * 2;
        }

        return num;
    }

    //escreve o arquivo original
    void writeUncomp(char* file, char* destiny, HuffNode* root){
        ifstream myFile;
        myFile.open(file, std::ios::in | std::ios::binary);

        myFile.seekg(0, ios::end);
        int size = myFile.tellg();
        myFile.seekg(0, ios::beg);

        if(myFile.is_open()){
            bitArray = new QBitArray(8);

            ofstream uncompFile;
            if(strlen(destiny) == 0){
                uncompFile.open(originalName, std::ios::out | std::ios::binary);
            }else{
                strcat(destiny, originalName);
                uncompFile.open(destiny, std::ios::out | std::ios::binary);
            }

            int x;
            HuffNode *actualNode = root;

            for(int i = treeSize+nameSize+3; i < size; i++){
                myFile.seekg(i);
                x = myFile.get();

                intToBa(x, 7, 0);

                int k;
                if(i != size-1){
                    k = 7;
                }else{
                    k = 7 - trash;
                }

                for(int j = 0; j <= k; j++){
                    if(actualNode->isLeaf()){
                        uncompFile.put(actualNode->contet());
                        actualNode = root;
                    }
                    if(!actualNode->isLeaf()){
                        if(bitArray->at(j)){
                            actualNode = actualNode->getRight();
                        }else{
                            actualNode = actualNode->getLeft();
                        }
                    }
                }

                if(actualNode->isLeaf()){
                    uncompFile.put(actualNode->contet());
                    actualNode = root;
                }
            }

            uncompFile.close();
        }

        myFile.close();

        cout << "Descomprimido!" << endl;

    }

    char* getOriginalName(){
        return originalName;
    }

};

#endif // FILE_H

