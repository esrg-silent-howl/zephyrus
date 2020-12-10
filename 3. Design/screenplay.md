### **1.Introduction**

Good afternoon. We are group 7 and we are going to present the design for project 1. Its name is Zephyrus and its intent is to implement a wrist motion-controlled remote car.

### **2. Agenda**

Here is the agenda for the presentation.

- Problem Statement  
- System Overview
- Hardware and Software Architectures
- Hardware Specification
- Peripheral Interface
- Software Specification
  - Local System
  - Reinforcement Learning
  - Remote System
- Test Cases
  - Local System
  - Remote System
  - Integration
- Gantt Diagram  

### **3. Problem statement**

Motion-tracking and gesture-controlled robotics have been gaining popularity in various professional areas.  

With that in mind, this project intends to serve as a research and development tool for wearable-controlled technologies and will consist of a car, remotely controlled by a wristband using natural wrist and finger movements.  

### **4, 5, 6. System Overview**

This Figure depicts the system overview, where one can identify the main and remote boards and the ways in which they interact with each other and the external environment.

### **7. Hardware Architecture**

The Hardware for the  Local System includes: 

- A more capable main controller;
- Sensors for measuring proximity to close objects and positioning in the horizontal plane;
- Actuators for motor control ;
- A short-range wireless communications device;
- A power supply unit.

The Hardware for the Remote System includes: 

- An energy-efficient main controller;
- Sensors for measuring hand positioning;
- The counterpart for the short-range wireless communications device in the local system;
- A power supply unit.

### **8. Software Architecture**

The software architecture for both devices include high abstraction for communicating with each other and sensing modules, as well as power management and task scheduling. The local system, specifically, includes two high-level layers of abstraction for the Reinforcement Learning model.  

### **9. Hardware Specification: Local (Logic Elements)**

The main functional roles of the Local System are played by:

- A NUCLEO-F767ZI board, which hosts the main controller and all the components used to program it and make it work;
- A module containing an accelerometer/gyroscope IC that can be controlled through an I2C interface;
- A module containing a short-range radio IC and antenna instrumentation, for wireless communication;
- An board which contains an ultrasonic sensor and respective instrumentation;
- A very simple voltage level converter board to serve as **glue logic** between the main controller and the ultrasonic sensor board;
- A module containing a dual-motor driver IC.

### **10. Hardware Specification: Local (Power Supply)**

The elements that make up the power supply unit for the Local System are:

- A pair of Li-Ion batteries;
- A Battery Management System board;
- A 5V switching DC-DC step-down converter board.

### **11. Hardware Specification: Remote**

The hardware on the Remote System is comprised mainly by:

- An accelerometer and gyroscope module like the one on the Local System;
- An RF communication module that matches the one on the Local System;
- An exchangeable coin cell that serves as the main power supply for the system;
- A push button ON/OFF controller IC.

### **12. Hardware Specification: Custom Hardware (Prototype Alpha) SCH**

In order to develop for the remote system's microcontroller, a simple board was devised. This solution contains the minimum components and interfaces to be used by any microcontroller in the STM32F0 family of devices (with a QFP32 package). Additionally, it is simple enough to provide accurate real-world power profiling, necessary for alpha testing low-power applications, which is the case in this project.

The schematic for this board, designed in EAGLE CAD is represented in this image.

### **13. Hardware Specification: Custom Hardware (Prototype Alpha) BRD**

Here one can see the blueprints for that same board, with its dimensions and a glimpse of the outcome.

### **14. Hardware Specification: Custom Hardware (Prototype Beta) UC**

Following the alpha prototype, it was decided that there would be a beta prototype, containing all the integrated solutions that make up the wristband, into a slim and portable format.

With the aid of STM32CubeMX, one was able to plan out the pinout configuration and labelling for the Remote System's MCU.

### **15, 16. Hardware Specification: Custom Hardware (Prototype Beta) SCH**

Here are the initial schematics for the final wristband prototype, considering the predefined labels and separated by function into Main Logic and Power and Radio.  

### **17. Peripheral Interface: Common**

Here are the schematics documenting the interfaces between the microcontroller and the rest of the hardware. This one is for the hardware that is common to both systems...

### **18. Peripheral Interface: Specific**

... and this one is for hardware that is specific to one of the two.

### **19. Software Specification: Task Priority**

The thread pool for each system is represented in these diagrams, along with their relative priorities, statically defined. 

### **20. Software Specification: Task Timelines**

To ensure the meeting of critical timing constraints, a priority-based scheduling scheme with preemption was devised for each system. For both of them, one considered each task's worst case scenario in terms of processor time usage, as well as the power/efficiency trade-off. Dead time between task execution will be used to put the system asleep, favouring low power consumption.


### **21. Software Specification: Task Communication**

This is a diagram describing the means by which the tasks in each system interact with each other. These mechanisms satisfy what was laid out in the task timelines referring to inter-thread communication and synchronism.


### **22, 23, 24, 25. Software Specification: Threads**

As every other aspect of the project, the tasks' workflows also need a design stage. Here we can see the flowcharts for the most complex ones:

- The Local System's:
  - Gyroscope and accelerometer management task
  - RF communications management task
  - The inference task
- The Remote System's:
  - Gyroscope and accelerometer management task
  - RF communications management task


### **26. System Overview (Reinforcement Learning)**

The Local System will have a Reinforcement Learning module that is highlighted in this system overview.

### **27. System Overview (Reinforcement Learning)**

Zephyrus will rely on a feed-forward neural network that takes as input percentage speed and steer reference values, front and back measurements of distance to the nearest obstacle, and the environment state provided by the on-board sensors. With that in mind, the network will output the average percentage of the maximum voltage to be output to the motors. Percentage values were used to allow for portability to loads that require different working voltages.

### **28, 29. Test Cases**

At this stage, it is important to define a test strategy. Considering this, the tests were split into unit and integration tests taking into account alpha and beta prototypes.

### **30. Gantt Diagram**

The Gantt Diagram is presented in this slide and presents the tasks assigned to each week until the deadline.