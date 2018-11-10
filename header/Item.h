//
// Created by vassilis on 13/10/2018.
//

#ifndef TEST_ITEM_H
#define TEST_ITEM_H
#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
using namespace std;
extern int k;
class Item {
private:
    string name;
    vector<int>content;
    vector<int>g_vector=vector<int>(k);
    int cluster = -1;
public:
    Item(string);
    Item(vector<string>&element);
    ~Item();
    vector<int>& getContent();
    void SetContent(vector<int>&);
    string getName();
    void setGVector(vector<int>& h_i);
    vector<int>& getGVector();
    int GetPoint(int);
    int size();
    void SetCluster(int);
    int GetCluster();
};


#endif //TEST_ITEM_H
