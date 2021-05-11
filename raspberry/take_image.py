import cv2
import os

def take_image(num, pause):
    cv2.namedWindow("cam1")
    cv2.namedWindow("cam2")

    vc1 = cv2.VideoCapture(0)
    vc2 = cv2.VideoCapture(2)
    
    vc1.set(cv2.CAP_PROP_EXPOSURE, -8)
    vc1.set(cv2.CAP_PROP_EXPOSURE, -8)


    for i in range(1, num + 1):
        number = len(os.listdir('images/data'))
        
        inameI = "imageI" + '_' + str(number + 1) + ".jpg"
        inameII = "imageII" + '_' + str(number + 1) + ".jpg"
        
        rval, frame1 = vc1.read()
        rval, frame2 = vc2.read()
        
        cv2.imshow("cam1", frame1)
        cv2.imshow("cam2", frame2)
        
        os.chdir("images/insert_I")
        cv2.imwrite(inameI, frame1)

        os.chdir("images/insert_I")
        cv2.imwrite(inameII, frame2)

        cv2.waitKey(pause)

