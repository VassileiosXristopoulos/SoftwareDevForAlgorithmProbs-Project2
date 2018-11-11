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
   /* srand(time(0));
    for(unsigned int i=0; i<Clusters.size(); i++){
        // select a random Item from Map for each Cluster's centroid
        Item * item = Dataset->at(rand() % Dataset->size());
        item->SetCluster(i);
        Clusters[i]->SetCentroid(item);
    }*/
   kmeanspp();
}



/**
 * Initialize centers
 * K-Means++
 */
void ClusterMaster::kmeanspp() {
    int initialCentroid = rand() % Dataset->size();
    Clusters[0]->SetCentroid(Dataset->at(initialCentroid)); // pick uniformly first centroid
    DataSetMap* nonCentroidMap = new DataSetMap(*Dataset); // get copy of Dataset, keep only the non centroids
    nonCentroidMap->erase(Dataset->at(initialCentroid)); //erase from map, keep only the non centroids

    double maxDi = 0;
    vector<double>Di; // minimum distance from centroids
    // the idea is that we compute every D(i) once and for all and when we set a centroid we erase the element
    for(unsigned int i=0;i<nonCentroidMap->size();i++){ // for each non centroid (compute minimum distace)

        //initially, it is the distance from the "first" centroid
        double minDist = Util::EucledianDistance(nonCentroidMap->at(i)->getContent(),Clusters[0]->GetCentroid()
        ->getContent());

        for(unsigned int j = 1; j < Clusters.size(); j++){ // search minimum
            if(Clusters[j]->GetCentroid() != nullptr){
                double dist = Util::EucledianDistance(nonCentroidMap->at(i)->getContent(),Clusters[j]->GetCentroid()
                        ->getContent());
                if(dist < minDist){
                    minDist = dist;
                }
            }
        }
        // here, minimum is found and is minDist
        Di.push_back(minDist); // Di[i] has the D(i) of non centroid i
        if(minDist > maxDi){
            maxDi = minDist;
        }
    }

    for(unsigned int i=0; i<Di.size(); i++){ // normalize D(i)'s
        Di[i] /= maxDi;
    }

    // perform the choosing of centroids
    for(int i=1;i<Clusters.size(); i++){ // until all the clusters have centroids
        vector<double>P((unsigned long)(nonCentroidMap->size() + 1),0); // P[0] = 0

        for(int j = 1; j < P.size(); j++){ // fill P(r)
            for(int k = 0; k < j ; k++){ // sum of D(i) from start until r
                P[j] += Di[k];
            }
        }
        // now we have P(r) array
        float rand_x = (float) (rand()) / ((float) (RAND_MAX / P[P.size()-1]) ); // pick random x ~ [0,P(n-t)]
        int newCentroid = -1;
        for(unsigned int j=1;j<P.size(); j++){
            if(rand_x > P[j-1] && rand_x <= P[j]){
                newCentroid = j-1; // get the position (in Di or in nonCentroidMap) of the new selected centroid
            }
        }

        if(newCentroid == -1){
            cout << "Error occured while performing kmeans++" <<endl;
            exit(0);
        }

        Clusters[i]->SetCentroid(nonCentroidMap->at(newCentroid));

        // now keep only the non centroid in Di and nonCentroidMap - that way the indexing will be consistent
        nonCentroidMap->erase(nonCentroidMap->at(newCentroid));
        Di.erase(Di.begin()+newCentroid);
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


