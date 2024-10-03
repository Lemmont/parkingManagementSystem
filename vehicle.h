#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <ctime>
#include "parkinglot.h"
#include <string>
#include <vector>

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

Vehicle* getAVehicle(std::map<int, Vehicle *>* vehicles);

std::vector<Vehicle *> createVehicles(int amount);
void addVehicles(std::vector<Vehicle *> vehicles,std::map<int, Vehicle*> *ridingVehicles, int amount);

#endif // VEHICLE_H