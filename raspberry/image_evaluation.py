import cv2
import numpy as np
import image_function
from PIL import Image, ImageEnhance
import os

def take_picture(img_name, cam):
    debug = False
    path = '/home/pi/RoboCup2021/'
    img = Image.open(path + 'images/insert_' + str(cam) + '/' + img_name)       #liest Image ein
    img.thumbnail((100, 80))                            #verkleinert Image auf 128x96 Pixel (102x80)
    img = img.convert('L')                              #wandelt Bild in Graustufen um

    width, height  = img.size                            #liest groessee des Bildes aus

    gray = list(img.getdata())
    b = []
    for h in range(height):
        b.append([])
        for w in range(width):
            b[h].append(gray[h*width+w])

    if debug:
        image_function.write(b, 'original.txt')

    edit = []
    for i in range(0, int(height)):
        edit.append([])
    for h in range(0, int(height)):
        for w in range(0, int(width)):
            if (b[h][w] <= 50):# 50):  # Grenzwet: 75
                edit[h].append(1)
            else:
                edit[h].append(0)

	if debug:
		image_function.write(edit, 'edit.txt')

	smaller_edit = edit

	if debug:
		image_function.write(smaller_edit, 'smaller_edit.txt')

	height = len(smaller_edit) - 1
	width = len(smaller_edit[0]) - 1

	if debug:
		image_function.write(smaller_edit, 'border_less.txt')

	num = 1
	for i in range(0, width):
		for j in range(0, height):
			if (smaller_edit[j][i] == 1):
				num += 1
				image_function.blop_deleting(i, j, num, smaller_edit)

	if debug:
		image_function.write(smaller_edit, 'blop.txt')

	index = []
	for i in range(0, num + 1):  # Index Array wird mit dumping Werten gefuellt
		index.append(0)

	for i in range(0, height):  # das Array wird nach den Indexen abgesucht
		for j in range(0, width):  # Haeufigkeit des einzelnen Indexen wird in Array festgehalten
			for k in range(0, num + 1):
				if (smaller_edit[i][j] == k):
					index[k] = index[k] + 1

	if debug:
		for l in range(0, len(index)):
			print(str(l) + ': ' + str(index[l]))

	possible_blops = []
	for k in range(1, len(index)):
		if (index[k] < 10):#(index[k] > 200) or (index[k] < 10) or (width < 15) or (height < 15):  # maximal bzw minimal Groesse eines Blopes
			for i in range(1, width):
				for j in range(1, height):  # ueberschreibt jeden uebrigen Blop der zu Gross bzw. zu Klein ist mit Null
					if (smaller_edit[j][i] == k):
						smaller_edit[j][i] = 0
		else:
			possible_blops.append(k)

	if debug:
		image_function.write(smaller_edit, 'letter_only.txt')

	results = []
	for index in possible_blops:
		result = detect_letter(smaller_edit, index, img_name, cam, debug)
		if result != 'e':
			return result
			break

def detect_letter(smaller_edit, index, img_name, cam, debug):
    width = len(smaller_edit) - 1
    height = len(smaller_edit[0]) - 1

    black_1 = [width, height]  # links oben
    black_2 = [0, 0]  # rechts unten

    for i in range(0, width):
        for j in range(0, height):
            if (smaller_edit[i][j] == index):
                if (black_2[1] < j):
                    black_2[1] = j

    for j in range(0, height):
        for i in range(0, width):
            if (smaller_edit[i][j] == index):
                if (black_2[0] < i):
                    black_2[0] = i

    for i in range(0, width):
        for j in range(0, height):
            if (smaller_edit[width - i][height - j] == index):
                if (black_1[1] > height - j):
                    black_1[1] = height - j

    for j in range(0, height):
        for i in range(0, width):
            if (smaller_edit[width - i][height - j] == index):
                if (black_1[0] > width - i):
                    black_1[0] = width - i


    smaller_edit[black_1[0]][black_1[1]] = 111
    smaller_edit[black_2[0]][black_2[1]] = 222

    if debug:
        image_function.write(smaller_edit, 'point.txt')

    letter_height = black_2[0] - black_1[0] + 1
    letter_width = black_2[1] - black_1[1] + 1

    n = letter_width + 1  # x
    m = letter_height + 1  # y
    crop = [[0] * n for i in range(m)]  # erstellt Array mit dumping Werten in der Groesse des Bereichs des Buchstabens

    for i in range(0, letter_width):
        for j in range(0, letter_height):  # uebergibt Bereich um den Buchstaben in extra Array
            crop[j][i] = smaller_edit[black_1[0] + j][black_1[1] + i]

    if debug:
        image_function.write(crop, 'crop.txt')

    if (len(crop) > 0):
        height = len(crop) - 1
        width = len(crop[0]) - 1

        if (width > 15) and (height > 15):

            hit_1 = False
            hit_2 = False
            hit_3 = False
            hit_4 = False

            for k in range(0, int(round(width / 3))):
                if (crop[int(round(height / 2))][k] != 0):   # mitte Hoehe; links
                    hit_1 = True
                if (crop[int(round(height / 2))][width - k] != 0): # mitte Hoehe; rechts
                    hit_3 = True
            for k in range(0, int(round(height / 3))):
                if (crop[k][int(round(width / 2))] != 0):    # mitte Breite; oben
                    hit_2 = True
                if (crop[height - k][int(round(width / 2))] != 0):   # mitte Breite; unten
                    hit_4 = True

            if debug:
                print(hit_1)
                print(hit_2)
                print(hit_3)
                print(hit_4)

            letter = False

            if (hit_1) and not(hit_2) and (hit_3) and not(hit_4):
                letter = True
                return 'H'
            if (hit_2) and (hit_4):
                letter = True
                return 'S'
            if (hit_1) and (hit_2) and (hit_3) and not(hit_4):
                letter = True
                return 'U'
            if not (letter):
                return 'e'

        else:
            return 'e'

def colour_search(img_name, cam):
    count_red = 0
    count_green = 0
    count_yellow = 0
    
    #path = ''
    path = '/home/pi/RoboCup2021/'
    img = Image.open(path + 'images/insert_' + str(cam) + '/' + img_name)
    img.thumbnail((102, 80))	#verkleinert Image auf 128x96 Pixel (102x80)

    height, width = img.size    #liest groesse des Bildes aus

    colours = []
    colours = [[0] * height for i in range(width)]

    for i in range(1, width - 1):
        for j in range(1, height - 1):
            coordinates = x, y = j, i
            red, green, blue = img.getpixel(coordinates)
            if (red > 100) and (green < 50) and (blue < 50):
                colours[i][j] = 'r'
                count_red = count_red + 1
            else:
                if (green > 100) and (blue < 100) and (red < 50):
                    colours[i][j] = 'g'
                    count_green = count_green + 1
                else:
                    if (red > 150) and (green > 150) and (blue < 50):
                        colours[i][j] = 'y'
                        count_yellow = count_yellow + 1
                    else:
                        colours[i][j] = 'u'

    colour = False
    if (count_red > count_green) and (count_red > count_yellow) and (count_red > 10):
        colour = True
        return 'r'
    if (count_green > count_red) and (count_green > count_yellow) and (count_green > 10):
        colour = True
        return 'g'
    if (count_yellow > count_red) and (count_yellow > count_green) and (count_yellow > 10):
        colour = True
        return 'y'
    if not(colour):
        return 'e'
