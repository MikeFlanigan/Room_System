import datetime
import timeit
import subprocess
import RPi.GPIO as gpio

alarm_time_hour = 7 # 5 am default
alarm_time_minute = 10

Alarm_Lights = False
Alarm_Lights_Flash = False

Audio_Enable = True
Play_Music = False
Music_Start_OneShot = False


Alarm_Set_Btn = False


Lights_Pin = 26 
Flash_Pin = 19
# _Pin = 13
gpio.setmode(gpio.BCM) # sets pin reference scheme to gpio instead of pins
##gpio.setup(MPS_inp, gpio.IN) # N.O.
##gpio.setup(Start_btn, gpio.IN, gpio.PUD_UP) # N.C. with pull up resistor
gpio.setup(Lights_Pin, gpio.OUT) # N.O.
gpio.setup(Flash_Pin, gpio.OUT) # N.O.

try:
    while True:
        Now = datetime.datetime.now()
        # alarm button
        # if alarm set button high for
        alarm_set_time = timeit.default_timer() # begin timer
        while Alarm_Set_Btn:
            # enter into alarm set mode
            if timeit.default_timer()-alarm_set_time > 6000:
                # enter setting mode
                # every press moves the alarm time up by 0.25 hours
                unpressed_time = timeit.default_timer()
                while timeit.default_timer() - unpressed_time < 20000:
                    if Alarm_Set_Btn:
                        unpressed_time = timeit.default_timer()
                        alarm_time_minute += 15
                        if alarm_time_minute >= 60:
                            alarm_time_minute = 0
                            alarm_time_hour += 1
                        if alarm_time_hour >= 8: # no later than 8 pm
                            alarm_time_hour = 5
                            alarm_time_minute = 0
        # audio button
        # if audio button latched then enable audio
        # if audio button unlatched then no audio
        # if latched and unlatched 4 times in 8 seconds then launch an audio test


        # if time t - 5 then set a digital pin high for 5 seconds to tell arduino
        # to begin lights
        # if time t = 0 then ''''
        # if t+ 5 then ''''

        
        if Now.hour == alarm_time_hour - 1 and Now.minute >= 55 and alarm_time_minute == 0:
            Alarm_Lights = True
        elif Now.hour == alarm_time_hour and Now.minute >= alarm_time_minute-5 and Now.minute<=alarm_time_minute+5:
            Alarm_Lights = True
        else: Alarm_Lights = False 
            

        if Now.hour == alarm_time_hour and Now.minute >= alarm_time_minute and Now.minute <= alarm_time_minute+5:
            Alarm_Lights_Flash = True
            Play_Music = True
        else:
            Play_Music = False
            Alarm_Lights_Flash = False

##        Alarm_Lights = True
##        Alarm_Lights_Flash = True
        if Play_Music and not Music_Start_OneShot:
            Music_Start_OneShot = True
            music = subprocess.Popen(['mpg123','/home/pi/Desktop/theintro.mp3'])
        elif not Play_Music and Music_Start_OneShot:
            music.kill()
            Music_Start_OneShot = False
        
        if Alarm_Lights: gpio.output(Lights_Pin, gpio.LOW)
        else: gpio.output(Lights_Pin, gpio.HIGH)
        if Alarm_Lights_Flash: gpio.output(Flash_Pin, gpio.LOW)
        else: gpio.output(Lights_Pin, gpio.HIGH)
        print("Lights:",Alarm_Lights,"Flash:",Alarm_Lights_Flash,"Music:",Play_Music)
except KeyboardInterrupt: pass
gpio.cleanup()
