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

        int enterLot(Vehicle* vehicle, int day, int hour);

        int leaveLot(Vehicle* vehicle);

        void viewLot();
        void summarizeLot();

    private:

        void entryMessage(Vehicle* vehicle);
        void leaveMessage(Vehicle* vehicle);
};

#endif // PARKINGLOT_H