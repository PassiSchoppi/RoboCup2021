import cv2
import os

def take_image(num, pause, cam):
    cv2.namedWindow("cam1")
    cv2.namedWindow("cam2")

    vc1 = cv2.VideoCapture(0)
    vc2 = cv2.VideoCapture(2)
    
    vc1.set(cv2.CAP_PROP_EXPOSURE, -1)
    vc1.set(cv2.CAP_PROP_EXPOSURE, -1)

    if (cam == 1):
        number = len(os.listdir('/home/pi/Robocup_2020/images/insert_1'))
        for i in range(1, num + 1):        
            inameI = "image1" + '_' + str(number + i) + ".jpg"       
            rval, frame1 = vc1.read()        
            #cv2.imshow("cam1", frame1)        
            os.chdir("/home/pi/Robocup_2020/images/insert_1")
            cv2.imwrite(inameI, frame1)
            cv2.waitKey(pause)
            return inameI

    if (cam == 2):
        number = len(os.listdir('/home/pi/Robocup_2020/images/insert_2'))
        for i in range(1, num + 1):
            inameII = "image2" + '_' + str(number + 1) + ".jpg"
            rval, frame2 = vc2.read()
            #cv2.imshow("cam2", frame2)
            os.chdir("/home/pi/Robocup_2020/images/insert_2")
            cv2.imwrite(inameII, frame2)
            cv2.waitKey(pause)
            return inameII
