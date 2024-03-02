import time
import csv
from serial import Serial
import os
print("Current Working Directory:", os.getcwd())


with open(r"C:\Users\zachg\Downloads\sketchyLidar\SensorData.csv", mode='a') as sensor_file:
    sensor_writer = csv.writer(sensor_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    sensor_writer.writerow(["Dist", "Angle"])


com = "COM5"
baud = 9600

x = Serial(com, baud, timeout = 0.1)

while x.isOpen() == True:
    data = str(x.readline().decode('utf-8')).rstrip()
    if data is not '':
         print(data)
         with open(r"C:\Users\zachg\Downloads\sketchyLidar\SensorData.csv", mode='a') as sensor_file:
             sensor_writer = csv.writer(sensor_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
             sensor_writer.writerow([data])
             sensor_file.flush()