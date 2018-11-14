//
// Created by vassilis on 10/11/2018.
//

#ifndef CLUSTER_CLUSTERMASTER_H
#define CLUSTER_CLUSTERMASTER_H

// Lloyd's

#include "DataSetMap.h"
#include "Cluster.h"
#include "lsh/AHashTable.h"
#include "hypercube/AHypercube.h"
#include "Util.h"
#include "lsh/lsh.h"

class ClusterMaster {
private:
    bool notFinished = true;
    bool canRepeat = false;
    DataSetMap* Dataset;
    vector<Cluster*> Clusters;
    vector<int>Choises;
    lsh * lsh_master;
    AHypercube * Hypercube;

    void Initialization();
    void RandomSelection();
    void kmeanspp();
    void Assignement();
    void LloydsAssignment();
    void LSHAssignment();
    void Update();
    void SetNextChoise();
    void ResetDataset();
public:
    ClusterMaster(Config_info, DataSetMap*, int[],string&);
    ~ClusterMaster();
    void Clustering();



};


#endif //CLUSTER_CLUSTERMASTER_H
