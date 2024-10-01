#include "vehicle.h"

Vehicle::Vehicle(int x) {
    id = x;
    entryTimestamp = 0;
    parkingLot = NULL;
}

void Vehicle::enterLot(ParkingLot* parkingLot) {
    // check if full?
    this->parkingLot = parkingLot;
    this->entryTimestamp = std::time(0);
};