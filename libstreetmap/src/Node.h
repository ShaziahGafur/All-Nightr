
#ifndef NODE_H
#define NODE_H

#include <vector>
class Node{
    
public:
    int ID; //equal to the intersection ID
    std::vector<int> outEdgeIDs;
    int reachingEdge;
    double bestTime;
    bool crawlEnable;
    Node(int intersectionID, int reachingEdgeID, double shortestTime);
    Node();
    
};

#endif /* NODE_H */


