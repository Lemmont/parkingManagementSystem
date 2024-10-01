#include "vehicle.h"

Vehicle::Vehicle() {
    id = 0;
}

Vehicle::Vehicle(int x) {
    id = x;
    entryDay = 0;
    entryHour = 0;
    state = "Moving";
    parkingLot = NULL;
}

void Vehicle::enterLot(ParkingLot* parkingLot, int day, int hour) {
    // check if full?
    this->parkingLot = parkingLot;
    this->state = "Parked";
    this->entryDay = day;
    this->entryHour = hour;
};

void Vehicle::leaveLot(ParkingLot* parkingLot) {
    // check if full?
    this->parkingLot = NULL;
    this->entryDay = 0;
    this->state= "Moving";
    this->entryHour = 0;
};