import os

def copy_images(root_path, target_path):
    data = os.listdir(root_path)
    os.chdir(path)
    for i in range(len(data)):
        os.rename(root_path + data[i], target_path + data[i])
    
def clear(path):
    data = os.listdir(path)
    os.chdir(path)
    for i in range(len(data)):
        os.remove(data[i])

def blop_naming(x, y, num, height, width, edit): #benennt rekursiv alle schwarzen Pixel, die zusammen haengen, um
    #print(num)
    #print('x', x)
    #print('y', y)
    edit[x][y] = num
    
    if (x-1 >= 0) and (y-1 >= 0):
        if (edit[x-1][y-1] == 1):   #links oben
            blop_naming((x-1), (y-1), num, height, width, edit)
    if (y-1 >= 0):    
        if (edit[x][y-1] == 1): #mitte oben
            blop_naming(x, (y-1), num, height, width, edit)
    if (x+1 < width) and (y-1 >= 0):
        if (edit[x+1][y-1] == 1):   #rechts oben
            blop_naming((x+1), (y-1), num, height, width, edit)
    if (x-1 >= 0):
        if (edit[x-1][y] == 1):  #links mitte
            blop_naming((x-1), y, num, height, width, edit)
    if (x+1 < width):
        if (edit[x+1][y] == 1): #rechts mitte
            blop_naming((x+1), y, num, height, width, edit)
#    if (x-1 > 0) and (y+1 < height):
#        if (edit[x-1][y+1] == 1) or ((edit[x-1][y+1] != num) and (edit[x-1][y+1] != 0)):   #links unten
#            blop_naming((x-1), (y+1), num, height, width)
    if (y+1 < 0):
        if (edit[x][y+1] == 1): #mitte unten
            blop_naming(x, (y+1), num, height, width, edit)
    if (x+1 < width) and (y+1 < height):
        if (edit[x+1][y+1] == 1): #rechts unten
            blop_naming((x+1), (y+1), num, height, width, edit)


def blop_deleting(x, y, num, height, width, edit): #benennt rekursiv alle schwarzen Pixel, die zusammen haengen, um
    #print(num)
    edit[x][y] = num
    #print('k')
    if (x-1 >= 0) and (y-1 >= 0):
        if (edit[x-1][y-1] != 0):   #links oben
            blop_deleting((x-1), (y-1), num, height, width, edit)
    if (y-1 >= 0):    
        if (edit[x][y-1] != 0): #mitte oben
            blop_deleting(x, (y-1), num, height, width, edit)
    if (x+1 < width) and (y-1 >= 0):
        if (edit[x+1][y-1] != 0):   #rechts oben
            blop_deleting((x+1), (y-1), num, height, width, edit)
    if (x-1 > 0):
        if (edit[x-1][y] != 0):  #links mitte
            blop_deleting((x-1), y, num, height, width, edit)
            #print('l')
    if (x+1 < width):
        if (edit[x+1][y] != 0): #rechts mitte
            blop_deleting((x+1), y, num, height, width, edit)
            #print('r')
    #if (x-1 > 0) and (y+1 < height):
    #    if (edit[x-1][y+1] != 0):   #links unten
    #        blop_naming((x-1), (y+1), num, height, width, edit)
    if (y+1 <= 0):
        if (edit[x][y+1] != 0): #mitte unten
            blop_deleting(x, (y+1), num, height, width, edit)
    if (x+1 < width) and (y+1 < height):
        if (edit[x+1][y+1] != 0): #rechts unten
            blop_deleting((x+1), (y+1), num, height, width, edit)

def write(array, dataname):
    dataname = 'txt info/' + dataname
    try:
        os.remove(dataname)
    except:
        print('')

    file = open(dataname, 'a')

    for i in range(len(array)):
        file.write(str(array[i]) + '\n')
    file.close

def letter_crop_detection(i, j, edit, x_min, y_min, x_max, y_max, width, height):

    if (i-1 >= 0) and (j-1 >= 0):
        if (edit[i-1][j-1] != 0):   #links oben
            if (i - 1 < x_min):
                x_min = i - 1
            if (j - 1 < y_min):
                y_min = j - 1
                letter_crop_detection((i-1), (j-1), edit, x_min, y_min, x_max, y_max, width, height)

    if (j-1 >= 0):    
        if (edit[i][j-1] != 0): #mitte oben
            if (j - 1 < y_min):
                y_min = j - 1
                letter_crop_detection((i), (j-1), edit, x_min, y_min, x_max, y_max, width, height)

    if (i+1 < width) and (j-1 >= 0):
        if (edit[i+1][j-1] != 0):   #rechts oben
            if (i + 1 > x_max):
                x_max = i + 1
            if (j - 1 < y_min):
                y_min = j - 1
                letter_crop_detection((i+1), (j-1), edit, x_min, y_min, x_max, y_max, width, height)

    if (i-1 > 0):
        if (edit[i-1][j] != 0):  #links mitte
            if (i - 1 < x_min):
                x_min = i - 1
                letter_crop_detection((i-1), (j), edit, x_min, y_min, x_max, y_max, width, height)

    if (i+1 < width):
        if (edit[i+1][j] != 0): #rechts mitte
            if (i + 1 > x_max):
                x_max = i + 1
                letter_crop_detection((i+1), (j), edit, x_min, y_min, x_max, y_max, width, height)

    #if (x-1 > 0) and (y+1 < height):
    #    if (edit[x-1][y+1] != 0):   #links unten
    #        blop_naming((x-1), (y+1), num, height, width, edit)

    if (j+1 <= 0):
        if (edit[i][j+1] != 0): #mitte unten
            if (j + 1 > y_max):
                y_max = j + 1
                letter_crop_detection((i), (j+1), edit, x_min, y_min, x_max, y_max, width, height)

    if (i+1 < width) and (j+1 < height):
        if (edit[i+1][j+1] != 0): #rechts unten
            if (i + 1 > x_max):
                x_max = i + 1
            if (j + 1 > y_max):
                y_max = j + 1
                letter_crop_detection((i+1), (j+1), edit, x_min, y_min, x_max, y_max, width, height)

    xy = []
    xy = [x_min, y_min, x_max, y_max]
    return xy
