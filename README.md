# ESP Access Point

In this project, I used ESP8266 as an access point with an interface to control robot movement using JSON.
<br>



# Method of Work

After running the ESP, I can connect to its network using wifi
Then, I can open an html page which has five buttons: Forward, Right, Stop, Left, and Backward.
Using javascript, each button will return a value of '1' and using JSON I sent all directions values to the ESP.

I used the serial monitor to display the output, it could be replaced to move the robot forward, for example if we used DC motors with H-Bridge driver: <br>

```C++
robot.rotate(motor1, 60, CW);
robot.rotate(motor2, 60, CW);
```
<br>
# Working Example

<br>



https://user-images.githubusercontent.com/85786699/128396145-0484f069-4005-493b-9c41-6a0ba5cc2185.mp4

