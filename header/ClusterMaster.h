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
    bool canRepeat = false;
    DataSetMap* Dataset;
    vector<Cluster*> Clusters;
    vector<int>Choises;

    void Initialization();
    void RandomSelection();
    void kmeanspp();
    void Assignement();
    void LloydsAssignment();
    void Update();
    void SetNextChoise();
    void ResetDataset();
public:
    ClusterMaster(int , DataSetMap*, int[]);
    ~ClusterMaster();
    void Clustering();



};


#endif //CLUSTER_CLUSTERMASTER_H
