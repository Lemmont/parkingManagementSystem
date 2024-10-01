#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <ctime>
#include "parkinglot.h"
#include <string>

class ParkingLot;


class Vehicle {
    public:
        int id;
        int entryDay;
        int entryHour;
        std::string state;
        ParkingLot* parkingLot;

        Vehicle();
        Vehicle(int x);

        void enterLot(ParkingLot* parkingLot, int day, int hour);
        void leaveLot(ParkingLot* parkingLot);

        void timeSinceEntry();


};
#endif // VEHICLE_H