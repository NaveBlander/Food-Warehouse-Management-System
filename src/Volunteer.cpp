#include "Volunteer.h"
#include "Order.h"

//Volunteer ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Volunteer::Volunteer(int id, const string &name) :
    completedOrderId(NO_ORDER),
    activeOrderId(NO_ORDER),
    id(id),
    name(name){}

int Volunteer::getId() const {return id;}

const string& Volunteer::getName() const {return name;}

int Volunteer::getActiveOrderId() const {return activeOrderId;}

int Volunteer::getCompletedOrderId() const {return completedOrderId;}

bool Volunteer::isBusy() const {return activeOrderId != NO_ORDER;}

//Collector Volunteer -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown) :
    Volunteer(id, name),
    coolDown(coolDown),
    timeLeft(coolDown) {}

CollectorVolunteer* CollectorVolunteer::clone() const {return new CollectorVolunteer(*this);}

void CollectorVolunteer::step() {
    if(decreaseCoolDown()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string CollectorVolunteer::toString() const{
    string output = "VolunteerID: " + std::to_string(getId()) +
                    "\nisBusy: " + std::string(this->isBusy() ? "True" : "False");
    if(isBusy()){
        output = output + 
                "\nOrderID: " + std::to_string(activeOrderId) +
                "\nTimeLeft: " + std::to_string(timeLeft);
    }
    else{
        output = output + 
                "\nOrderID: None" +
                "\nTimeLeft: None";
    }

    output = output +
            "\nOrdersLeft: No Limit";

    return output;
}

int CollectorVolunteer::getCoolDown() const {return coolDown;}

int CollectorVolunteer::getTimeLeft() const {return timeLeft;}

bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft--;
    return timeLeft==0;
}

bool CollectorVolunteer:: hasOrdersLeft() const {return true;}

bool CollectorVolunteer::canTakeOrder(const Order &order) const {return !isBusy();}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;    
}

char CollectorVolunteer::getTypeVolunteer() {return 'C';}


//Limited Collector Volunteer ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown, int maxOrders) : 
    CollectorVolunteer(id, name, coolDown),
    maxOrders(maxOrders),
    ordersLeft(maxOrders) {}

LimitedCollectorVolunteer* LimitedCollectorVolunteer::clone() const {return new LimitedCollectorVolunteer(*this);}

bool LimitedCollectorVolunteer::hasOrdersLeft() const {return ordersLeft != 0;}

bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {return !isBusy() and hasOrdersLeft();}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {CollectorVolunteer::acceptOrder(order); ordersLeft--;}

int LimitedCollectorVolunteer::getMaxOrders() const {return maxOrders;}

int LimitedCollectorVolunteer::getNumOrdersLeft() const {return ordersLeft;}

string LimitedCollectorVolunteer::toString() const{
    string output = "VolunteerID: " + std::to_string(getId()) +
                    "\nisBusy: " + std::string(this->isBusy() ? "True" : "False");
    if(isBusy()){
        output = output + 
                "\nOrderID: " + std::to_string(activeOrderId) +
                "\nTimeLeft: " + std::to_string(getTimeLeft());
    }
    else{
        output = output + 
                "\nOrderID: None" +
                "\nTimeLeft: None";
    }

    output = output +
            "\nOrdersLeft: " + std::to_string(getNumOrdersLeft());

     return output;
}

char LimitedCollectorVolunteer::getTypeVolunteer() {return 'c';}

//Driver Volunteer ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) :
    Volunteer(id, name),
    maxDistance(maxDistance),
    distancePerStep(distancePerStep),
    distanceLeft(0) {}

DriverVolunteer* DriverVolunteer::clone() const {return new DriverVolunteer(*this);}

int DriverVolunteer::getDistanceLeft() const {return distanceLeft;}

int DriverVolunteer::getMaxDistance() const {return maxDistance;}

int DriverVolunteer::getDistancePerStep() const {return distancePerStep;}

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft = distanceLeft-distancePerStep;
    if(distanceLeft<=0){
        return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {return true;}

bool DriverVolunteer::canTakeOrder(const Order &order) const {return (!isBusy() and order.getDistance() <= maxDistance);}

void DriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    distanceLeft = order.getDistance();
}

void DriverVolunteer::step() {
    if(decreaseDistanceLeft()){
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const{
    string output = "VolunteerID: " + std::to_string(getId()) +
                    "\nisBusy: " + std::string(this->isBusy() ? "True" : "False");
    if(isBusy()){
        output = output + 
                "\nOrderID: " + std::to_string(activeOrderId) +
                "\nTimeLeft: " + std::to_string(distanceLeft);
    }
    else{
        output = output + 
                "\nOrderID: None" +
                "\nTimeLeft: None";
    }

    output = output +
            "\nOrdersLeft: No Limit";

    return output;
}

void DriverVolunteer::setDistanceLeft(const int distanceToSet) {distanceLeft = distanceToSet;}

char DriverVolunteer::getTypeVolunteer() {return 'D';}

//Limited Driver Volunteer --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep, int maxOrders) :
    DriverVolunteer(id, name, maxDistance, distancePerStep),
    maxOrders(maxOrders),
    ordersLeft(maxOrders) {}

LimitedDriverVolunteer* LimitedDriverVolunteer::clone() const {return new LimitedDriverVolunteer(*this);}

int LimitedDriverVolunteer::getMaxOrders() const {return maxOrders;}

int LimitedDriverVolunteer::getNumOrdersLeft() const {return ordersLeft;}

bool LimitedDriverVolunteer::hasOrdersLeft() const {return ordersLeft > 0;}

bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {return (DriverVolunteer::canTakeOrder(order) and hasOrdersLeft());}

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    setDistanceLeft(order.getDistance());
    ordersLeft--;
}

string LimitedDriverVolunteer::toString() const{
    string output = "VolunteerID: " + std::to_string(getId()) +
                    "\nisBusy: " + std::string(this->isBusy() ? "True" : "False");
    if(isBusy()){
        output = output + 
                "\nOrderID: " + std::to_string(activeOrderId) +
                "\nTimeLeft: " + std::to_string(getDistanceLeft());
    }
    else{
        output = output + 
                "\nOrderID: None" +
                "\nTimeLeft: None";
    }

    output = output +
            "\nOrdersLeft: " + std::to_string(getNumOrdersLeft());

     return output;
 }

char LimitedDriverVolunteer::getTypeVolunteer() {return 'd';}
