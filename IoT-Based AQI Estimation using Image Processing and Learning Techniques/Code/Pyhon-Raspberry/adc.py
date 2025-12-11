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
ads.gain = 1
while True:
    for i in range(0,7):
        strobe.off()
        sleep(80*micro)
        chan = AnalogIn(ads, ADS.P0)
        #bands.append(round(chan.voltage,2))
        bands.append((chan.value))
        strobe.on()
        sleep(80*micro)
    print(bands)
    bands=[]
    chan1 = AnalogIn(ads, ADS.P1)
    chan2 = AnalogIn(ads , ADS.P2)
    #print(chan1.value)
    #print(chan2.value)
    #print(chan1.value - chan2.value)
    sleep(1000*milli)


