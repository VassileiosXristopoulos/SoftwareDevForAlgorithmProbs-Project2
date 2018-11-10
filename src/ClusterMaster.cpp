//
// Created by vassilis on 10/11/2018.
//

#include "../header/ClusterMaster.h"
#include "../header/Util.h"
#include <ctime>

/**
 * Constructor
 */
ClusterMaster::ClusterMaster(int k, DataSetMap* set) {
    this->Clusters = vector<Cluster*>(k);
    for(int i=0;i<k;i++){
        Clusters[i] = new Cluster();
    }
    this->Dataset = set;
    this->Initialization();
}



/**
 * Destructor
 */
ClusterMaster::~ClusterMaster() {

}



/**
 * Initialize centers
 * Random selection (simple)
 */
void ClusterMaster::Initialization() {
    srand(time(0));
    for(unsigned int i=0; i<Clusters.size(); i++){
        // select a random Item from Map for each Cluster's centroid
        Item * item = Dataset->at(rand() % Dataset->size());
        item->SetCluster(i);
        Clusters[i]->SetCentroid(item);
    }
}



/**
 * Assign each point to new Cluster
 * Lloyd's assignement
 */
void ClusterMaster::Assignement() {

    bool noChanges = true;
    for(int i=0; i<Dataset->size(); i++){ // for each item

        Item * dataSetItem = Dataset->at(i);

        /*--------- compute the distance from the closest cluster ------------*/

        /* set min as the distance of the item with the "first" centroid */
        int min = (int)Util::EucledianDistance(dataSetItem->getContent(),Clusters[0]->GetCentroid()->getContent());
        int closerCluster = 0; //set Cluster 0 as the closest

        /* find minimum distance */

        for(int j = 1; j<Clusters.size(); j++ ){
            int dist = (int)Util::EucledianDistance(dataSetItem->getContent(),Clusters[j]->GetCentroid()->getContent());
            if(dist<min){
                min = dist;
                closerCluster = j;
            }
        }
        /*--------------------------------------------------------------------*/


        // closest cluster is "closerCluster"


        // if the item is not already in it's closest cluster
        if(!Clusters[closerCluster]->Contains(dataSetItem->getName())){
            if(dataSetItem->GetCluster() > 0 ){ // if item is in some cluster (i.e. we are not in 1st step)
                // delete the item from it's previous cluster
                Clusters[dataSetItem->GetCluster()]->DeleteMember(dataSetItem->getName());
            }
            dataSetItem->SetCluster(closerCluster); // set the item's cluster to be the new cluster
            Clusters[closerCluster]->InsertMember(dataSetItem); //insert the item to it's new cluster
            noChanges = false; // save that changes were made (for ending condition)
        }

    }
    notFinished = !noChanges; // if changes where made, then not finished
}




/**
 * Perform clustering
 */
void ClusterMaster::Clustering() {
    while( notFinished ){
        this->Assignement();
        this->Update();
    }
    cout << "Clustering finished!" << endl;
}



/**
 * Update centers of each Cluster
 */
void ClusterMaster::Update() {
    for(auto const& cluster_i : Clusters){
        cluster_i->Update();
    }

}
