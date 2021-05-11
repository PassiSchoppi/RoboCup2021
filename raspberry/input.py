import serial

ser = serial.Serial('/dev/ttyAMA0')
ser.baudrate = 19600
while(True):
    print(ser.read())

ser.close()
