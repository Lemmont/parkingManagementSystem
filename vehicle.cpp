#include "vehicle.h"
#include <vector>

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

/* Get a random vehicle pointer from a vector of pointer vehicles based*/
Vehicle* getAVehicle(std::map<int, Vehicle *>* vehicles) {
    if (vehicles->size() == 0) {
        return NULL;
    }

    std::srand(std::time(0));
    int randomIndex = std::rand() % vehicles->size();
    auto it = vehicles->begin();
    std::advance(it, randomIndex);

    return vehicles->at(it->first);
}

/*
    Create <amount> vehicles and return them in a vector.
*/
std::vector<Vehicle *> createVehicles(int amount) {
    std::vector<Vehicle*> vehicles;
    for (int i = 1; i <= amount; i++) {
        vehicles.push_back(new Vehicle(i));
    }
    return vehicles;
}

void addVehicles(std::vector<Vehicle *> vehicles, std::map<int, Vehicle*> *ridingVehicles , int amount) {
    for (int i = 1; i <= amount; i++) {
        Vehicle* newVh = new Vehicle(i);
        vehicles.push_back(newVh);
        ridingVehicles->insert(std::pair<int, Vehicle*>(newVh->id, newVh));
    }
}
