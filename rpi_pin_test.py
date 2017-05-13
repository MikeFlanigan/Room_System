import RPi.GPIO as gpio
import datetime

Change_Flag = False

gpio.setmode(gpio.BCM) # sets pin reference scheme to gpio instead of pins


gpio.setup(Lights_Pin, gpio.OUT) # N.O.


Lights_Pin = 26 # com to arduino

try:
    while True:

        Now = datetime.datetime.now() # update time variable

        if Now.second - 30 <= 0:
                Alarm_Lights = False
        else:
            Alarm_Lights = True

            
        ######## gpio pin controls ################################
        if Alarm_Lights: gpio.output(Lights_Pin, gpio.LOW)
        else: gpio.output(Lights_Pin, gpio.HIGH)


        if not Change_Flag and Alarm_Lights:
            print(Alarm_Lights)
            Change_Flag = True
        elif Change_Flag and not Alarm_Lights:
            print(Alarm_Lights)
            Change_Flag = False
##        print(Alarm_Lights)
except KeyboardInterrupt: pass
gpio.cleanup()
