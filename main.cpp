
/*
    Develop a program to:
    .track vehicles entering and exiting a parking lot
    .calculate charges
    .maintain vehicle details.
*/
#include "parkinglot.h"
#include "vehicle.h"
#include <iostream>
#include <functional>
#include <list>
#include <random>
#include <thread>
#include <chrono>
#include <climits>


int DAY_STEPS = 24;
int PARKINGLOT_SIZE = 16;
int BASE_DAILY_COST = 500;

std::random_device rd;

int vehicle_id = 0;
int parkinglot_id = 0;

std::list<ParkingLot *> parkingLots;
std::map<int, Vehicle*> parkedVehicles;
std::map<int, Vehicle*> ridingVehicles;



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


void setParkedVehicle(Vehicle* vehicle) {
    if (vehicle != NULL && parkedVehicles.count(vehicle->id) == 0) {
        parkedVehicles[vehicle->id] = vehicle;
        ridingVehicles.erase(vehicle->id);
    }
}

void setRidingVehicle(Vehicle* vehicle) {
    if (vehicle != NULL && ridingVehicles.count(vehicle->id) == 0) {
        ridingVehicles[vehicle->id] = vehicle;
        parkedVehicles.erase(vehicle->id);
    }
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

ParkingLot* createNewParkingLot() {
    int size_factor = rand()%(10-1 + 1) + 1;
    int rate_factor = rand()%(10-1 + 1) + 1;
    ParkingLot* newPl = new ParkingLot(parkinglot_id++ , PARKINGLOT_SIZE * size_factor, 1.25 * rate_factor, BASE_DAILY_COST * size_factor);
    return newPl;
};

/* Get the most optimal parking lot to park a vehicle. */
ParkingLot* parkingLoadBalancer(std::list<ParkingLot*>* parkingLots) {
    if (parkingLots->empty()) return nullptr;

    ParkingLot* bestLot = nullptr;
    double minScore = std::numeric_limits<double>::max();

    // Weights for occupancy and hourly rate
    double w_occupancy = 0.6;  // 70% weight for occupancy
    double w_rate = 0.4;       // 30% weight for rate

    // Find the maximum hourly rate in the parking lots to normalize rates
    double maxRate = 0.0;
    for (auto lot : *parkingLots) {
        if (lot->getHourRate() > maxRate) {
            maxRate = lot->getHourRate();
        }
    }

    for (auto it = parkingLots->begin(); it != parkingLots->end(); it++) {
        ParkingLot* lot = *it;
        int vehicleCount = lot->vehicles.size();
        int maxVehicles = lot->space;

        if (vehicleCount >= maxVehicles) {
            continue;
        }

        double occupancy = static_cast<double>(vehicleCount) / maxVehicles;

        double normalizedRate = static_cast<double>(lot->getHourRate()) / maxRate;

        double score = (w_occupancy * occupancy) + (w_rate * normalizedRate);

        if (occupancy < minScore) {
            minScore = score;
            bestLot = lot;
        }
    }

    if (bestLot == nullptr || minScore >= 1.0) {
        ParkingLot* newPl = createNewParkingLot();
        parkingLots->push_back(newPl);
        return newPl;
    }


    return bestLot;

}

void displayParkingLots() {
    for (auto it = parkingLots.begin(); it != parkingLots.end(); it++) {
        ParkingLot* lot = *it;
        lot->summarizeLot();
    }
}

void updateBalanceDailyCostParkingLot() {
    for (auto it = parkingLots.begin(); it != parkingLots.end(); it++) {
        ParkingLot* lot = *it;
        lot->updateBalance(lot->getDailyCosts(), false);
    }
}

void loop(std::vector<Vehicle*>* vehicles, ParkingLot* plInit, int day) {
    ParkingLot* pl = plInit;
    int busyFactor = 1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.0-1.0)));
    for (int i = 0; i < DAY_STEPS; i++) {
        // clear screen
        std::cout << "\033[2J\033[1;1H";


        // determine random chances and factors
       float r3 = 0.9 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.3-0.9)));

       int hourEnter = 1;
       int hourLeave = 1;

       if (i >= 7 && i < 19) {
            hourEnter = 2;
            hourLeave = 0.8;
       } else {
            hourEnter = 0.1;
            hourLeave = 4;
       }

       int baseEnter = 10;
       int baseLeave = 10;

        // leave parking lot
       for (int j = 0; j < (int) (baseLeave * busyFactor) * hourLeave * r3; j++) {
            Vehicle* veh = getAVehicle(&parkedVehicles);
            if (veh != NULL) {
                pl = veh->parkingLot;
                pl->leaveLot(veh, day, i);
                setRidingVehicle(veh);
            }
        }

        // enter parking lot
        for (int j = 0; j < (int) (baseEnter * busyFactor) * hourEnter; j++) {
            Vehicle* veh = getAVehicle(&ridingVehicles);
            if (veh != NULL && veh->parkingLot == NULL) {
                pl = parkingLoadBalancer(&parkingLots);
                pl->enterLot(veh, day, i);
                setParkedVehicle(veh);
            }
        }

        std::cout << "Day: " << day << ", Hour: " << i << "\n\n";

        /* check for parking lots that have been empty for a long time */;
        displayParkingLots();

        // suspense between iterations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    updateBalanceDailyCostParkingLot();
}



int main() {
    srand (static_cast <unsigned> (time(0)));
    int daysPassed = 0;
    int maxDays = 30;

    // create vehicles and save them in vector
    std::vector<Vehicle*> vehicles = createVehicles(1000);

    for (int i = 0; i < vehicles.size(); i++) {
        ridingVehicles[vehicles[i]->id] = vehicles[i];
    }

    // create first parking lot
    ParkingLot* pl = createNewParkingLot();
    parkingLots.push_back(pl);

    // Hour progression
    while(daysPassed <= maxDays) {
        loop(&vehicles, pl, daysPassed);
        daysPassed++;
        std::cout << "Day: " << daysPassed << " \n";
    };


    return 0;
}

