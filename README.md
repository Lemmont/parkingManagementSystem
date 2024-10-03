# Parking Management System
A Parking Management System that simulates a paid parking lot that keeps track of vehicles, the time they spend parking and what they owe the parking lot. 

### Goal
The goal of this program is to test my C++ skills, get to know the syntax and the infrastructure better. More precisely: object oriented programming, referencing classes from other files and memory management.

Furthermore, providing my Github with an additional project to show of my endeavor in becoming a better software developer. 

### Results
- Not completed with the program yet.
- Given my experience of C, which I obtained during my university program, C++ is a piece of cake and only a quick refresher was needed to get the hang of it. Hoping to do some more complex stuff in the future.
- Weighted scoring functions

### Simulation
This program runs based on a clock, where every hour of a day is a iteration. Eacht 24 step cycle is a labeled as a day. Each hour, a car can decide to enter or leave a parking lot. It does this based on the avaiable space in the parking lots, basing its decision on the most 'attractive' one. It prefers the parking lot with the most space left, skipping any other which may have space left but not as much as the favorite. 

#### Vehicle
A vehicle want to enter a parking lot and stay there for a random amount of time (hours). 

#### Parking Lot
A parking lot provides space for vehicles that want to stay there. The total amount of space (maximum vehicles that can be in a parking lot at any given time) is determined at creation. A parking lot charges vehicles a fee for every hour that they stay there. The 'hourly rate' is determiend at creation. Each parking lot pays a daily amount for maintance of the parking lot, which is based on its size.

#### Load balacing
How do vehicles 'choose' what parking lot seems optimal for them?

Currently, the following factors are considered:
- Occupancy
- Hour rate

Using a weighted scoring function, multiple factors can be considered when choosing the optimal lot. Each factor has a weight associated with it given its importance in the choosing process. For each parking lot, a occupancy_ratio is calculated to determine its 'attractiveness' given its total amount of space avaiable and its current occupancy. Then the hourly rate is normalized so that all the rates are compared on the same scale. Ultimately, all these factors are used to determine a score which is then used to compare it to other parking lots. 

score= w_occupancy * occupancy_ratio + w_rate * (max_rate/hourly_rate)
​
#### Closing parking lots
When a parking lot is not attracting vehicles, staying open is not profitable. When a parking lot has a debt ((daily revenue - daily costs) < 0) for consecutive days in a row, a parking lot closes.

#### Expanding parking lots
A parking lot can decide to expand the amount of vehicles it can house. When it has atleast 50000 in balance, it can double its parking lot size. If expanded, so does the operating costs which is also doubled.


More to be added...
