# ArduinoJune2019

Collection of Arduino refresher projects June 2019

## Installation

https://www.arduino.cc/

this is best instruction > https://developers.wia.io/docs/wemos-esp-wroom-32

also this youtube
https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers
download, unzip, open CP210xVCPInstaller_x64.exe _AS_ADMINISTRATOR_

troubleshooting - remove CP210x device driver
https://www.silabs.com/community/interface/knowledge-base.entry.html/2014/11/19/uninstalling_thecp2-QPVH

https://www.youtube.com/watch?v=C_KPTnHgAl8
cmd prompt
cd c:\
SET DEVMGR_SHOW_NONPRESENT_DEVICES=1
DEVMGMT.MSC   (this opens device manager)
device manager > menu > view > show hidden devices


to verify installed > device manager > ports (COM & LPT)
check the COM nominated is set in arduino Menu > Tools > port


for ESP32 add this json to menu > file > preferences > “Additional Board Manager URLs”
https://dl.espressif.com/dl/package_esp32_index.json
(this worked for ESP-WROOM-32)

more instructions here
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/

Menu > Tools > Board > Boards Manager >
search for ESP32, select, wait for install
this is slow. very slow. just wait

reference (not needed if above install process followed)
https://github.com/espressif/arduino-esp32


https://github.com/espressif/esptool/#troubleshooting

good instructions on how to setup pyserial with python3
https://youtu.be/Pf-cGzOQmXU
pip install pyserial

add capacitor to remove need to hold down boot button when uploading new sketch.
https://randomnerdtutorials.com/solved-failed-to-connect-to-esp32-timed-out-waiting-for-packet-header/


## Usage
ESPWROOM32 requires board firebeetle ESP-32 selected
(arduino > menu > Tools > board > firebeetle32)

upload speed needs to be nominated and matching in three places.
1: device manager > ports > (select silicon labs CP210x USB to UART Bridge)
right click > properties > port settings > bits per second > select 115200  

2: in arduino > menu > tools > upload speed > select 115200  
3: in arduino sketch, when opening serial connection ie : Serial.begin(115200);

NB: when using the serial monitor in Arduino, need to set the baud rate to match the above. otherwise serial output is garbled.

write sketch in arduino  
start the upload in arduino menu > Sketch > upload
hold the 'boot' button down on the ESPWROOM32 board during connection until 100% written

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License
[MIT](https://choosealicense.com/licenses/mit/)
