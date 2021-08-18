# Desgin
![elevator](https://user-images.githubusercontent.com/57719752/129957554-d0df5454-54d0-493a-a3ea-3559c879e31c.gif)

#Description
In our faculty of engineering, building 3000 contains seven floors plus ground. An elevator system is used
for this building. It is controlled by a keypad inside the elevator that is used by the user to select the target
floor. The system should detect the entered floor number and moves a stepper motor according to the
current floor and the target floor values. Assume that one full motor revolution is required to move the
elevator by a single floor. That means a full clockwise revolution moves the elevator one floor up and a
full anticlockwise revolution moves the elevator one floor down. In addition, six push switches are used
to call the elevator and six 7 Segment BCD to display the current floor of the elevator (one switch and
one 7 segment per floor). The push switch should be pressed for 200 milliseconds at least. Weight sensor
is used to avoid overload. It measures the weight from 0 to 100 Kg and outputs a voltage that ranges
linearly from 0 to 5 V. It is replaced by an input voltage source. If the weight is more than 70 Kg the
motor should not move and a red led should be turned on. The motor should resume received commands
and the red led should be turned off after removing the extra weight (changing the input voltage source).
Any key could be pressed at any time either from the inside key pad or outside switches.
The elevator should respond to any series of requests in the minimum number of rotations. For example,
if it is at the start (at the ground floor) and the user pressed 4 at the inside keypad the motor should make
four full rotations to reach the fourth floor. While the motor movement, if someone pressed the “Call”
switch at the third floor then there are two cases:

1- If the motor rotated three or less full rotations, then it should stop after the third full rotation for
one second (assuming it is the time needed for this person to open the door and enter the
elevator). Then, it resumes the fourth rotation. In this case, it moved to the third floor then for the
fourth floor. The motor makes four rotations.

2- If the motor rotated more than three rotations. Then it currently passed the third floor. Therefore,
it should continue its rotations and stops for one second after the fourth rotation. Afterwards, it
rotates one anticlockwise rotation. In this case, it moved to the fourth floor then returned back to
the third floor. The motor makes five rotations.
Make the hardware and software design of this system using Arduino Uno. You can use any desired
dimensions of the keypad. You are allowed to use any number of resistors in addition to mentioned
devices (written in bold). In addition, you are allowed to use any number of only one type of gates.
This task is graded out of 10 based on the following points (one mark for each point)

1. The motor rotates the minimum number of rotations to fulfill the requests.
2. Red led working correctly (open / close)
3. The motor stop movement correctly with the weight sensor and resume the required orders after removing the weight without losing any order
4. The elevator responds to any pressed push switch more than 200 milliseconds
5. The elevator doesn’t responds to any pressed push switch less than 200 milliseconds
6. It discards the repeated key strokes at the internal keypad or any external push switch.
7. All seven segments are working correctly
8. You shouldn’t use any type of delay. Use millis
9. You should use at least one interrupt
10. No more than 20% more than the best fully working optimized code size (HEX file size in KB)
