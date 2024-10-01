
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


int DAY_STEPS = 24;
int PARKINGLOT_SIZE = 16;

int vehicle_id = 0;
int parkinglot_id = 0;

std::list<ParkingLot> parkingLots;


ParkingLot createParkingLot() {
    ParkingLot pl(++parkinglot_id, PARKINGLOT_SIZE);
    return pl;
}

void loop(std::_Bind<std::uniform_int_distribution<int> (std::default_random_engine)> dice) {

    for (int i = 0; i < DAY_STEPS; i++) {

        int num = dice();

        // new car
        if (num == 1) {
            Vehicle vehicle(++vehicle_id);
            std::cout << "hour: " << i << " vehicle: " << vehicle.id <<" \n";
        } else {
            std::cout << "hour: " << i <<" \n";
        }

        // car goes into avaiable parking lot
        // go over parking lots.

        // suspense between iterations
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}



int main() {
    int daysPassed = 0;
    int maxDays = 30;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(1,2);
    auto dice = std::bind ( distribution, generator);

    while(daysPassed <= maxDays) {
        loop(dice);
        daysPassed++;
        std::cout << "Day: " << daysPassed << " \n";
    };

    // ParkingLot pl1(1, 2);
    // Vehicle vh1(1);
    // Vehicle vh2(2);

    // pl1.enterLot(&vh1);
    // pl1.enterLot(&vh2);

    // std::cout << "Parking Lot Id: " << pl1.id << ", Size: " << pl1.space << "\n";
    // std::cout << "Members: " << pl1.vehicles.size() << "\n";

    return 0;
}