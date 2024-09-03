# Food Warehouse Management System

The Food Warehouse Management System is a command-line simulation program designed to model warehouse operations. Volunteers manage orders from various customer types, demonstrating object-oriented programming principles and the "Rule of 5" in C++.

## Features

- **Order Management**: Handles the complete lifecycle of orders from placement to completion.
- **Volunteer Management**: Manages different volunteer types, including Collectors and Drivers with various capabilities.
- **Customer Management**: Supports multiple customer types, such as Soldiers and Civilians, with distinct needs and priorities.
- **Simulation Controls**: Allows users to simulate various scenarios and manage warehouse operations effectively.

## Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/YourUsername/Food-Warehouse-Management-System.git

2. **Navigate to the Project Directory:**
   ```bash
   cd Food-Warehouse-Management-System

2. **Compile the program:**:
   ```bash
     make

## Usage
1. **Run the program**:
   ./bin/warehouse <path_to_configuration_file>
   Replace <path_to_configuration_file> with the path to your configuration file. This file should include the initial setup for the warehouse.

2. **Configuration File Format:**
- **Volunteer**: `Volunteers - volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)`
- **Customer**: ` Customers - customer <customer_name> <customer_type> <customer_distance> <max_orders>`
`

   ### Example
     ```bash
      volunteer Nave limited_collector 3 2
      customer Moshe civilian 4 1
      volunteer Ofek limited_collector 2 2
      volunteer Yoav limited_driver 10 5 2
      volunteer Alon driver 8 3
    
3. **Interacting with the Program:** After starting the simulation, you can enter commands to place orders, check order statuses, and perform simulation steps. You can take a look at the assignment guidelines for a detailed description of available commands and their usage.

## Authors
- Nave Blander
- Yoav Schwammenthal


