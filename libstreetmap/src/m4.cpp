#include "m4.h"
#include "m3.h"
#include "m3A.h"
#include "globals.h"


#define NO_DELIVERY_NODE -1

//global variable


// This routine takes in a vector of N deliveries (pickUp, dropOff
// intersection pairs), another vector of M intersections that
// are legal start and end points for the path (depots), a turn 
// penalty in seconds (see m3.h for details on turn penalties), 
// and the truck_capacity in pounds.
//
// The first vector 'deliveries' gives the delivery information.  Each delivery
// in this vector has pickUp and dropOff intersection ids and the weight (also
// in pounds) of the delivery item. A delivery can only be dropped-off after
// the associated item has been picked-up. 
// 
// The second vector 'depots' gives the intersection ids of courier company
// depots containing trucks; you start at any one of these depots and end at
// any one of the depots.
//
// This routine returns a vector of CourierSubpath objects that form a delivery route.
// The CourierSubpath is as defined above. The first street segment id in the
// first subpath is connected to a depot intersection, and the last street
// segment id of the last subpath also connects to a depot intersection.  The
// route must traverse all the delivery intersections in an order that allows
// all deliveries to be made with the given truck capacity. Addionally, a package
// should not be dropped off if you haven't picked it up yet.
//
// The start_intersection of each subpath in the returned vector should be 
// at least one of the following (a pick-up and/or drop-off can only happen at 
// the start_intersection of a CourierSubpath object):
//      1- A start depot.
//      2- A pick-up location (and you must specify the indices of the picked 
//                              up orders in pickup_indices)
//      3- A drop-off location. 
//
// You can assume that N is always at least one, M is always at least one
// (i.e. both input vectors are non-empty), and truck_capacity is always greater
// or equal to zero.
//
// It is legal for the same intersection to appear multiple times in the pickUp
// or dropOff list (e.g. you might have two deliveries with a pickUp
// intersection id of #50). The same intersection can also appear as both a
// pickUp location and a dropOff location.
//        
// If you have two pickUps to make at an intersection, traversing the
// intersection once is sufficient to pick up both packages, as long as the
// truck_capcity fits both of them and you properly set your pickup_indices in
// your courierSubpath.  One traversal of an intersection is sufficient to
// drop off all the (already picked up) packages that need to be dropped off at
// that intersection.
//
// Depots will never appear as pickUp or dropOff locations for deliveries.
//  
// If no valid route to make *all* the deliveries exists, this routine must
// return an empty (size == 0) vector.
/*
std::vector<CourierSubpath> traveling_courier( const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, const float turn_penalty, const float truck_capacity){
    //hold a vector of nodes that need to be reached
    
    std::vector <std::pair<int, std::string>> pickUpDropOffLocations;
    for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
        std::pair<int, std::string> pup ((*itDeliveries).pickUp, "pickup");
        std::pair<int, std::string> doff ((*itDeliveries).dropOff, "dropoff");
        pickUpDropOffLocations.push_back(pup);
        pickUpDropOffLocations.push_back(doff);
    }
    
   //keep track of status of path. Path is invalid if: 1) driving path does not exist or 2) an item weight exceeds truck capacity
    bool invalidPath = false; 
    //Full path from starting Depot to end Depot
    std::vector<CourierSubpath> fullPath;
    //Incremental path, from a specific intersection to another intersection
    std::vector<int> drivingPath; 
    //An invalid ID, indicating the previous Intersection was not a delivery pick up or drop off
    int prevIntersectID = NO_DELIVERY_NODE; 
    //stores indices from delivery vector of deliveries picked up at start_intersection in CourierSubpath
    std::vector<unsigned> pickUpIndices; 
    struct CourierSubpath deliverySubpath; 
    
    //store weight currently carried by truck
    float totalWeight = 0;
    
    //Chosen Depots
    //represents the IDs from the depots vector. Can assume that at least 1 depot exists, thus index 0 exists
    int startDepot = depots[0], endDepot = depots[0]; 
    int deliveryIndice = 0;
    int previousWeight = 0;
    int intersectionFound;
    
    bool packagesLeft = true;
    
    while(packagesLeft){ 
        //check if nodes to be found is empty, if it is, break
        if (pickUpDropOffLocations.empty()){
            packagesLeft = false;
            break;
        }
        //check if weight of item exceeds the truck capacity
        if (totalWeight > truck_capacity){ 
            //path is deemed invalid (according to Piazza)
            invalidPath = true; 
            break;
        }
        //Get the directions TO the pick up for the delivery
        //If we are dealing with the first pick up (the intersection before must be a depot)
        if (prevIntersectID == NO_DELIVERY_NODE){ 
            //Find directions from starting Depot to the first pickup
            drivingPath = find_path_djikstra(startDepot, pickUpDropOffLocations , turn_penalty);
            //if path does not exist
            if (drivingPath.empty()){ 
                invalidPath = true;
                break;
             }
            //This is the first subpath from starting intersection to first delivery. Pick up indices should have the starting indice.
            //not pickung up anything at starting thereofre empty pickUpIndices
            
            //get intersection id that was reached in djikstra
            intersectionFound = intersectionsReached.back().first;
            deliverySubpath = { startDepot, intersectionFound, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
            
        }
            
        //We are dealing with a pickup or drop offthat is NOT the first (previous intersection visited was the drop-off of another delivery)
        else{
            //Find directions from previous drop off intersection to a new pick
            drivingPath = find_path_djikstra(prevIntersectID, pickUpDropOffLocations, turn_penalty); 
            if (drivingPath.empty()){
                invalidPath = true;
                break;
             }
            //get intersection id that was reached in djikstra
            intersectionFound = intersectionsReached.back().first;
            //check if intersection reached was a drop off or pick up intersection
            std::string pUdO = intersectionsReached.back().second;
            if (pUdO == "dropoff"){
                pickUpIndices.clear();
            }
            else{
                //deliveryIndice?????
                pickUpIndices.push_back(deliveryIndice); 
            }
            
            deliverySubpath = {prevIntersectID, intersectionFound, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
        }
        
        
            //remove that intersection from pickUpDropOffLocations
            for (std::vector<std::pair<int, std::string>>::iterator itLocations = pickUpDropOffLocations.begin(); itLocations != pickUpDropOffLocations.end(); itLocations++){
                if (itLocations->first == intersectionFound){
                    pickUpDropOffLocations.erase(itLocations);
                    break;
                }
            }
    }
    
    if (invalidPath)
        //return empty vector
        return std::vector<CourierSubpath>(); 
    
    //Add the last part of the directions: the path from the last drop-off location to the end Depot
    //Find directions directly from the pickup to drop-off of the same delivery
    drivingPath = find_path_between_intersections(prevIntersectID, endDepot, turn_penalty);
    if (drivingPath.empty()){
        //return empty vector
       return std::vector<CourierSubpath>(); 
    }
     

    //add new path section to the full Path
    deliverySubpath = {prevIntersectID, endDepot, drivingPath, pickUpIndices}; 
    //At this point, pickUpIndices is empty vector
    //for safety
    pickUpIndices.clear();
    fullPath.push_back(deliverySubpath);
    
    return fullPath;
    
}
 */

std::vector<CourierSubpath> traveling_courier( const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, const float turn_penalty, const float truck_capacity){
    //hold a vector of nodes that need to be reaches
    
    std::vector <int> pickUpDropOffLocations;
    for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
        pickUpDropOffLocations.push_back((*itDeliveries).pickUp);
        pickUpDropOffLocations.push_back((*itDeliveries).dropOff);
    }
    
   //keep track of status of path. Path is invalid if: 1) driving path does not exist or 2) an item weight exceeds truck capacity
    bool invalidPath = false; 
    //Full path from starting Depot to end Depot
    std::vector<CourierSubpath> fullPath;
    //Incremental path, from a specific intersection to another intersection
    std::vector<int> drivingPath; 
    //An invalid ID, indicating the previous Intersection was not a delivery pick up or drop off
    int prevIntersectID = NO_DELIVERY_NODE; 
    //stores indices from delivery vector of deliveries picked up at start_intersection in CourierSubpath
    std::vector<unsigned> pickUpIndices; 
    struct CourierSubpath deliverySubpath; 
    
    //store weight currently carried by truck
    float totalWeight = 0;
    
    //Chosen Depots
    //represents the IDs from the depots vector. Can assume that at least 1 depot exists, thus index 0 exists
    int startDepot = depots[0], endDepot = depots[0]; 
    int deliveryIndice = 0;
    int previousWeight = 0;
    
    std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin();
    //for each delivery item
    while(itDeliveries != deliveries.end()){ 
        //check if weight of item exceeds the truck capacity
        if (totalWeight > truck_capacity){ 
            //path is deemed invalid (according to Piazza)
            invalidPath = true; 
            break;
        }
        //Get the directions TO the pick up for the delivery
        //If we are dealing with the first pick up (the intersection before must be a depot)
        if (prevIntersectID == NO_DELIVERY_NODE){ 
            //Find directions from starting Depot to the first pickup
            drivingPath = find_path_between_intersections(startDepot, itDeliveries -> pickUp, turn_penalty); 
            //if path does not exist
            if (drivingPath.empty()){ 
                invalidPath = true;
                break;
             }
            //This is the first subpath from starting intersection to first delivery. Pick up indices should have the starting indice.
            //not pickung up anything at starting thereofre empty pickUpIndices
            pickUpIndices.clear();
            deliverySubpath = { startDepot, itDeliveries->pickUp, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
        }
        //We are dealing with a pickup that is NOT the first (previous intersection visited was the drop-off of another delivery)
        else{
            //Find directions from previous drop off intersection to a new pick
            totalWeight = totalWeight - itDeliveries->itemWeight;
            drivingPath = find_path_between_intersections(prevIntersectID, itDeliveries->pickUp, turn_penalty); 
            if (drivingPath.empty()){
                invalidPath = true;
                break;
             }
            //At this point, pickUpIndices is empty vector
            pickUpIndices.clear();
            deliverySubpath = {prevIntersectID, itDeliveries->pickUp, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
        }
        
        //Get directions FROM the pickup for the delivery TO its drop-off location
        //Find directions directly from the pickup to drop-off of the same delivery
        drivingPath = find_path_between_intersections(itDeliveries->pickUp, itDeliveries->dropOff, turn_penalty);
        if (drivingPath.empty()){
           invalidPath = true;
           break;
        }
        //add weight to truck
        totalWeight = totalWeight + itDeliveries->itemWeight;
        //In this  sub path, a pick-up occurred at the starting intersection of sub path, so add this to the vector 
        //add new path section to the full Path
        pickUpIndices.push_back(deliveryIndice); 
        //At this point, pickUpIndices is empty vector
        deliverySubpath = {itDeliveries->pickUp, itDeliveries->dropOff, drivingPath, pickUpIndices}; 
        fullPath.push_back(deliverySubpath);
        //save the value of the most recent Intersection visited
        prevIntersectID = itDeliveries->dropOff; 
        pickUpIndices.clear();
        itDeliveries++;  
        deliveryIndice++;
    }
    
    if (invalidPath)
        //return empty vector
        return std::vector<CourierSubpath>(); 
    
    //Add the last part of the directions: the path from the last drop-off location to the end Depot
    //Find directions directly from the pickup to drop-off of the same delivery
    drivingPath = find_path_between_intersections(prevIntersectID, endDepot, turn_penalty);
    if (drivingPath.empty()){
        //return empty vector
       return std::vector<CourierSubpath>(); 
    }
     

    //add new path section to the full Path
    deliverySubpath = {prevIntersectID, endDepot, drivingPath, pickUpIndices}; 
    //At this point, pickUpIndices is empty vector
    //for safety
    pickUpIndices.clear();
    fullPath.push_back(deliverySubpath);
    
    return fullPath;
    
}