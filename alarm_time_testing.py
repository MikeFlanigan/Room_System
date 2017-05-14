import datetime
import numpy as np


alarm_hour = 6
alarm_minute = 20

while True:
    Now = datetime.datetime.now()


    if Now.hour >= 11:
        try:
            alarm = datetime.datetime(Now.year,Now.month,Now.day+1,alarm_hour,alarm_minute)
##            alarm = datetime.datetime(Now.year,Now.month,Now.day,alarm_hour,alarm_minute)
        except ValueError:
            try:
                alarm = datetime.datetime(Now.year,Now.month+1,1,alarm_hour,alarm_minute)
            except ValueError:
                alarm = datetime.datetime(Now.year+1,1,1,alarm_hour,alarm_minute)
    else: alarm = datetime.datetime(Now.year,Now.month,Now.day,alarm_hour,alarm_minute)


    dif = alarm-Now

    if dif.days >= 0 and dif.seconds/60 <= 15:
        print('T - ', int(dif.seconds/60), np.mod(dif.seconds,60))
    elif dif.days < 0 and abs(int(dif.seconds/60-24*60)) < 15:
        print('T + ', abs(int(dif.seconds/60-24*60)), 60-np.mod(dif.seconds,60))
    else:
        print(dif.days,int(dif.seconds/60), np.mod(dif.seconds,60))

    
