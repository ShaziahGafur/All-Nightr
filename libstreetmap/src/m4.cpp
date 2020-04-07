#include "m4.h"
#include "m3.h"


#define NO_DELIVERY -1

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
//otherwise returne a vector of CourierSubpath objects
std::vector<CourierSubpath> traveling_courier( const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, const float turn_penalty, const float truck_capacity){
    //Stub implementation
    
   //keep track of status of path. Path is invalid if: 1) driving path does not exist or 2) an item weight exceeds truck capacity
    bool pathValid = true; 
    
    //Full path from star Depot to end Depot
    std::vector<CourierSubpath> fullCourierPath;
    
    //struct containing 1) start/Depot intersection,2) end/Depot intersection,3) inner-path, 4) pick-up indices (if any)
    CourierSubpath subCourierPath; 
    //path IN BETWEEN starting and ending intersections of a CourierSubpath
    std::vector<int> subpath; 
    //stores indices of the deliveries vector of the picked up item (if any) at the START of a subCourierPath
    std::vector<unsigned> pickUpIndices; 
    
    
    //store weight currently carried by truck
    float totalWeight = 0;
    
    //Chosen Depots
    //represents the IDs from the depots vector. Can assume that at least 1 depot exists, thus index 0 exists
    int startDepot = depots[0], endDepot = depots[0]; 

    int startId = NO_DELIVERY;
    //used to populate pickUpIndeces vector
    int deliveriesIdx = 0;
    int previousWeight = 0;

    
    std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin();
    
    //check if any deliveries are over the weight limit
    //if so must return an empty vector
    while(itDeliveries != deliveries.end()){ 

        //check if weight of any item exceeds the truck capacity
        if (itDeliveries-> itemWeight > truck_capacity) 
        itDeliveries++;
    }
    
    itDeliveries = deliveries.begin();  
    while( itDeliveries != deliveries.end() ){ 
        
        //For first pick up, the start intersection must be a depot
        if (startId == NO_DELIVERY){
            
            startId = startDepot;     
     
            //Find directions from previous drop off intersection to a new pick-up
            subpath = find_path_between_intersections(startId, itDeliveries->pickUp, turn_penalty); 
            if (subpath.empty()){
                pathValid = false;
                break;
             }
            //For the first subpath, pickUpIndices should be empty 
            pickUpIndices.clear();
            subCourierPath = {startId, itDeliveries->pickUp, subpath, pickUpIndices}; 
            fullCourierPath.push_back(subCourierPath);
            
            startId = itDeliveries->pickUp;
        }

        else{//We are dealing with a pickup/drop-off
 
            //"add item to truck"
            pickUpIndices.push_back(deliveriesIdx); 
            
            //Find directions directly for the pickup to drop-off of the delivery
            subpath = find_path_between_intersections(startId, itDeliveries->dropOff, turn_penalty);
            if (subpath.empty()){
               pathValid = false;
               break;
            }
            //NOTE: not necessary for this implementation, since we are picking up one item at a time
            //update totalWeight
//          totalWeight = totalWeight + itDeliveries->itemWeight;
//          //check if weight of item exceeds the truck capacity
//          if (totalWeight > truck_capacity){ 
//              //path is deemed invalid (according to Piazza)
//              pathValid = false; 
//              break;
//          }
            
            //add new path section to the full Path
            subCourierPath = {startId, itDeliveries->dropOff, subpath, pickUpIndices}; 
            fullCourierPath.push_back(subCourierPath);

            itDeliveries++; 
            deliveriesIdx++;
            pickUpIndices.clear();
            
            //update startId
            startId = itDeliveries->dropOff; 
        }
    }
    //if no invalid paths were found thus far
    if (pathValid == true){
        //Add the last part of the directions: the path from the last drop-off location to the end Depot
        //Find directions directly from the pickup to drop-off of the same delivery
        subpath = find_path_between_intersections(startId, endDepot, turn_penalty);
        
        if (subpath.empty())
           pathValid = false; 
        
    }
        
    //last check: if final subpath was invalid, return false
    if (pathValid == false)
        return std::vector<CourierSubpath>(); 

    //add last path section to the full Path
    subCourierPath = {startId, endDepot, subpath, pickUpIndices}; 
    //At this point, pickUpIndices is empty vector
    fullCourierPath.push_back(subCourierPath);
    
    return     fullCourierPath;
    
}


