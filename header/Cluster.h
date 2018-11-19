//
// Created by vassilis on 10/11/2018.
//

#ifndef CLUSTER_CLUSTER_H
#define CLUSTER_CLUSTER_H


#include <map>
#include "Item.h"

class Cluster {
private:
    Item * Centroid;
    map<string,Item*>Members;
    double ShilouetteValue;
    bool PAM();
    bool kmeans();
public:
    Cluster() = default;
    ~Cluster();
    void SetCentroid(Item*);
    Item* GetCentroid();
    bool Contains(string item);
    void DeleteMember(string);
    void InsertMember(Item*);
    bool ReplaceElements(vector<Item*>);
    map<string,Item*> GetMembers();
    bool Update(int);
    void FlushClusterMembers();
    void ComputeSilhouette();
    double GetSilhouette();
    int size();
};


#endif //CLUSTER_CLUSTER_H
