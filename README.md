# Food Warehouse Management System

The system simulates the operations of a warehouse where volunteers handle orders made by different types of customers. It's designed as a command-line program using object-oriented programming principles, memory management, and the "Rule of 5" implementation in C++.

## Features

- Simulates the operations of a food warehouse, including handling orders, assigning volunteers, processing orders, and managing customer interactions.
- Supports different types of customers (Soldier Customer, Civilian Customer) and volunteers (Collector, Limited Collector, Driver, Limited Driver).
- Provides actions for placing orders, creating new customers, checking order status, performing simulation steps, and more.

## How to Run

To run the program, follow these steps:

1. Clone the repository to your local machine.
2. Navigate to the project directory in your terminal.
3. Compile the source files using the provided makefile:

```bash
make
```

4. Run the compiled executable with the path to the configuration file as the first command-line argument:

```bash
./bin/warehouse <path_to_configuration_file>
```

Replace `<path_to_configuration_file>` with the path to the configuration file containing the initial state of the warehouse.

## Example Configuration File

The configuration file should contain the initial setup of the warehouse, including customers and volunteers. Each line in the file represents either a customer or a volunteer, following the specified format. Here's an example:

```
volunteer Nave limited_collector 3 2
customer Moshe civilian 4 1
volunteer Ofek limited_collector 2 2
volunteer Yoav limited_driver 10 5 2
volunteer Alon driver 8 3
```

## Usage

Upon startup, the program initializes the warehouse based on your configuration file. It begins the simulation and prompts you to enter commands for various actions such as placing orders, checking statuses, simulating steps, and more. The available actions are described in the assignment guidelines.


## Authors

- Nave Blander
- Yoav Schwammenthal 

---
