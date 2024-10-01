#include "parkinglot.h"
#include "vehicle.h"

ParkingLot::ParkingLot(int x, int y) {
    id = x;
    space = y;
};

int ParkingLot::enterLot(Vehicle* vehicle) {
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
        vehicle->enterLot(this);
        entryMessage(vehicle);
        return 0;
    }
};

void ParkingLot::entryMessage(Vehicle* vehicle){
            std::cout << "vehicle " << vehicle->id << " has entered lot " << this->id << " at " << time(0) << "\n";
        };