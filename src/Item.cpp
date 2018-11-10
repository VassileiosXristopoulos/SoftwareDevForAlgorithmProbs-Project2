//
// Created by vassilis on 13/10/2018.
//

#include <cstdlib>
#include "../header/Item.h"
extern int k;
Item::Item(vector<string> &element){

    name = element.at(0);
    element.erase(element.begin());

    for(unsigned int i=0;i<element.size();i++){
        content.push_back(atoi(element[i].c_str()));
    }
}


vector<int> &Item::getContent() {
    return content;
}
string Item::getName(){
    return name;
}
void Item::setGVector(vector<int>& h_i) {
    for(unsigned int i=0;i<g_vector.size(); i++){
        g_vector[i] = h_i[i];
    }
}

Item::~Item() {}



vector<int>& Item::getGVector() {
    return g_vector;
}


int Item::size() {
    int size=(int)name.size();
    for(unsigned int i=0;i<content.size(); i++){
        size+=sizeof(content[i]);
    }
    for(unsigned int i=0; i<g_vector.size(); i++){
        size+=sizeof(g_vector[i]);
    }
    return size;
}






