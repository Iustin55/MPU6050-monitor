# MPU6050-monitor
Connect an Arduino with a MPU6050 sensor attached and analyze the serial output using a Python script

For the Arduino, the Wire library is required for the I2C communication.
For the Python script, the matplotlib and serial packages are required.

The communication between the two components has an annoying delay currently, I'm working on a solution for this by reading the sensor data using a separate thread. Matplotlib is so slow...
