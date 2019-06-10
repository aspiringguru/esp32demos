#to run python3 > cd to dir containing python3
#cd C:\Users\Matthew\Anaconda3

#need to setup btstack.h in arduino
#https://techtutorialsx.com/2017/07/08/esp32-bluetooth-using-the-btstack-library/
#https://github.com/bluekitchen/btstack

#setup mysys2
#https://docs.espressif.com/projects/esp-idf/en/latest/get-started/windows-setup.html

import bluetooth

devices = bluetooth.discover_devices(lookup_names=True)
print(type(devices))

print("Devices found: %s" % len(devices))

for item in devices:
    print(item)
