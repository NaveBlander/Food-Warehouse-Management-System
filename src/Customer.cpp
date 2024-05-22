#include "Customer.h"

Customer::Customer(int id, const string &name, int locationDistance, int maxOrders) :
    id(id), 
    name(name), 
    locationDistance(locationDistance),
    maxOrders(maxOrders),
    ordersId(){}

Customer::Customer(const Customer& other)
    : id(other.id), name(other.name), locationDistance(other.locationDistance), 
      maxOrders(other.maxOrders), ordersId(other.ordersId) {
    // Additional copy logic, if necessary
}

const string& Customer::getName() const {return name;}

int Customer::getId() const {return id;}

int Customer::getCustomerDistance() const {return locationDistance;}

int Customer::getMaxOrders() const {return maxOrders;}

int Customer::getNumOrders() const {return ordersId.size();}

bool Customer::canMakeOrder() const {return (static_cast<int>(ordersId.size()) != maxOrders);}

const vector<int>& Customer::getOrdersIds() const {return ordersId;}

int Customer::addOrder(int orderId){
    int ret = -1;
    if(canMakeOrder()){
        ordersId.push_back(orderId);
        ret = orderId;
    }
    return ret;
}

SoldierCustomer::SoldierCustomer(int id, string name, int locationDistance, int maxOrders) :
    Customer(id, name, locationDistance, maxOrders) {}

SoldierCustomer* SoldierCustomer::clone() const {return new SoldierCustomer(*this);}

CivilianCustomer::CivilianCustomer(int id, string name, int locationDistance, int maxOrders) :
    Customer(id, name, locationDistance, maxOrders) {}

CivilianCustomer* CivilianCustomer::clone() const {return new CivilianCustomer(*this);}
