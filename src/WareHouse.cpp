#include "WareHouse.h"
#include "Parser.h"
#include "Action.h"
#include "Volunteer.h"
#include "Order.h"
#include <iostream>
#include <sstream>
#include <string>


// Constructor
WareHouse::WareHouse(const string &configFilePath) 
    : isOpen(true),
      actionsLog(),
      volunteers(),
      pendingOrders(),
      inProcessOrders(),
      completedOrders(),
      customers(),
      customerCounter(0),
      volunteerCounter(0),
      dummyOrder(new Order(-1, -1, 0)){

    Parser parser;
    parser.parseConfigFile(configFilePath);

    customerCounter = parser.getCustomers().size();
    volunteerCounter = parser.getVolunteers().size();

    volunteers = parser.getVolunteers();
    customers = parser.getCustomers();
}

// Copy-Constructor
WareHouse::WareHouse(const WareHouse &other) :
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    dummyOrder(other.dummyOrder->clone()){

    for (BaseAction* action : other.actionsLog){
        actionsLog.push_back(action->clone());
    }

    for (Volunteer* volunteer : other.volunteers){
        volunteers.push_back(volunteer->clone());
    }

    for (Order* order : other.pendingOrders){
        pendingOrders.push_back(order->clone());
    }

    for (Order* order : other.inProcessOrders){
        inProcessOrders.push_back(order->clone());
    }

    for (Order* order : other.completedOrders){
        completedOrders.push_back(order->clone());
    }

    for (Customer* customer : other.customers){
        customers.push_back(customer->clone());
    }
}

// Destructor
WareHouse::~WareHouse(){
    cleanVectors();
    delete dummyOrder;
}

// Copy-Assignment-Operator
WareHouse& WareHouse::operator=(const WareHouse& other){
    if(&other != this){
        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        cleanVectors();
        delete this->dummyOrder;
        this->dummyOrder = other.dummyOrder->clone();

        for (BaseAction* action : other.actionsLog){
            actionsLog.push_back(action->clone());
        }

        for (Volunteer* volunteer : other.volunteers){
            volunteers.push_back(volunteer->clone());
        }

        for (Order* order : other.pendingOrders){
            pendingOrders.push_back(order->clone());
        }

        for (Order* order : other.inProcessOrders){
            inProcessOrders.push_back(order->clone());
        }

        for (Order* order : other.completedOrders){
            completedOrders.push_back(order->clone());
        }

        for (Customer* customer : other.customers){
            customers.push_back(customer->clone());
        }   
    }
    return *this;
}


// Move 1
WareHouse::WareHouse(WareHouse&& other) noexcept :
    isOpen(other.isOpen),
    actionsLog(),
    volunteers(),
    pendingOrders(),
    inProcessOrders(),
    completedOrders(),
    customers(),
    customerCounter(other.customerCounter),
    volunteerCounter(other.volunteerCounter),
    dummyOrder(other.dummyOrder){

        cleanVectors();

        for (BaseAction* action : other.actionsLog){
            actionsLog.push_back(action);
        }

        for (Volunteer* volunteer : other.volunteers){
            volunteers.push_back(volunteer);
        }

        for (Order* order : other.pendingOrders){
            pendingOrders.push_back(order);
        }

        for (Order* order : other.inProcessOrders){
            inProcessOrders.push_back(order);
        }

        for (Order* order : other.completedOrders){
            completedOrders.push_back(order);
        }

        for (Customer* customer : other.customers){
            customers.push_back(customer);
        }   
        
        other.actionsLog.clear();
        other.volunteers.clear();
        other.pendingOrders.clear();
        other.inProcessOrders.clear();
        other.completedOrders.clear();
        other.customers.clear();

        other.dummyOrder = nullptr;
}

// Move 2
WareHouse& WareHouse::operator=(WareHouse&& other) noexcept{

    if (this != &other) {
        cleanVectors(); // Clean up existing resources
        delete dummyOrder;

        isOpen = other.isOpen;
        customerCounter = other.customerCounter;
        volunteerCounter = other.volunteerCounter;
        actionsLog = std::move(other.actionsLog);
        volunteers = std::move(other.volunteers);
        pendingOrders = std::move(other.pendingOrders);
        inProcessOrders = std::move(other.inProcessOrders);
        completedOrders = std::move(other.completedOrders);
        customers = std::move(other.customers);
        dummyOrder = other.dummyOrder;

        // Set the source object's members to default values
        other.isOpen = false;
        other.customerCounter = 0;
        other.volunteerCounter = 0;
        other.dummyOrder = nullptr;
    }
    return *this;
}

void WareHouse::cleanVectors() {
    for(BaseAction* action : actionsLog){
        delete action;
    }
    actionsLog.clear();

    for (Volunteer* volunteer : volunteers){
        delete volunteer;
    }
    volunteers.clear();

    for (Order* order : pendingOrders){
        delete order;
    }
    pendingOrders.clear();

    for (Order* order : inProcessOrders){
        delete order;
    }
    inProcessOrders.clear();

    for (Order* order : completedOrders){
        delete order;
    }
    completedOrders.clear();

    for (Customer* customer : customers){
        delete customer;
    }
    customers.clear();
}

void WareHouse::start(){
    std::cout << "Warehouse is open!" << std::endl;
    while (isOpen)
    {
        BaseAction* action;
        string userInput;
        string actionOfUser;

        // Read a line of input from the user
        std::getline(std::cin, userInput);

        // Use stringstream to extract the first word
        std::stringstream ss(userInput);
        ss >> actionOfUser;
        
        if(actionOfUser == "step"){
            int numOfSteps;
            ss >> numOfSteps;
            action = new SimulateStep(numOfSteps);
        }

        else if(actionOfUser == "customer"){
            string customerName;
            string customerType;
            int distance;
            int maxOrders;
    
            ss >> customerName >> customerType >> distance >> maxOrders;
            
            action = new AddCustomer(customerName, customerType, distance, maxOrders);
        }

        else if(actionOfUser == "order"){
            int customerId;
            ss >> customerId;
            action = new AddOrder(customerId);
        }

        else if(actionOfUser == "orderStatus"){
            int orderId;
            ss >> orderId;
            action = new PrintOrderStatus(orderId);
        }

        else if(actionOfUser == "customerStatus"){
            int customerId;
            ss >> customerId;
            action = new PrintCustomerStatus(customerId);
        }

        else if(actionOfUser == "volunteerStatus"){
            int volunteerId;
            ss >> volunteerId;
            action = new PrintVolunteerStatus(volunteerId);
        }

        else if(actionOfUser == "log"){
            action = new PrintActionsLog();
        }

        else if(actionOfUser == "close"){
            action = new Close();
        }

        else if(actionOfUser == "backup"){
            action = new BackupWareHouse();
        }

        else if(actionOfUser == "restore"){
            action = new RestoreWareHouse();
        }

        action->act(*this);
        actionsLog.push_back(action);
    }
}

void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
}

void WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}

Customer& WareHouse::getCustomer(int customerId) const{
    return *(customers.at(customerId));
}

Volunteer& WareHouse::getVolunteer(int volunteerId) const{
    return *(volunteers.at(volunteerId));
}

Order& WareHouse::getOrder(int orderId) const{    
    for(Order* order : pendingOrders){
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for(Order* order : inProcessOrders){
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    for(Order* order : completedOrders){
        if (order->getId() == orderId)
        {
            return *order;
        }
    }
    return *dummyOrder; // We know that whoever calls this function, knows if the ordeId it sent exist, so we will never actualy return this dummy order, it is here just to handel the compiler warning
}

const vector<BaseAction*>& WareHouse::getActions() const{return actionsLog;}

void WareHouse::close(){
    isOpen = false;
    
}
void WareHouse::open(){isOpen = true;}
vector<Order*>& WareHouse::getPendingOrders() {return pendingOrders;}
vector<Order*>& WareHouse::getInProcessOrders() {return inProcessOrders;}
vector<Order*>& WareHouse::getCompletedOrders() {return completedOrders;}
vector<Volunteer*>& WareHouse::getVolunteers() {return volunteers;}
vector<Customer*>& WareHouse::getCustomers() {return customers;}
void WareHouse::increaseCustCount() {customerCounter++;}
int WareHouse::getCustCounter() {return customerCounter;}
int WareHouse::getOrdersCounter() {
    return pendingOrders.size() + inProcessOrders.size() + completedOrders.size();
}

int WareHouse::getVolunteerCounter() const{
    return volunteerCounter;
}


