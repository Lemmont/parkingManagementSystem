
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
int BREAK_POINT = 2;

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

        /*
            *0-5*
            8-18: druk (aankomen)
            18-7: rustig (weggaan)

            *6-7*
            10-21: druk
            21:10: rustig
        */

        if (num >= BREAK_POINT + ((i > 8 && i < 18) ? -1 : 2)) {
            // park 1+n car
            int var = 5;
            for (int j = 0; j < 5; j++) {
                Vehicle* veh = getAVehicle(vehicles);
                if (veh->parkingLot == NULL) {
                    pl = parkingLoadBalancer(&parkingLots);
                    pl->enterLot(veh, day, i);
                }
            }


        } else if (num < BREAK_POINT + ((i > 8 && i < 18) ? -1 : 3)) {
            // leave 1+n car
            int var = 5;

            // TODO: track vehicles still present and remove them from there.
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

        // suspense between iterations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main() {

    int daysPassed = 0;
    int maxDays = 30;

    // Generate 2-dice
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0,3);
    auto dice2 = std::bind ( distribution, generator);

    // create vehicles and save them in vector
    std::vector<Vehicle*> vehicles = createVehicles(300);

    // create first parking lot
    ParkingLot pl = ParkingLot(parkinglot_id++ , PARKINGLOT_SIZE * 4);
    parkingLots.push_back(&pl);

    // Hour progression
    while(daysPassed <= maxDays) {
        loop(&vehicles, &pl, dice2, daysPassed);
        daysPassed++;
        std::cout << "Day: " << daysPassed << " \n";
    };


    return 0;
}

