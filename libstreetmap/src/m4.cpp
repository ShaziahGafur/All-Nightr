#include "m4.h"

#define NO_DELIVERY_NODE -1

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
std::vector<CourierSubpath> traveling_courier(
		            const std::vector<DeliveryInfo>& deliveries,
	       	        const std::vector<int>& depots, 
		            const float turn_penalty, 
		            const float truck_capacity){
    //Stub implementation
    
    //Set-up
    bool invalidPath = false; //keep track of status of path. Path is invalid if: 1) driving path does not exist or 2) an item weight exceeds truck capacity
    std::vector<CourierSubpath> fullPath; //Full path from starting Depot to end Depot
    std::vector<StreetSegmentIndex> drivingPath; //Incremental path, from a specific intersection to another intersection
    int prevIntersectID = -1; 
    std::vector<int> pickUpIndices;
    struct CourierSubpath deliveryPath;
    
    //Chosen Depots
    int startDepot = 0, endDepot = 0; //represents the IDs from the depots vector. Can assume that at least 1 depot exists, thus index 0 exists
    
    std::vector<DeliveryInfo>::const_iterator it = deliveries.begin();
    while(/*!invalidPath && */it != deliveries.end()){
        if (it->itemWeight > truck_capacity){ //check if weight of item exceeds the truck capacity
            invalidPath = true;
            break;
        }
        //Get the directions TO the pick up for the delivery
        if (prevIntersectID == NO_DELIVERY_NODE){ //If we are dealing with the first pick up (the intersection before must be a depot)
            drivingPath = find_path_between_intersections(startDepot, it->pickUp, turn_penalty); //Find directions from starting Depot to the first pickup
            if (drivingPath.empty()){
                invalidPath = true;
                break;
             }
             //add new path section to the full Path
            deliveryPath = {startDepot, it->pickUp, drivingPath, pickUpIndices}; //At this point, pickUpIndices is empty vector
            fullPath.push_back(deliveryPath);
        }
        else{//We are dealing with a pickup that is NOT the first (previous intersection visited was the drop-off of another delivery)
            drivingPath = find_path_between_intersections(prevIntersectID, it->pickUp, turn_penalty); //Find directions from starting Depot to the first pickup
            if (drivingPath.empty()){
                invalidPath = true;
                break;
             }
            deliveryPath = {prevIntersectID, it->pickUp, drivingPath, pickUpIndices}; //At this point, pickUpIndices is empty vector
            fullPath.push_back(deliveryPath);
        }
        
        //Get directions FROM the pickup for the delivery TO its drop-off location
         drivingPath = find_path_between_intersections(it->pickUp, it->dropOff, turn_penalty);//Find directions directly from the pickup to drop-off of the same delivery
         if (drivingPath.empty()){
            invalidPath = true;
            break;
         }
         pickUpIndices.push_back(it->pickUp); //In this  sub path, a pick-up occurred at the starting intersection, so add this to the vector 
         //add new path section to the full Path
         deliveryPath = {it->pickUp, it->dropOff, drivingPath, pickUpIndices}; //At this point, pickUpIndices is empty vector
         fullPath.push_back(deliveryPath);
         prevIntersectID = it->dropOff; //save the value of the most recent Intersection visited
         pickUpIndices.clear();
        it++;   
    }
    
    if (invalidPath)
        return std::vector<CourierSubpath>(); //return empty vector
    
    //Add the last part of the directions: the path from the last drop-off location to the end Depot
         drivingPath = find_path_between_intersections(prevIntersectID, endDepot, turn_penalty);//Find directions directly from the pickup to drop-off of the same delivery
         if (drivingPath.empty()){
            return std::vector<CourierSubpath>(); //return empty vector
         }
         
        
         
                  pickUpIndices.clear(); //for safety

         
         //add new path section to the full Path
         deliveryPath = {prevIntersectID, endDepot, drivingPath, pickUpIndices}; //At this point, pickUpIndices is empty vector
         fullPath.push_back(deliveryPath);
    
    return fullPath;
    
}


