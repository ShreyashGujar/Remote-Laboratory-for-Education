import csv
import os
from gpiozero import LED
sel0 = LED(16)
sel1 = LED(20)
# Temperature, Pressure and Humidity
import smbus2
import bme280
port = 1
address = 0x76
bus = smbus2.SMBus(port)

calibration_params = bme280.load_calibration_params(bus, address)

def get_weather_data():
    data = bme280.sample(bus, address, calibration_params)
    return data.temperature, data.pressure, data.humidity

# GPS
import serial
import time
import string
import pynmea2

def get_gps_coordinates():
    sel0.off()
    sel1.on()
    time.sleep(1)
    port="/dev/ttyUSB0"
    ser=serial.Serial(port, baudrate=9600, timeout=0.5)
    dataout = pynmea2.NMEAStreamReader()
    newdata=ser.readline()

    if newdata[0:6] == "$GPRMC":
        newmsg=pynmea2.parse(newdata)
        lat=newmsg.latitude
        lng=newmsg.longitude
        gps = "Latitude=" + str(lat) + "and Longitude=" + str(lng)
        print(gps)
        return  lat,lng
    else:
        return [-1,-1]

#PM sensor
import serial
import time
import struct

# arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=9600, timeout=.1)
arduino = serial.Serial(port='/dev/serial0', baudrate=9600,timeout=0.2)
send_array_passive_read =[0x42,0x4d,0xe2,0x00,0x00,0x01,0x71]
send_array_cm = [0x42,0x4d,0xe1,0x00,0x00,0x01,0x70]
send_array_wake =[0x42,0x4d,0xe4,0x00,0x01,0x01,0x74]
send_array_sleep =[0x42,0x4d,0xe4,0x00,0x01,0x01,0x73]
# send_array = chr(send_array)
def write_read(x,size):
#     arduino.write(bytes(x, 'utf-8'))
    arduino.write(x)
    #data = arduino.readline()
    data = arduino.read(size)
#     .decode('utf-8').rstrip()
    return data

def change_mode():
    num = send_array_cm
    value = write_read(num,8)
    values1 = list(value)
    #print(values1)
    #time.sleep(1)

def sleep_mode():
    num = send_array_sleep
    value = write_read(num,8)
    values1 = list(value)
    #print(values1)
    #time.sleep(1)

def wake_mode():
    num = send_array_wake
    value = write_read(num,8)
    values1 = list(value)
    #print(values1)
    #time.sleep(1)


def read_pm():
#    num = send_array_wake
#    value = write_read(num)
#    values1 =list(value)
    #print(values1) # printing the value
#    time.sleep(1)

    num = send_array_passive_read
    value = write_read(num,32)
    values2 =list(value)
    #print(values2) # printing the value
    #time.sleep(1)

 #  num = send_array_sleep
 #  value = write_read(num)
 #  values3 =list(value)
    #print(values) # printing the value
  #  time.sleep(1)
    return values2

def validate_data(data):
#     print(data)
    if(len(data) == 32):
        data[12] = data[12]*256
        data[14] = data[14]*256
        data[30] = data[30]*256
        #print(data)
        sum_r = data[30]+data[31]
        checksum = sum(data) - sum_r
    #   print(sum_r)
#       print(checksum)
        if(sum_r==checksum):
            result = [-1,-1]
            pm10 = data[14] + data[15]
            pm25 = data[12] + data[13]
            result[0] = pm25
            result[1] = pm10
            print("PM2.5: ",pm25)
            print("PM10: ",pm10)
            return result
        else:
            print(data)
            sleep_mode()
            time.sleep(1)
            wake_mode()
            return [-1,-1]
    else:
        print(data)
        #print("PMS error")
        return[-1,-1]


#ADC libraries and MSGEQ-7
import board
import busio
from gpiozero import LED
from time import sleep

milli = 1/1000
micro = 1/1000000

i2c = busio.I2C(board.SCL, board.SDA)

import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

strobe = LED(23)
reset = LED(24)
def setup_msgeq7():
    reset.off()
    strobe.off()
    sleep(1*milli)
    reset.on()
    sleep(1*milli)
    reset.off()
    strobe.on()
    sleep(1*milli)
    
bands = []
ads = ADS.ADS1115(i2c)
ads.gain = 16

def read_msgeq():
    ads.gain = 1
    bands=[]
    for i in range(0,7):
        strobe.off()
        sleep(80*micro)
        chan = AnalogIn(ads, ADS.P0)
        bands.append(chan.value)
        #bands.append(chan)
        strobe.on()
        sleep(80*micro)
    #print(bands)   
    return bands

#camera and email
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders
import datetime;
from picamera import PiCamera
import os,shutil,sys
import urllib.request

camera = PiCamera()

memory_path = "/home"

def get_time():
    # Get time stamp
    ct = datetime.datetime.now()
    time_stamp = (str(ct.year) + '-' + str(ct.month) + '-' + str(ct.day) +' '+
    str(ct.hour) + ':' + str(ct.minute) + ':' + str(ct.second))
    return time_stamp

def take_picture(time_stamp,save_path):
    camera.start_preview()
    sleep(5)
    camera.capture(save_path)
    camera.stop_preview()
    
device_id = "124"
time_stamp = get_time()
mail_content = time_stamp
sender_address = 'rpi.uploader007@gmail.com'
sender_pass = 'a1b2c3d4$'
receiver_address = 'rpi.uploader007@gmail.com'
subject_text =  device_id
attach_file_name = device_id+ ' ' + time_stamp  + '.jpg'
save_path = '/home/pi/Desktop/scripts/' + attach_file_name
host = 'http://google.com'

def send_email(sender_address,sender_pass,receiver_address,subject_text,attach_file_name):

    #Setup the MIME
    message = MIMEMultipart()
    message['Subject'] = subject_text
    message['From'] = sender_address
    message['To'] = receiver_address


    #The body and the attachments for the mail
    message.attach(MIMEText(mail_content, 'plain'))
    attach_file = open(attach_file_name, 'rb') # Open the file as binary mode
    data = attach_file.read()
    payload = MIMEBase('application', "octet-stream")
    payload.set_payload(data)
    encoders.encode_base64(payload) #encode the attachment
    #add payload header with filename
    payload.add_header("Content-Disposition", 'attachment', filename=attach_file_name)
    message.attach(payload)


    #Create SMTP session for sending the mail
    try:
        session = smtplib.SMTP('smtp.gmail.com', 587) #use gmail with port
        session.starttls() #enable security
        session.login(sender_address, sender_pass) #login with mail_id and password
        text = message.as_string()
        session.sendmail(sender_address, receiver_address, text)
        session.quit()
        os.remove(save_path)
        
    except SMTPException:
        print ("Error: unable to send email")


def connect(host):
    try:
        urllib.request.urlopen(host) #Python 3.x
        return True
    except:
        return False

def read_co():
    ads.gain = 16
    W = AnalogIn(ads, ADS.P2)
    #time.sleep(0.1)
    RC = AnalogIn(ads,ADS.P1)
    return( RC.value - W.value)

#loop
setup_msgeq7()

import thingspeak
channel_id = 1477732
write_key = 'LN25DJXKCY8ODO02'
read_key = 'MLTJB3QJBKVC73Q6'
channel_write = thingspeak.Channel(id=channel_id, api_key=write_key)

change_mode()
#time.sleep(5)
arduino.flush()
filename = "log_file.csv"
time.sleep(1)

import subprocess
if(connect(host)):
#    subprocess.call(['sh', './ntp_update.sh'])
    os.system("sudo service ntp start")

pm_data =[-1,-1]
reset_count = 0

while True:
    #time.sleep(1)
    arduino.reset_input_buffer()
    data1= read_pm()
    #pm_data = [-1,-1]
    pm_data = validate_data(data1)
    print(pm_data)
    if(pm_data[0]== -1):
        reset_count = reset_count+1
    else:
        reset_count=0

    if(reset_count>4):
        os.system("sudo reboot now")
    #arduino.flush()
    temp,pressure,humidity = get_weather_data()

    #bands = read_msgeq()
    bands=[1,1,1,1,1,1,1]
    co = read_co()
    print(co)
    #lat, long = get_gps_coordinates()
    lat =0
    long =0
    time_stamp = get_time()
    mail_content = time_stamp.split()[0]
    attach_file_name = device_id+ ' ' + time_stamp  + '.jpg'
    save_path = '/home/pi/Desktop/scripts/' + attach_file_name
    take_picture(time_stamp,save_path)
    if(connect(host)):
        send_email(sender_address,sender_pass,receiver_address,subject_text,attach_file_name)
        response = channel_write.update({'field1':temp,'field2':humidity, 'field3':pm_data[0],
        'field4':pm_data[1],'field5':time_stamp, 'field6':co, 'latitude': lat, 'longitude':long, 'elevation':pressure})
    else:
        file =  open(filename, 'a')
        obj=csv.writer(file)
        csvwriter = csv.writer(file)
        rows = []
        rows.append(time_stamp)
        rows.append(temp)
        rows.append(humidity)
        rows.append(pm_data[0])
        rows.append(pm_data[1])
        rows.append(str(bands))
        rows.append(co)
        rows.append(lat)
        rows.append(long)
        rows.append(pressure)
        csvwriter.writerow(rows)
        file.close()

    stat = shutil.disk_usage(memory_path)
    if((stat[2]/1000000000) < 10):
        sys.exit()
    print("Iteration over. Going to sleep.")
    sleep(20)
