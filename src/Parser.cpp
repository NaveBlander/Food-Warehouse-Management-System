#include "Parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Customer.h"
#include "Volunteer.h"

Parser::Parser() :
    customers(),
    volunteers(),
    customerCounter(0),
    volunteerCounter(0) {}

void Parser::parseConfigFile(const std::string& configFilePath) {
    std::ifstream file(configFilePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "customer") {
            parseCustomerLine(line);
        } else if (type == "volunteer") {
            parseVolunteerLine(line);
        }
    }
}

void Parser::parseCustomerLine(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name, customerType;
    int distance, maxOrders;

    iss >> type >> name >> customerType >> distance >> maxOrders;

    Customer* customer = nullptr;
    if (customerType == "soldier") {
        customer = new SoldierCustomer(customerCounter++, name, distance, maxOrders);
    } else if (customerType == "civilian") {
        customer = new CivilianCustomer(customerCounter++, name, distance, maxOrders);
    }

    if (customer != nullptr) {
        customers.push_back(customer);
    }
}

void Parser::parseVolunteerLine(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name, volunteerType;
    int coolDown, maxDistance, distancePerStep, maxOrders;

    iss >> type >> name >> volunteerType;

    Volunteer* volunteer = nullptr;
    if (volunteerType == "collector") {
        iss >> coolDown;
        volunteer = new CollectorVolunteer(volunteerCounter++, name, coolDown);
    } else if (volunteerType == "limited_collector") {
        iss >> coolDown >> maxOrders;
        volunteer = new LimitedCollectorVolunteer(volunteerCounter++, name, coolDown, maxOrders);
    } else if (volunteerType == "driver") {
        iss >> maxDistance >> distancePerStep;
        volunteer = new DriverVolunteer(volunteerCounter++, name, maxDistance, distancePerStep);
    } else if (volunteerType == "limited_driver") {
        iss >> maxDistance >> distancePerStep >> maxOrders;
        volunteer = new LimitedDriverVolunteer(volunteerCounter++, name, maxDistance, distancePerStep, maxOrders);
    }

    if (volunteer != nullptr) {
        volunteers.push_back(volunteer);
    }
}

std::vector<Customer*>& Parser::getCustomers() {
    return customers;
}

std::vector<Volunteer*>& Parser::getVolunteers() {
    return volunteers;
}
