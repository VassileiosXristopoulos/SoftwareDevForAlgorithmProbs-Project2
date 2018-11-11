//
// Created by vassilis on 10/11/2018.
//

#ifndef CLUSTER_CLUSTERMASTER_H
#define CLUSTER_CLUSTERMASTER_H

// Lloyd's

#include "DataSetMap.h"
#include "Cluster.h"
class ClusterMaster {
private:
    bool notFinished = true;
    DataSetMap* Dataset;
    vector<Cluster*> Clusters;


    void Initialization();
    void Assignement();
    void Update();
    void kmeanspp();
public:
    ClusterMaster(int , DataSetMap*);
    ~ClusterMaster();
    void Clustering();


};


#endif //CLUSTER_CLUSTERMASTER_H
