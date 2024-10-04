#include "parkinglot.h"
#include "vehicle.h"
#include <list>


ParkingLot::ParkingLot(int x, int y, float rate, float cost, float startingBalance) {
    id = x;
    space = y;
    hourRate = rate;
    dailyCosts = cost;
    balance = startingBalance;
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
    if  (this->vehicles.count(vehicle->id) != 0) {
        return 3; // already found
    } else {
        vehicle->enterLot(this, day, hour);
        this->vehicles[vehicle->id] = vehicle;
        //entryMessage(vehicle);
        return 0;
    }
};

int ParkingLot::leaveLot(Vehicle* vehicle, int day, int hour) {
    // check if vehicle is valid
    if (vehicle == NULL) {
        return 2; // not valid vehicle
    }

    if  (this->vehicles.count(vehicle->id) == 0) {;
        return 3; // not found
    } else {
        // calculate costs
        int daysSpent = day - vehicle->entryDay;
        int hoursSpent = (hour - vehicle->entryHour) + hour + (daysSpent > 1 ? 24 * daysSpent : 0);
        int hourRate = vehicle->parkingLot->getHourRate();
        float owned = hourRate * hoursSpent;

        vehicle->parkingLot->updateBalance(owned, owned >= 0 ? true : false);
        vehicle->parkingLot->updateMoneyEarned(owned);


        this->vehicles.erase(vehicle->id);

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

float ParkingLot::getMoneyEarned() {
    return this->moneyEarned;
};
float ParkingLot::getHourRate() {
    return this->hourRate;
};
float ParkingLot::getDailyCosts() {
    return this->dailyCosts;
};

float ParkingLot::getBalance() {
    return this->balance;
};
void ParkingLot::updateBalance(float value, bool addition) {
    if (addition) {
        this->balance += value;
    } else {
        this->balance -= value;
    }
};

void ParkingLot::updateCosts(float value) {
    this->dailyCosts *= value;
};

void ParkingLot::updateRate(float value) {
    this->hourRate += value;
}

void ParkingLot::updateMoneyEarned(float value) {
    this->moneyEarned =+ value;
};

void ParkingLot::summarizeLot() {
    std::cout << "Parking Lot " << this->id << " ( balance: " << this->balance <<" )" << " ( rate: " << this->hourRate << " )" << " ( costs: " << this->dailyCosts << " ) " << " : " << this->vehicles.size() << "/" << this->space << "\n";
}

void ParkingLot::entryMessage(Vehicle* vehicle){
    std::cout << "vehicle " << vehicle->id << " has entered lot " << this->id << " on day " << vehicle->entryDay << " at hour" << vehicle->entryHour << "\n";
};

void ParkingLot::leaveMessage(Vehicle* vehicle){
    std::cout << "vehicle " << vehicle->id << " has left lot " << this->id << " on day " << vehicle->entryDay << " at hour" << vehicle->entryHour << "\n";
};

// Create a new parkinglot with some randomness.
ParkingLot* createNewParkingLot(int parkinglot_id, int parkinglot_size, float base_daily_cost, float startingBalance) {
    int size_factor = rand()%(10-1 + 1) + 1;
    int rate_factor = rand()%(10-1 + 1) + 1;
    ParkingLot* newPl = new ParkingLot(parkinglot_id, parkinglot_size * size_factor, 1.25 * rate_factor, base_daily_cost * size_factor, startingBalance * size_factor);

    return newPl;
};

void displayParkingLots(std::list<ParkingLot *> parkingLots) {
    for (auto it = parkingLots.begin(); it != parkingLots.end(); it++) {
        ParkingLot* lot = *it;
        lot->summarizeLot();
    }
}

void updateBalanceDailyCostParkingLot(std::list<ParkingLot *> parkingLots) {
    for (auto it = parkingLots.begin(); it != parkingLots.end(); it++) {
        ParkingLot* lot = *it;
        lot->updateBalance(lot->getDailyCosts(), false);
    }
}