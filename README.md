# TI-RSLK-Max-Wall-Sprint

Wall Sprint code for Spring 2022 EECS 195: Embedded Systems course final at University of California, Irvine. Won first place for the wall sprint. The objective of the wall sprint is to first cross the starting line, then sprint through a narrow lane bounded by electrical tape without going out of bounds, hit a wall, turn around, sprint through the same lane again, and finally stop when crossing the starting line the second time.

# Code Overview
## Starting Sprint:
The wall sprint starts when one of the bump sensors is triggered. While sprinting through the narrow race track lane, the reflectance sensors and bump sensors are turned on. 

When crossing the starting line the first time, lineCrossing and lineCrossed is set to 1. After crossing the line, lineCrossing is set back to 0. If the bump sensors are triggered before the robot crosses the starting line for the first time, it will stop and only resume when the bump sensor(s) are released.

## During the sprint:
If some of the reflectance sensors detect a black line but not all of them, that means the robot crossed the boundary line a bit. To prevent the robot from going off the track completely, an offset is added to one of the motor outputs to steer the robot back on track. 

Ex. Motor_Forward(forwardSpeed + leftSpeedOffset, forwardSpeed + rightSpeedOffset), leftSpeedOffset = 0 and rightSpeedOffset > 0. Since rightSpeedOffset is greater than 0, the right motor’s speed is increased and the robot steers more to the left.

## Hit Wall (Bump sensor are triggered):
When the robot hits the wall, the robot stops and moves backwards until the bumper sensor stops triggering. This is done to prepare the robot for the 180 degree turn. 

The tachometer function is turned on. This allows the robot to keep track of the number of wheel rotations done during the turn for each wheel. This allows the option to turn very quickly and with precision on how many degrees you want to turn.
The robot turns 180 degrees (depends on the max tachometer count value and tracktion of the tires) for the return run. 

## Return Run:
The tachometer is turned off because it is only used for the turning purpose and it uses a lot of computing. The return run functions the same as the initial sprint.

## Crossing the Starting Line the Second Time/ Finish Line:
When the starting line is crossed the second time, lineCrossed is set to 2 and the robot stops after 750ms.

# Test Run Video

[![Watch the video](https://img.youtube.com/vi/WNANxv6DUgE/maxresdefault.jpg)](https://www.youtube.com/shorts/WNANxv6DUgE)

# Acknowledgement

* Most of the libraries are done by completing the challenges in the TI-RSLK-MAX Solderless Maze Curriculum by Texas Instrument University. Their curriculum is available at their website https://university.ti.com/en/faculty/ti-robotics-system-learning-kit/ti-rslk-max-edition-curriculum.

* The Ultrasonic Sensor library code was based on a tutorial done by my instructor and his colleagues at https://www.hackster.io/125747/texas-instruments-rslk-with-ultrasonic-distance-sensors-7ee15d.

