#import numpy as np
#import array
import os
#import math
from PIL import Image
import function

global edit

#data = os.listdir('images/insert')
#print(data)
#print(len(data))

#for i in range(len(data)):
#    print(data[i])

#pixel_1 = ['a', 'b', 'c']
#pixel_2 = ['d', 'e', 'f']
#pixel_3 = ['g', 'h', 'i']

#alle = [pixel_1, pixel_2, pixel_3]

#print(alle)
#print(alle[1])
#print(alle[1][1])

#alle[1][1] = 'z'

#print(alle[1][1])
#print(alle[1])
#print(alle)

#liste = []
#for i in range(10):
#  liste.append("variable"+str(i))
#print(liste)

#for i in range(10):
#    for j in range(10):
#        liste[i].append('e')

#print(liste)

#x = 12.9
#print(str(math.floor(x)))

#def blop_naming(x, y, num, height, width): #benennt rekursiv alle schwarzen Pixel, die zusammen hängen, um
#    #print(num)
#    edit[x][y] = num
    
#    if (x-1 >= 0) and (y-1 >= 0):
#        if (edit[x-1][y-1] == 1):   #links oben
#            blop_naming((x-1), (y-1), num, height, width)
#    if (y-1 >= 0):    
#        if (edit[x][y-1] == 1): #mitte oben
#            blop_naming(x, (y-1), num, height, width)
#    if (x+1 < width) and (y-1 >= 0):
#        if (edit[x+1][y-1] == 1):   #rechts oben
#            blop_naming((x+1), (y-1), num, height, width)
#    if (x-1 > 0):
#        if (edit[x-1][y] == 1):  #links mitte
#            blop_naming((x-1), y, num, height, width)
#    if (x+1 < width):
#        if (edit[x+1][y] == 1): #rechts mitte
#            blop_naming((x+1), y, num, height, width)
##    if (x-1 > 0) and (y+1 < height):
##        if (edit[x-1][y+1] == 1) or ((edit[x-1][y+1] != num) and (edit[x-1][y+1] != 0)):   #links unten
##            blop_naming((x-1), (y+1), num, height, width)
#    if (y+1 <= 0):
#        if (edit[x][y+1] == 1): #mitte unten
#            blop_naming(x, (y+1), num, height, width)
#    if (x+1 < width) and (y+1 < height):
#        if (edit[x+1][y+1] == 1): #rechts unten
#            blop_naming((x+1), (y+1), num, height, width)


#def write(array, dataname):
#    dataname = 'txt info/' + dataname
#    try:
#        os.remove(dataname)
#    except:
#        print('')

#    file = open(dataname, 'a')

#    for i in range(len(array)):
#        file.write(str(array[i]) + '\n')
#    file.close

#img = Image.open('images/insert/image-h_1.jpg')     #liest Image ein
#img.thumbnail((102, 80))                            #verkleinert Image auf 128x96 Pixel (102x80)
#img = img.convert('L')                              #wandelt Bild in Graustufen um

##img.show()
#height, width = img.size
#data = list(img.getdata())
#data = [data[offset:offset+height] for offset in range(0, width*height, height)] #erzeugt ein Zweidimensionales Array mit den Farbwerten des entsprechenden Pixels
##print(data)

#edit = data

#for i in range(width):
#    for j in range(height):
#        if (edit[i][j] < 75):
#            edit[i][j] = 1
#        else:
#            edit[i][j] = 0

##print(edit)

#write(edit, 'row.txt')

#num = 1
#for i  in range(width - 1):
#    for j in range(height - 1):
#        if (edit[i][j] == 1):
#            num = num + 1
#            blop_naming(i, j, num, height, width)
#            #print(num)
#            #break;

##print(edit)
#write(edit, 'image.txt')

#print(num)

#index = []
#for i in range(num):
#    index.append(0)

#print(index)

#for i in range(width):
#    for j in range(height):
#        for k in range(num):
#            if (data[i][j] == k):
#                index[k] = index[k] + 1

#print(index)

#for k in range(1, len(index)):
#    if (index[k] > 300) or (index[k] < 150):
#        for i in range(1, width):
#            for j in range(1, height):
#                if (edit[i][j] == k):
#                    edit[i][j] = 0
#    else:
#        letter_index = k

#write(edit, 'letter_only.txt')
#print(letter_index)

#n = 3   #Dimensionen
#m = 4   #Elemente
#a = [[0] * m for i in range(n)]
#print(a)

#img = Image.open('images/#/image1_1.jpg')
#rotate = img.transpose(Image.ROTATE_90)
#rotate.save('image1_1-rotate.jpg')
os.chdir('/Robocup_2020/')

function.clear('images/insert')
function.clear('images/#')
function.clear('images/letter/H')
function.clear('images/letter/U')
function.clear('images/letter/S')

function.copy_images('images/all of data/data', 'images/insert')