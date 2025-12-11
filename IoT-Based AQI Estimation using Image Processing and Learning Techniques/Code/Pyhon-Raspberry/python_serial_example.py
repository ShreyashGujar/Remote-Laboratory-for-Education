# Importing Libraries
import serial
import time
import struct
from gpiozero import LED
sel0 = LED(16)
sel1 = LED(20)
en = LED(21)
time.sleep(1)
en.off()
sel0.off()
sel1.off()
time.sleep(1)

# arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=9600, timeout=.1)
arduino = serial.Serial(port='/dev/serial0', baudrate=9600, timeout=.1)
send_array_read =[0xAA,0x02,0x00,0x00,0x00,0x00,0x01,0x67,0xBB]
send_array_boot =[0xAA,0x01,0x00,0x00,0x00,0x00,0x01,0x66,0xBB]
send_array_shutdown =[0xAA,0x03,0x00,0x00,0x00,0x00,0x01,0x68,0xBB]
# send_array = chr(send_array)
def write_read(x):
#     arduino.write(bytes(x, 'utf-8'))
    arduino.write(x)
    data = arduino.readline()
#     .decode('utf-8').rstrip()
    return data

def read_pm():
    num = send_array_boot
    value = write_read(num)
    values1 =list(value)
#     print(values1) # printing the value
    time.sleep(2)

    num = send_array_read
    value = write_read(num)
    values2 =list(value)
#     print(values2) # printing the value
    time.sleep(1)

    num = send_array_shutdown
    value = write_read(num)
    values3 =list(value)
#     print(values) # printing the value
    time.sleep(1)
    return values2

def validate_data(data):
#     print(data)
    data[2] = data[2]*256
    data[4] = data[4]*256
    data[6] = data[6]*256
#     print(data)
    sum_r = data[6]+data[7]
    checksum = sum(data) - sum_r
#     print(sum_r)
#     print(checksum)
    if(sum_r==checksum):
        result = []
        pm10 = data[2] + data[3]
        pm25 = data[4] + data[5]
        result.append(pm25)
        result.append(pm10)
        print("PM2.5: ",pm25)
        print("PM10: ",pm10)
        return result
    else:
        return[1,1]
    
data= read_pm()
pm_data = validate_data(data)
