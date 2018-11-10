//
// Created by vassilis on 13/10/2018.
//

#ifndef TEST_DATASETMAP_H
#define TEST_DATASETMAP_H

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include "Item.h"

using namespace std;
class DataSetMap {
private:
    vector<Item*>Map;
public:
    DataSetMap();
    ~DataSetMap();
    void append(Item *item);
    double TrueDistance(Item*item,string mode);
    int size();
    Item* at(int index);
    string InsertFile(string file);
};


#endif
