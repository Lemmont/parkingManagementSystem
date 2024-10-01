#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include <iostream>
#include <map>
#include "vehicle.h"

class Vehicle;

class ParkingLot {
    public:
        int id;
        int space;
        std::map<int, Vehicle*> vehicles;

        ParkingLot(int x, int y);

        int enterLot(Vehicle* vehicle);

        // int leaveLot(Vehicle* vehicle);

        // void viewLot();

    private:

        void entryMessage(Vehicle* vehicle);
};

#endif // PARKINGLOT_H