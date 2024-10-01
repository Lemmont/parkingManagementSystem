#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <ctime>
#include "parkinglot.h"

class ParkingLot;

class Vehicle {
    public:
        int id;
        time_t entryTimestamp;
        ParkingLot* parkingLot;

        Vehicle(int x);

        void enterLot(ParkingLot* parkingLot);

        void timeSinceEntry();


};
#endif // VEHICLE_H