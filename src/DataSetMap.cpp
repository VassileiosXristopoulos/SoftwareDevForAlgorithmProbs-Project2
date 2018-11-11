//
// Created by vassilis on 13/10/2018.
//

#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "../header/DataSetMap.h"
#include "../header/Util.h"

int d,n;
extern int k;

DataSetMap::~DataSetMap() {
    for(unsigned int i=0; i<Map.size(); i++){
        delete(Map[i]);
    }
}


void DataSetMap::append(Item * item) {
    Map.push_back(item);
}



double DataSetMap::TrueDistance(Item *item,string mode) {
    double min=-1;
    for (auto &i : Map) {
        double dist;
        if(mode == "eucledian"){
            dist= Util::EucledianDistance(item->getContent(), i->getContent());
        }
        else if(mode == "cosine"){
            dist=Util::cosineDistance(item->getContent(), i->getContent());
        }
        if(min==-1 || dist<min)
            min = dist;
    }
    return min;
}

int DataSetMap::size(){
    return static_cast<int>(Map.size());
}

Item *DataSetMap::at(int index) {
    return Map[index];
}

void DataSetMap::InsertFile(string inputFile) {
    string FileLine;
    ifstream file(inputFile);

    //count lines of file -- if k = default then k = log2(n) -- for hypercube
    n = (int)std::count(std::istreambuf_iterator<char>(file),
               std::istreambuf_iterator<char>(), '\n') - 1;
    if(k==-1) k = (int)log2(n);
    file.close();

    ifstream input(inputFile);

    for( std::string line; getline( input, FileLine ); ) {

        line = FileLine.substr(0, FileLine.size() - 1);
        vector<string> element = Util::Split(line);
        Item *item = new Item(element);

        if(element.size()<=0){
            cout <<"element empty!!"<<endl;
            continue;
        }
        Map.push_back(item);

        d = static_cast<int>(element.size());

    }

}

DataSetMap::DataSetMap(DataSetMap & Map) {
    for(int i=0;i<Map.size(); i++){
        this->Map.push_back(Map.at(i));
    }
}

void DataSetMap::erase(Item *item) {
    int pos = (int)(find(Map.begin(), Map.end(), item) - Map.begin());
    if(pos<Map.size()){
        Map.erase(Map.begin()+pos);
    }
}






