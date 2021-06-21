import os
import sys
#import  global_variables

sys.setrecursionlimit(200000)

def blop_deleting(x, y, num, edit):  # benennt rekursiv alle schwarzen Pixel, die zusammen haengen, um
    height = len(edit)
    width = len(edit[0])

    edit[y][x] = num
    new = False

    if (y + 1 < height) and not(new):
        if (edit[y + 1][x] == 1) and (edit[y + 1][x] != num):  # unten mitte
            blop_deleting(x, (y + 1), num, edit)

    if (y - 1 >= 0) and not(new):
        if (edit[y - 1][x] == 1) and (edit[y - 1][x] != num):  # oben mitte
            blop_deleting(x, (y - 1), num, edit)

    if (x + 1 < width) and not(new):
        if (edit[y][x + 1] == 1) and (edit[y][x + 1] != num):  # mitte rechts
            blop_deleting((x + 1), y, num, edit)

    if (x - 1 >= 0) and not(new):
        if (edit[y][x - 1] == 1) and (edit[y][x - 1] != num):  # mitte links
            blop_deleting((x - 1), y, num, edit)

    if (x - 1 >= 0) and (y - 1 >= 0) and not(new):
        if (edit[y - 1][x - 1] == 1) and (edit[y - 1][x - 1] != num):  # links oben
            blop_deleting((x - 1), (y - 1), num, edit)

    if (x - 1 <= 0) and (y + 1 < height) and not(new):
        if (edit[y + 1][x - 1] == 1) and (edit[y + 1][x - 1] != num):  # unten links
            blop_deleting((x - 1), (y + 1), num, edit)

    if (x + 1 < width) and (y - 1 >= 0) and not(new):
        if (edit[y - 1][x + 1] == 1) and (edit[y - 1][x + 1] != num):   # oben rechts
            blop_deleting((x + 1), (y - 1), num, edit)

    if (x + 1 < width) and (y + 1 < height) and not(new):
        if (edit[y + 1][x + 1] == 1) and (edit[y + 1][x + 1] != num):  # unten rechts
            blop_deleting((x + 1), (y + 1), num, edit)

def write(array, dataname):
    path = os.path.dirname(os.path.abspath(__file__))
    #path = sys.path[-1].replace('\\', '/') + '/'
    dataname = path + '/txt_info/' + dataname
    try:
        os.remove(dataname)
    except:
        pass
        print('')

    file = open(dataname, 'w')
    for w in range(0, len(array) - 1):
        for h in range(0, len(array[0]) - 1):
            file.write(str(array[w][h]) + ', ')
        file.write('\n')
    file.close()
    #pass

#path = os.path.dirname(os.path.abspath(__file__))
#dataname = path + '/txt_info/'
#print(dataname)