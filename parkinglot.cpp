#include "parkinglot.h"
#include "vehicle.h"

ParkingLot::ParkingLot(int x, int y) {
    id = x;
    space = y;
};

int ParkingLot::enterLot(Vehicle* vehicle, int day, int hour) {
    // check if there is space
    if (this->space == this->vehicles.size()) {
        return 1; // not space left
    }

    // check if vehicle is valid
    if (vehicle == NULL) {
        return 2; // not valid vehicle
    }

    // check if not in already
    if  (vehicles.count(vehicle->id) != 0) {
        return 3; // already found
    } else {
        vehicles[vehicle->id] = vehicle;
        vehicle->enterLot(this, day, hour);
        //entryMessage(vehicle);
        return 0;
    }
};

int ParkingLot::leaveLot(Vehicle* vehicle) {
    // check if vehicle is valid
    if (vehicle == NULL) {
        return 2; // not valid vehicle
    }

    if  (vehicles.count(vehicle->id) == 0) {
        return 3; // not found
    } else {
        vehicles.erase(vehicle->id);
        vehicle->leaveLot(this);
        //leaveMessage(vehicle);
        return 0;
    }
}

void ParkingLot::viewLot() {
    std::map<int, Vehicle*>::iterator it;
    std::cout << "Parking Lot " << this->id << " \n";
    for (it = vehicles.begin(); it != vehicles.end(); it++) {
        std::cout << "Day: " << it->second->entryDay << " Hour: " << it->second->entryHour << " Vehicle: " << it->second->id << "\n";
    }
}

void ParkingLot::summarizeLot() {
    std::cout << "Parking Lot " << this->id << " : " << this->vehicles.size() << "/" << this->space << "\n";
}

void ParkingLot::entryMessage(Vehicle* vehicle){
    std::cout << "vehicle " << vehicle->id << " has entered lot " << this->id << " on day " << vehicle->entryDay << " at hour" << vehicle->entryHour << "\n";
};

void ParkingLot::leaveMessage(Vehicle* vehicle){
    std::cout << "vehicle " << vehicle->id << " has left lot " << this->id << " on day " << vehicle->entryDay << " at hour" << vehicle->entryHour << "\n";
};