#include "Order.h"

Order::Order(int id, int customerId, int distance) :
    id(id),
    customerId(customerId),
    distance(distance),
    status(OrderStatus::PENDING),
    collectorId(NO_VOLUNTEER),
    driverId(NO_VOLUNTEER) {}

Order* Order::clone() const {
    return new Order(*this);
}

int Order::getId() const {return id;}

int Order::getCustomerId() const {return customerId;}

void Order::setStatus(OrderStatus status) {this->status = status;}

void Order::setCollectorId(int collectorId) {this->collectorId = collectorId;}

void Order::setDriverId(int driverId) {this->driverId = driverId;}

int Order::getCollectorId() const {return collectorId;}

int Order::getDriverId() const {return driverId;}

int Order::getDistance() const {return distance;}

OrderStatus Order::getStatus() const {return status;}

const string Order::toString() const {
    string output = "OrderId: " + std::to_string(getId()) +
                    "\nOrderStatus: " + orderStatusToString() +
                    "\nCustomerID: " + std::to_string(customerId);
    if(orderStatusToString() == "Pending"){
        output = output + "\nCollector: None";
    }
    else{
        output = output + "\nCollector: " + std::to_string(collectorId);
    }

    if(orderStatusToString() == "Delivering" || orderStatusToString() == "Completed"){
        output = output + "\nDriver: " + std::to_string(getDriverId());
    }
    else{
        output = output + "\nDriver: None";
    }
    
    return output;
}

string Order::orderStatusToString() const{
    if (status == OrderStatus::PENDING) {return "Pending";}
    else if (status == OrderStatus::COLLECTING) {return "Collecting";}
    else if (status == OrderStatus::DELIVERING) {return "Delivering";}
    else {return "Completed";}
}
