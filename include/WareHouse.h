#pragma once
#include <string>
#include <vector>
using namespace std;

#include "Order.h"
#include "Customer.h"

class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        WareHouse(const WareHouse &other); // We added
        ~WareHouse(); // We added
        WareHouse& operator=(const WareHouse& other); // We added
        WareHouse(WareHouse&& other) noexcept; // We added
        WareHouse& operator=(WareHouse&& other) noexcept; // We added

        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        vector<Order*> &getPendingOrders();
        vector<Order*> &getInProcessOrders();
        vector<Order*> &getCompletedOrders();
        vector<Volunteer*> &getVolunteers();
        vector<Customer*> &getCustomers();
        void increaseCustCount();
        int getCustCounter();
        int getOrdersCounter();
        int getVolunteerCounter() const;

        void cleanVectors(); //We added


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        Order *dummyOrder; // We added
};