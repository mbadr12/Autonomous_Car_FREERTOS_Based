# Autonomous Car with Obstacle Avoidance and Light Swinging

This is a README file for an autonomous car system that utilizes FreeRTOS to implement obstacle avoidance and light-swinging functionalities. The system includes a Tiva C controller, a car chassis, two LDR sensors, an ultrasonic sensor, a battery, and an LCD display. The LCD display shows various information such as temperature, elapsed time, distance, and the difference between LDR sensor readings. 

## System Overview

The autonomous car system is designed to navigate its environment while avoiding obstacles and swinging its lights. It uses the following components:

1. Tiva C Controller: The Tiva C microcontroller acts as the brain of the system, coordinating the inputs from various sensors and controlling the car's movements and light swinging.

2. Car Chassis: The car chassis provides the physical platform for the system and houses all the necessary components.

3. LDR Sensors: Two Light Dependent Resistor (LDR) sensors are used to detect light levels. The difference between their readings is displayed on the LCD.

4. Ultrasonic Sensor: The ultrasonic sensor measures the distance between the car and obstacles in its path. It helps the car to avoid collisions.

5. Battery: A battery powers the entire system, providing the necessary electrical energy for the car's operation.

6. LCD Display: The LCD display module shows real-time information such as temperature, elapsed time, distance from obstacles, and the difference between LDR sensor readings.

## Functionality

The autonomous car system incorporates the following functionalities:

1. Start Button: Pressing the start button initializes the system and allows the car to start moving autonomously.

2. Stop Button: Pressing the stop button immediately halts the car's movement and terminates the system.

3. Time Limit: If the stop button is not pressed, the system will automatically stop after 60 seconds to prevent extended operation.

4. Obstacle Avoidance: The ultrasonic sensor continuously measures the distance between the car and obstacles. If an obstacle is detected within a specified range, the car will autonomously change its direction to avoid a collision.

5. Light Swinging: The car's lights will swing back and forth as it moves, adding a dynamic visual effect.

6. LCD Display Information:
   - Temperature: The system measures and displays the current temperature.
   - Elapsed Time: The LCD shows the time elapsed since the system started.
   - Distance: The distance between the car and the nearest obstacle is displayed.
   - LDR Difference: The difference between the readings from the two LDR sensors is shown.

## Installation and Setup

To set up and run the autonomous car system, follow these steps:

1. Hardware Connections: Connect the Tiva C controller, LDR sensors, ultrasonic sensor, battery, and LCD display to the appropriate pins and interfaces, following the hardware specifications and pin mappings.

2. Software Dependencies: Install the necessary software dependencies, including FreeRTOS and the required libraries for Tiva C controller programming.

3. Code Compilation: Compile and upload the provided source code to the Tiva C controller using your preferred programming environment.

4. Start and Stop Buttons: Connect the start and stop buttons to the Tiva C controller's GPIO pins. Ensure that they are correctly wired and function as intended.

5. Power On: Power on the system by connecting the battery to the appropriate power supply.

6. LCD Display: Check that the LCD display is functioning correctly and that all the displayed information is visible.

7. Operation: Press the start button to initiate the system. The car should start moving autonomously, avoiding obstacles and swinging its lights. Press the stop button at any time to halt the car's movement and terminate the system.

## Troubleshooting

If you encounter any issues while setting up or running the autonomous car system, consider the following troubleshooting steps:

1. Hardware Connections: Verify that all the hardware components are properly connected according to the specifications and pin mappings.

2. Power Supply: Ensure that the battery is fully charged and providing sufficient power to all the system components.

3. Code Verification: Double-check the uploaded code for any errors or compatibility issues with the Tiva C controller and FreeRTOS.

4. Button Functionality: Test the start and stop buttons for proper functionality. Check the wiring and connections to the Tiva C controller.

5. Sensor Calibration: Verify that the ultrasonic sensor and LDR sensors are calibrated correctly for accurate readings.

6. LCD Display: Make sure the LCD display is properly connected and configured. Check for any display or communication issues.

7. Debugging: Utilize debugging tools and techniques to identify any runtime errors or logic issues in the code. Monitor the console output for any error messages or unexpected behavior.

## Conclusion

The autonomous car system with obstacle avoidance, light swinging, and FreeRTOS implementation provides an exciting demonstration of autonomous vehicle capabilities. By following the installation and setup instructions, you can create an enjoyable and educational project that showcases the integration of various sensors and control mechanisms. Feel free to customize and enhance the system based onyour specific requirements and preferences. Happy exploring and experimenting with your autonomous car!
