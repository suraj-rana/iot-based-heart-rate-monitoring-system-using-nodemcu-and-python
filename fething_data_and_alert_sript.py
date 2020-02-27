import urllib.request
import requests
import threading
import json

import random
import time



def read_data_thingspeak():
    URL='https://api.thingspeak.com/channels/902378/feeds.json?results=10'  # put json url from thingspeak channel
    KEY='Z7WLF99O66198LOI&results=' #put key from thingspeak channel
    HEADER=2
    NEW_URL=URL+KEY+str(HEADER)
    #print(NEW_URL)

    get_data=requests.get(NEW_URL).json()
    #print(get_data)
    channel_id=get_data['channel']['id']

    feild_1=get_data['feeds']
    #print(feild_1)

    t=[]
    for x in feild_1:
        #print(x['field1'])
        t.append(x['field1'])
    return t,HEADER

        

if __name__ == '__main__':

    new=[]
    while(1):
        
        new,b=read_data_thingspeak()
        print(new)

        for var in range(0,b,1):
            
                if(int(new[var])>110):
                
                    print("abnormality detected.....Beats per minute "+new[var]+" :HIGH")
                    print("call ambulance asap..")
                elif((int(new[var])<25) and (int(new[var])>10)):
                    print("abnormality detected.....Beats per minute "+new[var]+" :LOW")
                else:
                    pass
        time.sleep(17)

                
            
    
