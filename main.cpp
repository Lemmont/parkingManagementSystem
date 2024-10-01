
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
std::random_device rd;

int vehicle_id = 0;
int parkinglot_id = 0;

std::list<ParkingLot *> parkingLots;


ParkingLot createParkingLot() {
    ParkingLot pl(++parkinglot_id, PARKINGLOT_SIZE);
    return pl;
}

Vehicle* getAVehicle(std::vector<Vehicle *>* vehicles) {
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0, vehicles->size() - 1);
    auto diceLength = std::bind( distribution, generator);
    int num = diceLength();

    return (*vehicles)[num];
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

/* Get the most optimal parking lot to park a vehicle into. */
ParkingLot* parkingLoadBalancer(std::list<ParkingLot*>* parkingLots) {
    if (parkingLots->empty()) return nullptr;

    ParkingLot* leastLoadedLot = nullptr;
    int minVehicles = 16;

    for (auto it = parkingLots->begin(); it != parkingLots->end(); it++) {
        ParkingLot* lot = *it;
        int vehicleCount = lot->vehicles.size();

        int maxVehicles = lot->space;

        if (vehicleCount < maxVehicles || (vehicleCount < maxVehicles && vehicleCount < minVehicles)) {
            minVehicles = vehicleCount;
            leastLoadedLot = lot;
        }
    }

    if (leastLoadedLot == nullptr) {
        ParkingLot* newPl = new ParkingLot(parkinglot_id++ ,PARKINGLOT_SIZE);
        parkingLots->push_back(newPl);
        return newPl;
    }


    return leastLoadedLot;

}

void displayParkingLots() {
    for (auto it = parkingLots.begin(); it != parkingLots.end(); it++) {
        ParkingLot* lot = *it;
        lot->summarizeLot();
    }
}

void loop(std::vector<Vehicle*>* vehicles, ParkingLot* plInit, std::_Bind<std::uniform_int_distribution<int> (std::default_random_engine)> dice2, int day) {
    ParkingLot* pl = plInit;
    for (int i = 0; i < DAY_STEPS; i++) {
        std::cout << "ok" << "\n";
        std::cout << "\033[2J\033[1;1H";;

        int num = dice2();

        // park 1+n car
        if (num > 0) {
            int var = 5;
            for (int j = 0; j < 5; j++) {
                Vehicle* veh = getAVehicle(vehicles);
                if (veh->parkingLot == NULL) {
                    pl = parkingLoadBalancer(&parkingLots);
                    pl->enterLot(veh, day, i);
                }
            }


        } else if (num == 0) {
            // leave 1+n car
            int var = 5;
            for (int j = 0; j < 5; j++) {
                Vehicle* veh = getAVehicle(vehicles);
                if (veh->parkingLot != NULL) {
                    pl = veh->parkingLot;
                    pl->leaveLot(veh);
                }
            }
        }

        std::cout << "Day: " << day << ", Hour: " << i << "\n\n";

        /* check for parking lots that have been empty for a long time */

        // pk->viewLot();
        //pl->summarizeLot();
        displayParkingLots();


        // car goes into avaiable parking lot
        // go over parking lots.

        // suspense between iterations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main() {
    int daysPassed = 0;git a
    int maxDays = 30;


    // generate 2-dice
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0,2);
    auto dice2 = std::bind ( distribution, generator);

    std::vector<Vehicle*> vehicles = createVehicles(300);
    std::cout << vehicles.size() << "\n";

    Vehicle* vehicle = getAVehicle(&vehicles);
    std::cout << vehicle->id << "\n";

    ParkingLot pl = ParkingLot(parkinglot_id++ , PARKINGLOT_SIZE * 4);
    parkingLots.push_back(&pl);
    std::cout << "ok" << "\n";
    while(daysPassed <= maxDays) {
        loop(&vehicles, &pl, dice2, daysPassed);
        daysPassed++;
        std::cout << "Day: " << daysPassed << " \n";
    };


    return 0;
}

