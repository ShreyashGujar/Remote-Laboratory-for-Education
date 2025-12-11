# Importing Libraries
import serial
import time
import struct
 

# arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=9600, timeout=.1)
arduino = serial.Serial(port='/dev/serial0', baudrate=9600, timeout=.2)
send_array_passive_read =[0x42,0x4d,0xe2,0x00,0x00,0x01,0x71]
send_array_cm = [0x42,0x4d,0xe1,0x00,0x00,0x01,0x70]
send_array_wake =[0x42,0x4d,0xe4,0x00,0x01,0x01,0x74]
send_array_sleep =[0x42,0x4d,0xe4,0x00,0x00,0x01,0x73]
# send_array = chr(send_array)
def write_read(x,size):
#     arduino.write(bytes(x, 'utf-8'))
    arduino.write(x)
#    time.sleep(1)
    data = arduino.read(size)
#     .decode('utf-8').rstrip()
    return data

def change_mode():
    num = send_array_cm
    value = write_read(num,8)
    values1 = list(value)
    print("change mode:" + str(values1))
    time.sleep(1)

def go_to_sleep():
    num = send_array_sleep
    value = write_read(num,8)
    values1 = list(value)
    print("go to sleep: "+ str(values1))
    time.sleep(1)

def wake_up():
    num = send_array_wake
    value = write_read(num,8)
    values1 = list(value)
    print("WAKE UP:"+str(values1))
    time.sleep(1)


def read_pm():
#    num = send_array_wake
#    value = write_read(num)
#    values1 =list(value)
    #print(values1) # printing the value
#    time.sleep(1)

    num = send_array_passive_read
    value = write_read(num,32)
    values2 =list(value)
    print("Response:"+str(values2)) # printing the value
    time.sleep(1)

 #  num = send_array_sleep
 #  value = write_read(num)
 #  values3 =list(value)
    #print(values) # printing the value
  #  time.sleep(1)
    return values2

def validate_data(data):
#     print(data)
    data[12] = data[12]*256
    data[14] = data[14]*256
    data[30] = data[30]*256
    data[32] = data[32]*256

#     print(data)
    sum_r = data[30]+data[31]
    checksum = sum(data) - sum_r

#     print(sum_r)
#     print(checksum)
    if(sum_r==checksum):
        result = []
        pm10 = data[14] + data[15]
        pm25 = data[12] + data[13]
        result.append(pm25)
        result.append(pm10)
        print("PM2.5: ",pm25)
        print("PM10: ",pm10)
        return result
    else:
        return[-1,-1]

wake_up()
change_mode()

while True:
    #wake_up()
    #change_mode()
    arduino.flush()
    data= read_pm()
    pm_data = validate_data(data)
    arduino.flush()
    #go_to_sleep()
    time.sleep(2)
  