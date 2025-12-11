from gpiozero import LED
from time import sleep

led1 = LED(16)
led2 = LED(20)
led3 = LED(21)
led4 = LED(23)
led5 = LED(24)
led1.off()
led3.off()
while True:
    #led1.on()
    led2.on()
    #led3.on()
    #led4.on()
    #led5.on()
    sleep(2)
    #led1.off()
    led2.off()
    #led3.off()
    #led4.off()
    #led5.off()
    sleep(2)
