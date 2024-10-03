#ifndef PARKINGLOT_H
#define PARKINGLOT_H

#include <iostream>
#include <map>
#include <list>
#include "vehicle.h"

class Vehicle;

class ParkingLot {
    public:
        int id;
        int space;
        std::map<int, Vehicle*> vehicles = {};

        ParkingLot(int x, int y, float rate, float cost, float startingBalance);

        int enterLot(Vehicle* vehicle, int day, int hour);

        int leaveLot(Vehicle* vehicle, int day, int hour);

        float getMoneyEarned();
        float getHourRate();
        float getDailyCosts();

        float getBalance();
        void updateBalance(float value, bool addition);
        void updateMoneyEarned(float value);

        void updateCosts(float value);


        void viewLot();
        void summarizeLot();

    private:

        float moneyEarned = 0.0;
        float hourRate;
        float dailyCosts;
        float balance;

        void entryMessage(Vehicle* vehicle);
        void leaveMessage(Vehicle* vehicle);
};

ParkingLot* createNewParkingLot(int parkinglot_id, int parkinglot_size, float base_daily_cost, float startingBalance);
void displayParkingLots(std::list<ParkingLot *> parkingLots);
void updateBalanceDailyCostParkingLot(std::list<ParkingLot *> parkingLots);

#endif // PARKINGLOT_H