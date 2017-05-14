import numpy as np
import datetime
import timeit
import subprocess
import RPi.GPIO as gpio

# default alarm time variables
alarm_hour = 7 # X am default
alarm_minute = 10

Alarm_Lights = False
Alarm_Lights_Flash = False

Audio_Enable = True
Play_Music = False
Power_Speakers = False
Music_Start_OneShot = False

Change_Flag = False


Alarm_Set_Btn = False


## music files and info
music_files = ['/home/pi/Desktop/theintro.mp3', '/home/pi/Desktop/LifeCycles.mp3']
music_file_len = [4600, 103000] # approximate number of frames in each audio file


######## pin definitions ########
Lights_Pin = 26 # com to arduino
Flash_Pin = 19 # com to arduino
Speaker_Pwr_Pin = 6 # com to arduino
# _Pin = 13 # unused

####### gpio setup ###############
gpio.setmode(gpio.BCM) # sets pin reference scheme to gpio instead of pins
##gpio.setup(MPS_inp, gpio.IN) # N.O.
##gpio.setup(Start_btn, gpio.IN, gpio.PUD_UP) # N.C. with pull up resistor
gpio.setup(Lights_Pin, gpio.OUT) # N.O.
gpio.setup(Flash_Pin, gpio.OUT) # N.O.
gpio.setup(Speaker_Pwr_Pin, gpio.OUT) # N.O.


## temp counter vars
ii = 0

######## main loop ##############
try:
    while True:
        Now = datetime.datetime.now() # update time variable

        ## update alarm each day
        if Now.hour >= 11:
            try:
                alarm = datetime.datetime(Now.year,Now.month,Now.day+1,alarm_hour,alarm_minute)
                alarm = datetime.datetime(Now.year,Now.month,Now.day,alarm_hour,alarm_minute)
            except ValueError:
                try:
                    alarm = datetime.datetime(Now.year,Now.month+1,1,alarm_hour,alarm_minute)
                except ValueError:
                    alarm = datetime.datetime(Now.year+1,1,1,alarm_hour,alarm_minute)
        else: alarm = datetime.datetime(Now.year,Now.month,Now.day,alarm_hour,alarm_minute)

        ########## alarm activation
        Alarm_Lights = False
        Play_Music = False
        Alarm_Lights_Flash = False
        
        alm_delt = alarm-Now # time till alarm
        if (alm_delt.days >= 0 and alm_delt.seconds/60 <= 3) or (alm_delt.days < 0 and abs(int(alm_delt.seconds/60-24*60)) < 5): # T - 3 to T + 5
            Alarm_Lights = True
            Play_Music = True
        elif alm_delt.days >= 0 and alm_delt.seconds/60 <= 15: # T - 15
            Alarm_Lights = True
        elif alm_delt.days < 0 and abs(int(alm_delt.seconds/60-24*60)) < 15: # T + 5-15
            Alarm_Lights = True
            Play_Music = True
            Alarm_Lights_Flash = True
        ############################


        ######### alarm button
        # if alarm set button high for
##        alarm_set_time = timeit.default_timer() # begin timer
##        while Alarm_Set_Btn:
##            # enter into alarm set mode
##            if timeit.default_timer()-alarm_set_time > 6000:
##                # enter setting mode
##                # every press moves the alarm time up by 0.25 hours
##                unpressed_time = timeit.default_timer()
##                while timeit.default_timer() - unpressed_time < 20000:
##                    if Alarm_Set_Btn:
##                        unpressed_time = timeit.default_timer()
##                        alarm_time_minute += 15
##                        if alarm_time_minute >= 60:
##                            alarm_time_minute = 0
##                            alarm_time_hour += 1
##                        if alarm_time_hour >= 8: # no later than 8 pm
##                            alarm_time_hour = 5
##                            alarm_time_minute = 0
        # audio button
        # if audio button latched then enable audio
        # if audio button unlatched then no audio
        # if latched and unlatched 4 times in 8 seconds then launch an audio test


        # if time t - 5 then set a digital pin high for 5 seconds to tell arduino
        # to begin lights
        # if time t = 0 then ''''
        # if t+ 5 then ''''




           ####### debug testing and overrides ###########
	if Now.second - 30 <= 0:
            Alarm_Lights = False
            Play_Music = False
            Alarm_Lights_Flash = False
        else:
            Alarm_Lights = True
##            Play_Music = True
            Alarm_Lights_Flash = True

        ######### starting and stopping music subprocess ###################
        if Play_Music and not Music_Start_OneShot:
            Music_Start_OneShot = True
            Power_Speakers = True
            file_num = np.random.randint(0,2)
            if file_num == 0: # the intro
            	skip = '-k 1'
            else:
		if np.random.randint(0,2) == 0:
                	skip = '-k 1'
                else:
			# below means start position will be randomly between t+5 and t-5 on the music file
            		frame = int(np.random.rand(1)[0]*(music_file_len[file_num]-30000)+15000) 	
			skip = '-k ' + str(frame) 
            print(music_files[file_num], 'start frame:',skip)
            music = subprocess.Popen(['mpg123',skip,music_files[file_num]])
        elif not Play_Music and Music_Start_OneShot:
            music.kill()
            Music_Start_OneShot = False
            Power_Speakers = False




            
        ######## gpio pin controls ################################
        if Alarm_Lights: gpio.output(Lights_Pin, gpio.LOW)
        else: gpio.output(Lights_Pin, gpio.HIGH)
        if Alarm_Lights_Flash: gpio.output(Flash_Pin, gpio.LOW)
        else: gpio.output(Flash_Pin, gpio.HIGH)
        if Power_Speakers: gpio.output(Speaker_Pwr_Pin, gpio.LOW)
        else: gpio.output(Speaker_Pwr_Pin, gpio.HIGH)




        ### debug printing
##        print("Lights:",Alarm_Lights,"Flash:",Alarm_Lights_Flash,"Music:",Play_Music)
        if not Change_Flag and Alarm_Lights_Flash:
            print(Alarm_Lights_Flash)
            Change_Flag = True
        elif Change_Flag and not Alarm_Lights_Flash:
            print(Alarm_Lights_Flash)
            Change_Flag = False
        



########## shutdown, interrupt, cleanup ###############
except KeyboardInterrupt: pass
gpio.cleanup()
