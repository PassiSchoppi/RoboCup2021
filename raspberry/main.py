import os 
import take_image  
import output 
import image_evaluation
import time
import serial

ser = serial.Serial('/dev/ttyACM0', 115200)


files = os.listdir('/home/pi/RoboCup2021/images/insert_1')
if len(files) > 0:
    arch = os.listdir('/home/pi/RoboCup2021/images/archive/')
    os.makedirs('/home/pi/RoboCup2021/images/archive/' + str(len(arch)))
    os.chdir('/home/pi/RoboCup2021/images/insert_1')
    for file in files:
        print(file, len(arch))
        os.rename('/home/pi/RoboCup2021/images/insert_1/' + file, '/home/pi/RoboCup2021/images/archive/' + str(len(arch)) + '/' + file)

    os.chdir('/home/pi/RoboCup2021/images/insert_2')
    files = os.listdir('/home/pi/RoboCup2021/images/insert_2')
    for file in files:
        os.rename('/home/pi/RoboCup2021/images/insert_2/' + file, '/home/pi/RoboCup2021/images/archive/' + str(len(arch)) + '/' + file)

try:
    while (True):
        trigger = ser.read()
        print(trigger)
        if (trigger == 'j'):
            name1 = take_image.take_image(1, 10, 1)
            name2 = take_image.take_image(1, 10, 2)
            print('take')
            colour1 = image_evaluation.colour_search(name1, 1)
            colour2 = image_evaluation.colour_search(name2, 2)
            print('colour')
            letter1 = image_evaluation.take_picture(name1, 1)
            letter2 = image_evaluation.take_picture(name2, 2)
            print('letter')

            if colour1 != 'e':
                print(colour1)
                state = colour1        
                if state == 'r':
                 ser.write("r")
                 os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'r.jpg')
                 
                if state == 'g':
                 ser.write("g")
                 os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'g.jpg')
                 
                if state == 'y':
                 ser.write("y")
                 os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'y.jpg')

            if colour2 != 'e':
                print(colour2)
                state = colour2
                if state == 'r':
                    ser.write("R")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'R.jpg')
                 
                if state == 'g':
                    ser.write("G")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'G.jpg')
                 
                if state == 'y':
                    ser.write("Y")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'Y.jpg')

            
            if letter1 != 'e':
                print(letter1)
                state = letter1
                if state == 'H':
                    ser.write("h")
                    os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'h.jpg')
             
                if state == 'U':
                    ser.write("u")
                    os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'u.jpg')
             
                if state == 'S':
                    ser.write("s")
                    os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 's.jpg')
            
            if letter2 != 'e':
                print(letter2)
                state = letter2
                if state == 'H':
                    ser.write("H")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'H.jpg')
             
                if state == 'U':
                    ser.write("U")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'U.jpg')
             
                if state == 'S':
                    ser.write("S")
                    os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name2[:-4] + 'S.jpg')
            
            if (colour1 == 'e') and (letter1 == 'e'):
                os.rename('/home/pi/RoboCup2021/images/insert_1/' + name1, '/home/pi/RoboCup2021/images/insert_1/' + name1[:-4] + 'e.jpg')
            if (colour2 == 'e') and (letter2 == 'e'):
                os.rename('/home/pi/RoboCup2021/images/insert_2/' + name2, '/home/pi/RoboCup2021/images/insert_2/' + name1[:-4] + 'E.jpg')
            

            if (colour1 == 'e') and (colour2 == 'e') and (letter1 == 'e') and (letter2 == 'e'):
                ser.write('e')
             
            print('output')
except:
    print('')
