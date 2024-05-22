#include "Action.h"
#include "Volunteer.h"
#include <iostream>
#include "Order.h"
extern WareHouse* backup;

// 1. BaseAction --------------------------------------------------------------------------------------------------------------------------------------------------------------

BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}


ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete() {status = ActionStatus::COMPLETED;}

void BaseAction::error(string errorMsg) {
    status = ActionStatus::ERROR; 
    this -> errorMsg = errorMsg;
    std::cout << errorMsg << std::endl;
}

string BaseAction::getErrorMsg() const {return errorMsg;}

string BaseAction::actionStatusToString() const{
    if (status == ActionStatus::COMPLETED) {return "COMPLETED";}
    else {return "ERROR";}
}


// 2. SimulateStep ------------------------------------------------------------------------------------------------------------------------------------------------------------

SimulateStep::SimulateStep(int numOfSteps) : 
    BaseAction(),
    numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    int stepCounter = numOfSteps;
    while (stepCounter > 0) {
        makeOneStep(wareHouse);
        stepCounter--;
    }
}

void SimulateStep::makeOneStep(WareHouse &wareHouse) {

    //Stage 1 in the scheme 

    vector<Order*> &pendingOrders = wareHouse.getPendingOrders();
    vector<Volunteer*> &volunteers = wareHouse.getVolunteers();

    for (size_t k = 0; k < pendingOrders.size(); k++)
    {
        Order* order = pendingOrders.at(k);
        bool volunteerFound = false;

        if(order->getStatus() == OrderStatus::PENDING)
        {
            //PASS TO COLLECTOR
            for (size_t i = 0; i < volunteers.size() && !volunteerFound; i++)
            {
                Volunteer* vol = volunteers[i];
                if ((vol->getTypeVolunteer() == 'c' || vol->getTypeVolunteer() == 'C') &&
                    vol->canTakeOrder(*order))
                {
                    vol->acceptOrder(*order);
                    volunteerFound = true;
                    order->setCollectorId(vol->getId());
                    order->setStatus(OrderStatus::COLLECTING);
                    wareHouse.getInProcessOrders().push_back(order);
                    pendingOrders[k] = nullptr;
                }
            }  
        }

        else if(order->getStatus() == OrderStatus::COLLECTING)
        {
            //PASS TO DRIVER
            for (size_t i = 0; i < volunteers.size() && !volunteerFound; i++)
            {
                Volunteer* vol = volunteers[i];
                if ((vol->getTypeVolunteer() == 'd' || vol->getTypeVolunteer() == 'D') &&
                    vol->canTakeOrder(*order))
                {
                    vol->acceptOrder(*order);
                    volunteerFound = true;
                    order->setDriverId(vol->getId());
                    order->setStatus(OrderStatus::DELIVERING);
                    wareHouse.getInProcessOrders().push_back(order);
                    pendingOrders[k] = nullptr;
                }
            }  
        }
    }

    //Remove all nullptr (orders which are not more 'pending') from the pendingOrder vector
    for (size_t i = 0; i < pendingOrders.size(); i++)
    {
        if(pendingOrders[i] == nullptr){
            pendingOrders.erase(pendingOrders.begin() + i);
            i--;
        }
    }

    //Stage 2 + 3 + 4 in the scheme


    for (int i = volunteers.size() - 1; i >= 0; --i) {

        Volunteer* vol = volunteers.at(i);
        int preComplectedOrderId = vol->getCompletedOrderId();
        
        if(vol->getActiveOrderId() != NO_ORDER){

            vol->step();
            const int postComplectedOrderId = vol->getCompletedOrderId();

            if(preComplectedOrderId != postComplectedOrderId){

                Order& order = wareHouse.getOrder(postComplectedOrderId);

                if(vol->getTypeVolunteer() =='c' || vol->getTypeVolunteer() == 'C'){
                    //PUSH INTO PENDING VECTOR
                    order.setStatus(OrderStatus::COLLECTING);
                    (wareHouse.getPendingOrders()).push_back(&order);
                }

                if(vol->getTypeVolunteer() =='d' || vol->getTypeVolunteer() == 'D'){
                    //PUSH INTO COMPLECTED VECTOR
                    order.setStatus(OrderStatus::COMPLETED);
                    (wareHouse.getCompletedOrders()).push_back(&order);
                }

                //REMOVE FROM IN-PROCESS VECTOR
                for (int k = wareHouse.getInProcessOrders().size() - 1; k >= 0; --k){
                        if(wareHouse.getInProcessOrders().at(k)->getId() == postComplectedOrderId){
                            wareHouse.getInProcessOrders().erase(wareHouse.getInProcessOrders().begin() + k);
                        }
                }

                if (!vol->hasOrdersLeft()){
                    delete vol;
                    volunteers.erase(volunteers.begin() + i);  // Remove the pointer from the vector
                }
            }
        }
    }
}

string SimulateStep::toString() const {
    return "simulateStep " + std::to_string(numOfSteps) + " " + actionStatusToString();
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);   
}

// Add Customer ----------------------------------------------------------------------------------------------------------------------------------------------------------------

AddCustomer::AddCustomer(const string &customerName,const string &customerType, int distance, int maxOrders) :
    BaseAction(),
    customerName(customerName),
    customerType(stringToCustomerType(customerType)),
    distance(distance),
    maxOrders(maxOrders) {}

void AddCustomer::act(WareHouse &wareHouse) {
    wareHouse.increaseCustCount();

    //Create new customer
    Customer* customerToAdd;
    if(customerType == CustomerType::Civilian){
        customerToAdd = new CivilianCustomer
            (wareHouse.getCustCounter(),
            this->customerName,
            this->distance,
            this->maxOrders);
    }
    else{
        customerToAdd = new SoldierCustomer
            (wareHouse.getCustCounter(),
            this->customerName,
            this->distance,
            this->maxOrders);
    }

    //Updtae the warhouse customers
    wareHouse.getCustomers().push_back(customerToAdd);
    complete();
}

AddCustomer* AddCustomer::clone() const {return new AddCustomer(*this);}

string AddCustomer::customerTypeToString() const {
    if(customerType == CustomerType::Civilian) {return "civilian";}
    else{return "soldier";}
}

string AddCustomer::toString() const {
    return "customer " + customerName + " " + customerTypeToString() + " " + std::to_string(distance) + " " + std::to_string(maxOrders); 
}

CustomerType AddCustomer::stringToCustomerType(const string& CustomerType){
    if(CustomerType == "Soldier" || CustomerType == "soldier"){return CustomerType::Soldier;} 
    else{return CustomerType::Civilian;}
}



// Add Order -------------------------------------------------------------------------------------------------------------------------------------------------------------------

AddOrder::AddOrder(int id) :
    BaseAction(),
    customerId(id) {}

void AddOrder::act(WareHouse &wareHouse) {

    //Check if customer's id is valid, and if so, check if he can make order
    if(wareHouse.getCustCounter() > customerId && customerId >= 0 &&
        (wareHouse.getCustomer(customerId).canMakeOrder())){
        
        //Create an order (its fields as well)
        Order* order = new Order(wareHouse.getOrdersCounter(), customerId, wareHouse.getCustomer(this->customerId).getCustomerDistance());

        //Update customer's ordersId vector
        wareHouse.getCustomer(customerId).addOrder(order->getId());

        //Update the warhouse orders (pending oders.)
        wareHouse.addOrder(order);
        complete();
    }
    else{
         error("Error: Cannot place this order");
    }
}

AddOrder* AddOrder::clone() const {return new AddOrder(*this);}

string AddOrder::toString() const {
    return "order " + std::to_string(customerId) + " " + actionStatusToString();
}


// Print Order Status ---------------------------------------------------------------------------------------------------------------------------------------------------------

PrintOrderStatus::PrintOrderStatus(int id) :
    BaseAction(),
    orderId(id){}

void PrintOrderStatus::act(WareHouse &wareHouse) {

    if(wareHouse.getOrdersCounter() <= orderId || orderId < 0){
        error("Error: Order doesn’t exist");
    }
    else{
        std::cout << wareHouse.getOrder(orderId).toString() << std::endl;
        complete();
    }
}

string PrintOrderStatus::toString() const {
    return "orderStatus " + std::to_string(orderId) + " " + actionStatusToString();
}

PrintOrderStatus* PrintOrderStatus::clone() const {return new PrintOrderStatus(*this);}

// Print Customer Status --------------------------------------------------------------------------------------------------------------------------------------------------------

PrintCustomerStatus::PrintCustomerStatus(int customerId) :
    BaseAction(),
    customerId(customerId) {}

void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(customerId > wareHouse.getCustCounter() || customerId < 0){
        error("Customer doesn’t exist");
    }
    else{
        Customer& customer = wareHouse.getCustomer(customerId);
        std::cout << "CustomerID: " << customerId << std::endl;
        for (size_t i = 0; i < customer.getOrdersIds().size(); i++)
        {
            int order_id = customer.getOrdersIds().at(i);
            std::cout << "OrderId: " << order_id << std::endl;
            std::cout << "OrderStatus: " << wareHouse.getOrder(order_id).orderStatusToString() << std::endl;
        }
        std::cout << "numOrdersLeft: " << customer.getMaxOrders() - customer.getNumOrders() << std::endl;
        complete();
    }
}

string PrintCustomerStatus::toString() const {
    return "customerStatus " + std::to_string(customerId) + " " + actionStatusToString();
}

PrintCustomerStatus* PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}

// Print Volunteer Status -------------------------------------------------------------------------------------------------------------------------------------------------------

PrintVolunteerStatus::PrintVolunteerStatus(int id) :
    BaseAction(),
    volunteerId(id) {}

void PrintVolunteerStatus::act(WareHouse &wareHouse){
    if(volunteerId < 0 || volunteerId > wareHouse.getVolunteerCounter()-1){
        error("Volunteer doesn't exist");
    }
    else{
        std::cout << wareHouse.getVolunteer(volunteerId).toString() << std::endl;
        complete();
    }
}

string PrintVolunteerStatus::toString() const {
    return "volunteerStatus " + std::to_string(volunteerId) + " " + actionStatusToString();
}

PrintVolunteerStatus* PrintVolunteerStatus::clone() const {return new PrintVolunteerStatus(*this);}

// Print Action Log ------------------------------------------------------------------------------------------------------------------------------------------------------------

PrintActionsLog::PrintActionsLog() : BaseAction() {}

void PrintActionsLog::act(WareHouse &wareHouse) {
    for(BaseAction* action : wareHouse.getActions()){
        std::cout << action->toString()<< std::endl;
    }
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

string PrintActionsLog::toString() const {
    return "log " + actionStatusToString(); 
}

// Close -------------------------------------------------------------------------------------------------------------------------------------------

    Close::Close() : BaseAction() {}

    void Close::act(WareHouse &wareHouse) {
        for(Order* order : wareHouse.getPendingOrders()){
            std::cout << "OrderID: " + std::to_string(order->getId()) + 
                        ", CustomerID: " + std::to_string(order->getCustomerId()) +
                        ", Status: " + order->orderStatusToString() << std::endl;
        }

        for(Order* order : wareHouse.getCompletedOrders()){
            std::cout << "OrderID: " + std::to_string(order->getId()) + 
                        ", CustomerID: " + std::to_string(order->getCustomerId()) +
                        ", Status: " + order->orderStatusToString() << std::endl;
        }

        for(Order* order : wareHouse.getInProcessOrders()){
            std::cout << "OrderID: " + std::to_string(order->getId()) + 
                        ", CustomerID: " + std::to_string(order->getCustomerId()) +
                        ", Status: " + order->orderStatusToString() << std::endl;
        }

        wareHouse.close();
    }

    Close* Close::clone() const {
        return new Close(*this);
    }

    string Close::toString() const {
        return "close" + actionStatusToString();
    }

// Backup Warehouse ---------------------------------------------------------------------------------------------------------------------------------------------------------------


BackupWareHouse::BackupWareHouse() : BaseAction() {}

void BackupWareHouse::act(WareHouse &wareHouse) {

    if(backup == nullptr){
        backup = new WareHouse(wareHouse);
    }else{
        *backup = wareHouse;
    }
    complete();
}

BackupWareHouse* BackupWareHouse::clone() const {
    return new BackupWareHouse(*this);
}

string BackupWareHouse::toString() const{
    return "backupWarehouse " + actionStatusToString();
}

// Restore Warehoue ------------------------------------------------------------------------------------------------------------------------------------------------------------------

RestoreWareHouse::RestoreWareHouse() : BaseAction() {}

void RestoreWareHouse::act(WareHouse &wareHouse){
    if(backup == nullptr){
        error("Error: No backup available");
    } else{
        wareHouse = *backup;
        complete();
    }
}

RestoreWareHouse* RestoreWareHouse::clone() const{
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse::toString() const{
    return "restoreWarehouse " + actionStatusToString();
}