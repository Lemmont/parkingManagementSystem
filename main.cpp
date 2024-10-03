
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

int vehicle_id = 0;
int parkinglot_id = 0;

std::random_device rd;

std::list<ParkingLot *> parkingLots;
std::map<int, int> lotsInNegative = {};
std::map<int, Vehicle*> parkedVehicles;
std::map<int, Vehicle*> ridingVehicles;


// set a vehicle as parked
void setParkedVehicle(Vehicle* vehicle) {
    if (vehicle != NULL && parkedVehicles.count(vehicle->id) == 0) {
        parkedVehicles[vehicle->id] = vehicle;
        ridingVehicles.erase(vehicle->id);
    }
}

// set a vehicle as riding
void setRidingVehicle(Vehicle* vehicle) {
    if (vehicle != NULL && ridingVehicles.count(vehicle->id) == 0) {
        ridingVehicles[vehicle->id] = vehicle;
        parkedVehicles.erase(vehicle->id);
    }
}



/* Get the most optimal parking lot to park a vehicle. */
ParkingLot* parkingLoadBalancer(std::list<ParkingLot*>* parkingLots) {
    if (parkingLots->empty()) {
        ParkingLot* newPl = createNewParkingLot(++parkinglot_id, PARKINGLOT_SIZE, BASE_DAILY_COST, -10000.0);
        parkingLots->push_back(newPl);
        return newPl;
    };

    ParkingLot* bestLot = nullptr;
    double minScore = std::numeric_limits<double>::max();

    // Weights for occupancy and hourly rate
    double w_occupancy = 0.4;  // 70% weight for occupancy
    double w_rate = 0.6;       // 30% weight for rate

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
        ParkingLot* newPl = createNewParkingLot(++parkinglot_id, PARKINGLOT_SIZE, BASE_DAILY_COST, -10000.0);
        parkingLots->push_back(newPl);
        return newPl;
    }


    return bestLot;

}

std::list<ParkingLot *> deleteParkingLots(std::list<ParkingLot *>* parkingLots, std::map<int, Vehicle*>* parkedVehicles, int day, int hour) {
    std::list<ParkingLot *> copyParkingLots = *parkingLots;
    std::map<int, Vehicle*> copyParkedVehicles = *parkedVehicles;

    for (auto it = parkingLots->begin(); it != parkingLots->end(); it++) {
        ParkingLot* lot = *it;
        if (lot->getBalance() < 0) {
            if (lotsInNegative.count(lot->id) == 0) {
                lotsInNegative[lot->id] = 1;
                // 3 dagen
            } else if (lotsInNegative[lot->id] == 3) {
                // remove vehicles
                for (auto pv = copyParkedVehicles.begin(); pv != copyParkedVehicles.end(); pv++) {
                    if (pv->second->parkingLot != NULL && pv->second->parkingLot->id == lot->id) {
                        lot->leaveLot(pv->second, day, hour);
                        setRidingVehicle(pv->second);
                    }
                }
                // remove from tracker map
                copyParkingLots.remove(lot);
                lotsInNegative.erase(lot->id);

                // delete lot
                //free(lot);
            } else {
                lotsInNegative[lot->id] += 1;
            }
        }

        //lot->updateBalance(lot->getDailyCosts(), false);
    }
    return copyParkingLots;
}

void buyUpgradeLotSize(std::list<ParkingLot *>* parkingLots) {
    std::list<ParkingLot *> copyParkingLots = *parkingLots;
    for (auto it = parkingLots->begin(); it != parkingLots->end(); it++) {
        ParkingLot* lot = *it;
        if (lot->getBalance() > 50000) {
            // buy upgrade, double lot size
            lot->updateBalance(50000, false);
            lot->space *= 2;
            lot->updateCosts(2);
        }
    }
}

int getTotalSpace(std::list<ParkingLot *>* parkingLots) {
    std::list<ParkingLot *> copyParkingLots = *parkingLots;
    int space = 0;
    for (auto it = parkingLots->begin(); it != parkingLots->end(); it++) {
        ParkingLot* lot = *it;
        space += lot->space;
    }
    return space;
}

void loop(std::vector<Vehicle*>* vehicles, ParkingLot* plInit, int day) {
    ParkingLot* pl = plInit;
    int busyFactor = 1.0 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(3.0-1.0)));
    for (int i = 0; i < DAY_STEPS; i++) {
        srand (static_cast <unsigned> (time(0)));
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

       int baseEnter = 10 + ((int)(getTotalSpace(&parkingLots) / 20) * 1);
       int baseLeave = 10 + ((int)(getTotalSpace(&parkingLots) / 20) * 1);

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
        displayParkingLots(parkingLots);

        // suspense between iterations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    updateBalanceDailyCostParkingLot(parkingLots);
    parkingLots = deleteParkingLots(&parkingLots, &parkedVehicles, day, 0);
    displayParkingLots(parkingLots);
    buyUpgradeLotSize(&parkingLots);
    //addVehicles(*vehicles, &ridingVehicles ,100); //TODO depends on current supply-demand balance.
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
    ParkingLot* pl = createNewParkingLot(parkinglot_id, PARKINGLOT_SIZE, BASE_DAILY_COST, 0.0);
    parkingLots.push_back(pl);

    // Hour progression
    while(daysPassed <= maxDays) {
        loop(&vehicles, pl, daysPassed);
        daysPassed++;
        std::cout << "Day: " << daysPassed << " \n";
    };


    return 0;
}

