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


std::vector<CourierSubpath> traveling_courier( const std::vector<DeliveryInfo>& deliveries, const std::vector<int>& depots, const float turn_penalty, const float truck_capacity){
    
    //keep track of duplicate puck up or drop off intersections in deliveries
    bool duplicate = false;
    
    //dynamic vector of nodes that we want to search for. Initially only contains pick up nodes - no duplicates entered   
    std::vector <std::pair<int, std::string>> pickUpDropOffLocations;
    
    for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
        std::pair<int, std::string> pup ((*itDeliveries).pickUp, "pickup");
       
        for (std::vector<std::pair<int, std::string>>::iterator it = pickUpDropOffLocations.begin(); it != pickUpDropOffLocations.end(); it++){
            if (it->first == itDeliveries->pickUp){
                duplicate = true;
                break;
            }
        }
        if(!duplicate){
            pickUpDropOffLocations.push_back(pup);
        }
        else{
            duplicate = false;
        }
    }
            
   //keep track of status of path. Path is invalid if: 1) driving path does not exist or 2) an item weight exceeds truck capacity
    bool invalidPath = false; 
    //Full path from starting Depot to end Depot
    std::vector<CourierSubpath> fullPath;
    //Incremental path, from a specific intersection to another intersection
    std::vector<StreetSegmentIndex> drivingPath; 
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
    std::string pUdOprev, pUdOcurrent;
    std::vector<unsigned> deliveriesPickedUp;
    
    bool packagesLeft = true;
    
    while(packagesLeft){ 
        //to be safe
        pickUpIndices.clear();
        //check if nodes to be found is empty, if it is, break
        if (pickUpDropOffLocations.empty()){
            packagesLeft = false;
            break;
        }
        //Get the directions TO the pick up for the delivery
        //If we are dealing with the first pick up (the intersection before must be a depot)
        if (prevIntersectID == NO_DELIVERY_NODE){ 
            //Find directions from starting Depot to the nearest pickup
            drivingPath = find_path_djikstra(startDepot, pickUpDropOffLocations , turn_penalty);
            //if path does not exist
            if (drivingPath.empty()){ 
                invalidPath = true;
                break;
             }
            //This is the first subpath from starting intersection to first delivery. Pick up indices should have the starting indice.
            //not picking up anything at starting therefore empty pickUpIndices
            pickUpIndices.clear();
            
            //get intersection id that was reached in djikstra find_path and use it to put the subpath into struct and full path vector
            //set the current string
            intersectionFound = intersectionsReached.back().first;
            pUdOcurrent = intersectionsReached.back().second;
            
            deliverySubpath = { startDepot, intersectionFound, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
            
            //=======================This part looks messy but really its just adding the drop off intersection to the vector of places to checkout, and the for loops
            //=======================are to prevent duplicates. I tried with sets, but its messy to use those when the value is a pair, but if you have a better way, go for it
            
            //the first intersection found must be a pick up intersection, so add its drop off location into vector of intersections to be reached
            for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
                if (itDeliveries->pickUp == intersectionFound){
                    std::pair<int, std::string> doff ((*itDeliveries).dropOff, "dropoff");                   
                    //lots of for loops
                    duplicate = false;
                    for (std::vector<std::pair<int, std::string>>::iterator it = pickUpDropOffLocations.begin(); it != pickUpDropOffLocations.end(); it++){
                        if (it->first == itDeliveries->dropOff){
                            duplicate = true;
                            break;
                        }
                    }
                    if(!duplicate){
                        pickUpDropOffLocations.push_back(doff);
                        duplicate = false;
                    }
                }
            }
        }
            
        //We are dealing with a pickup or drop offthat is NOT the first
        else{
            //Find directions from previous drop off intersection to a new closest intersection which is in pickUpDropOff vector
            drivingPath = find_path_djikstra(prevIntersectID, pickUpDropOffLocations, turn_penalty); 
                       
            if (drivingPath.empty()){
                invalidPath = true;
                break;
             }
            //get intersection id that was reached in djikstra
            intersectionFound = intersectionsReached.back().first;

            //check if the node we are coming from is a pick up or drop off
            //if it is drop off, subtract total weight and clear pick up indices
            if (pUdOprev == "dropoff"){                      
                
                for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
                    if (itDeliveries->pickUp == prevIntersectID){
                        //dropping off a package so subtract items weight
                        totalWeight = totalWeight - itDeliveries->itemWeight;                        
                        break;
                    }
                }
                pickUpIndices.clear();
            }
            
            //we are coming from a pick up node so add weight of picked up item to total weight
            //and put delivery indices into pickUpIndices vector
            else if (pUdOprev == "pickup"){                           
                deliveryIndice = 0;
                for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
                    if (itDeliveries->pickUp == prevIntersectID){                        
                        
                        //if the previous intersectinoID was a pick up location of any of the deliveries, push back its indice
                        
                        //======This should probably be done something else maybe at the start of the code
                        //check if weight of individual item is too heavy - if it is, path is invalid
                        if (itDeliveries->itemWeight > truck_capacity){ 
                            invalidPath = true; 
                            break;
                        }
                        
                        
                        //check if total weight exceeds the truck capacity.
                        //if it does, do not pick up the item, do not add its weight to total weight, break and do not add the drop off location to pickupdropoff vector
                        //===== WE HAVE ISSUES WITH WHAT TO DO AFTER THIS. THE REST OF THE PICK UP LOCATIONS ARE BEING VISITED 
                        //======AND THEN ERASED FROM PICKUPDROPOFF VECTOR BUT THEN THEY ARE NEVER VISITED AGAIN SO THINGS DONT GET PICKED UP
                        if(totalWeight + itDeliveries->itemWeight > truck_capacity){
                            break;
                        }
                        totalWeight = totalWeight + itDeliveries->itemWeight;
                        pickUpIndices.push_back(deliveryIndice);                         
                    }
                    deliveryIndice++;
                }                
            }
            
            //THE PURPOSE OF THIS WHOLE IF STATEMENT IS JUST TO FIGURE OUT WHICH DROP OFF INTERSECTIONS NEED TO BE ADDED TO PICKUPANDDROPOFF WE CANT DO IT IN THE EARLIER
            // pUdOprev BECAUSE THEN ITS TOO LATE AND WE MIGHT SKIP THE DROP OFF EVEN IF ITS NEAR BY, WE HAVE TO ADD IT'S DROP OFF BEFORE WE LEAVE THE PICK UP
            
            //add drop off location when a package is picked up -> when pick up node is reached
            pUdOcurrent = intersectionsReached.back().second;
            if (pUdOcurrent == "pickup"){
                
                
                //I'LL LEAVE THE COMMENTED OUT STUFF HERE, MY IDEA WAS THE MAKE A DELIVERIESPICKEDUP VECTOR AND UPDATE IT AND USE THAT TO KEEP TRACK OF THE TOTAL WEIGHT STUFF
                //BUT I DIDNT GET IT WORKING AND IT LOOKS KINDA MESSY- THE OLDER VERSION IS UNDER IT
                /*
                //deliveries picked up must be added here 
                deliveryIndice = 0;
                deliveriesPickedUp.clear();
                for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
                    if (intersectionFound == itDeliveries -> pickUp){
                        
                        //check if total weight exceeds the truck capacity.
                        //if it does, do not pick up the item, do not add its weight to total weight, break and do not add the drop off location to pickupdropoff vector
                        if(totalWeight + itDeliveries->itemWeight > truck_capacity){
                            break;
                        }
                        deliveriesPickedUp.push_back(deliveryIndice);
                    }
                    deliveryIndice++;
                }
      
                //iterate through picked up deliveries
                for (std::vector<unsigned>::iterator itDeliveriesPickedUp = deliveriesPickedUp.begin(); itDeliveriesPickedUp != deliveriesPickedUp.end(); itDeliveriesPickedUp++){
                    //access picked up delivery, check if the pick up location of that delivery is the found intersection
                    if (deliveries[*itDeliveriesPickedUp].pickUp == intersectionFound){
                        
                        //add drop off location to vector which holds nodes to be visited
                        int dropOffId = deliveries[*itDeliveriesPickedUp].dropOff;
                        std::pair<int, std::string> doff (dropOffId, "dropoff");
                        
                        //lots of for loops- checking duplicates
                        duplicate = false;
                        for (std::vector<std::pair<int, std::string>>::iterator it = pickUpDropOffLocations.begin(); it != pickUpDropOffLocations.end(); it++){
                            if (it->first == dropOffId){
                                duplicate = true;
                                break;
                            }
                        }
                        if(!duplicate){
                            pickUpDropOffLocations.push_back(doff);
                            duplicate = false;
                        }
                    }
                } */
                
                
                for (std::vector<DeliveryInfo>::const_iterator itDeliveries = deliveries.begin(); itDeliveries != deliveries.end(); itDeliveries++){
                    
                    if (itDeliveries->pickUp == intersectionFound){ 
                        //check if delivery has actually been pickedUp
                        
                        //add drop off location to vector which holds nodes to be visited
                        std::pair<int, std::string> doff ((*itDeliveries).dropOff, "dropoff");
                        
                        //lots of for loops
                        duplicate = false;
                        for (std::vector<std::pair<int, std::string>>::iterator it = pickUpDropOffLocations.begin(); it != pickUpDropOffLocations.end(); it++){
                            if (it->first == itDeliveries->dropOff){
                                duplicate = true;
                            }
                        }
                        if(!duplicate){
                            pickUpDropOffLocations.push_back(doff);
                            duplicate = false;
                        }
                    }
                    deliveryIndice++;
                }               
            }
            
            //depending on whether it was a pick up or drop off, pickUpIndices has been updated and path is pushed            
            deliverySubpath = {prevIntersectID, intersectionFound, drivingPath, pickUpIndices}; 
            fullPath.push_back(deliverySubpath);
        }
                
        //update previous intersect id to the last intersection travelled to
        prevIntersectID = intersectionFound;
        
        //keep track of previous string type (pick up or drop off)
        pUdOprev = pUdOcurrent;
        
        
        //=======THIS IS CAUSING PROBLEMS BY ERASING THINGS THAT SHOULDN'T BE ERASED YET BECAUSE WE HAVENT PICKED UP ALL THE PACKAGES FROM AN INTERSECTION
        //MAYBE USE A
        //  bool allPickedUp
        
        //remove the intersection reached from pickUpDropOffLocations
            for (std::vector<std::pair<int, std::string>>::iterator itLocations = pickUpDropOffLocations.begin(); itLocations != pickUpDropOffLocations.end(); itLocations++){
                if (itLocations->first == intersectionFound){
                    pickUpDropOffLocations.erase(itLocations);
                    break;
                }
            }
    }
    
    if (invalidPath){
        //return empty vector
        return std::vector<CourierSubpath>(); 
    }
    
    //Path isn't invalid
    //Add the last part of the directions: the path from the last drop-off location to the end Depot
    
    drivingPath = find_path_between_intersections(prevIntersectID, endDepot, turn_penalty);
    if (drivingPath.empty()){
        //return empty vector
       return std::vector<CourierSubpath>(); 
    }
    
    //At this point, pickUpIndices is empty vector
    //for safety
    pickUpIndices.clear();

    //add new path section to the full Path
    deliverySubpath = {prevIntersectID, endDepot, drivingPath, pickUpIndices}; 
    
    fullPath.push_back(deliverySubpath);
    
    return fullPath;
    
}
 /*

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
    
}*/