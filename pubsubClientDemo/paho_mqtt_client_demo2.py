#this works with simple mosquitto server
'''
mosquitto -v -c mosquitto.conf #conf is optional
mosquitto -v
#check port mosquitto is running on is standard port so pub/sub works
netstat -a | findstr 18
mosquitto_sub  -d -t testTopic
mosquitto_pub  -d -t testTopic -m "Hello world!"

'''
#http://www.steves-internet-guide.com/python-mqtt-publish-subscribe/


import time
import paho.mqtt.client as paho
#broker="localhost"
broker="127.0.0.1"
#broker="192.168.1.118"
#define callback
def on_message(client, userdata, message):
    time.sleep(1)
    print("received message =",str(message.payload.decode("utf-8")))
    #add code to store received message here.

client= paho.Client("client-001") #create client object client1.on_publish = on_publish #assign function to callback client1.connect(broker,port) #establish connection client1.publish("house/bulb1","on")
######Bind function to callback
client.on_message=on_message
#####
print("connecting to broker ",broker)
client.connect(broker)#connect
#returns 0 if successful?
client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe("testTopic")#subscribe
time.sleep(2)
print("publishing ")
client.publish("testTopic","on")#publish
time.sleep(4)
client.disconnect() #disconnect
client.loop_stop() #stop loop
#exit()
