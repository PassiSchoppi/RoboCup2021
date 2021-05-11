import os
import shutil
from PIL import Image, ImageEnhance
import function 

def light_image(img_name):
	img = Image.open('images/insert/' + img_name)
	enhancer = ImageEnhance.Brightness(img)
	enhanced_img = enhancer.enhance(5)
	os.remove('images/insert/' + img_name)
	enhanced_img.save('images/insert/' + img_name)

def sort_image(img_name):
	
	img = Image.open('images/insert/' + img_name)       #liest Image ein
	img.thumbnail((102, 80))                            #verkleinert Image auf 128x96 Pixel (102x80)
	img = img.convert('L')                              #wandelt Bild in Graustufen um

	height, width = img.size                            #liest groesse des Bildes aus

	data = list(img.getdata())
	data = [data[offset:offset+height] for offset in range(0, width*height, height)] #erzeugt ein Zweidimensionales Array mit den Helligkeitswerten des entsprechenden Pixels

	brightness = 0

	for i in range(width):
		for j in range(height):
			brightness = brightness + data[i][j]
	
	proportion = brightness / (width * height)
	print(img_name)
	print(proportion)
	print('')

	if (proportion < 200):
		shutil.copyfile('images/insert/' + img_name, 'images/#/' + img_name)
		return False
	else:
		return True

def blop_search(index, edit, img_name):
	function.write(edit, '/letter_only/letter_only_' + img_name + '_' + str(index) +  '.txt') #schreibt Datei in der eigentlich nur noch der Buchstabe beschriftet sein sollte und der Rest Null	
	
	width = len(edit)
	height = len(edit[0])

	black_1 = [width - 1, height - 1]     # links oben
	black_2 = [0, 0]     # rechts unten
	
	for i in range(2, width - 2):
		for j in range(2, height - 2):
			if (edit[i][j] == index):
				if (black_2[1] < j):
					black_2[1] = j

	for j in range(2, height - 2):
		for i in range(2, width - 2):
			if (edit[i][j] == index):
				if (black_2[0] < i):
					black_2[0] = i

	for i in range(2, width - 2):
		for j in range(2, height - 2):
			if (edit[width - i][height - j] == index):
				if (black_1[1] > height - j):
					black_1[1] = height - j

	for j in range(2, height - 2):
		for i in range(2, width - 2):
			if (edit[width - i][height - j] == index):
				if (black_1[0] > width - i):
					black_1[0] = width - i
				
	#print(black_1)
	#print(black_2)

	#edit[black_1[0]][black_1[1]] = 111
	#edit[black_2[0]][black_2[1]] = 222

	#function.write(edit, '/point/point_' + str(index) + '_' + img_name +  '.txt')
	
	letter_width = black_2[0] - black_1[0]
	letter_height = black_2[1] - black_1[1]

	n = letter_width + 2    #x
	m = letter_height + 2   #y
	crop = [[0] * m for i in range(n)]   #erstellt Array mit dumping Werten in der Groesse des Bereichs des Buchstabens
	
	#print(letter_width)
	#print(letter_height)
	
	for i in range(letter_width):
		for j in range(letter_height):                    #uebergibt Bereich um den Buchstaben in extra Array
			crop[i + 1][j + 1] = edit[black_1[0] + i][black_1[1] + j]
			
	function.write(crop, '/crop/crop_' + img_name +'_' + str(index) + '.txt')    #schreibt Bereich um Buchstaben in Datei
	
	if (len(crop) > 0):
		width = len(crop)
		height = len(crop[1])
		letter = 0
		emty = 0

		hit_1 = False
		hit_2 = False
		hit_3 = False
		hit_4 = False

		for k in range(1, round(width / 3)):
			if (crop[k][round(height / 2)] != 0):
				hit_1 = True
			if (crop[width - k][round(height / 2)] != 0):
				hit_3 = True
		for k in range(1, round(height / 3)):
			if (crop[round(width / 2)][k] != 0):
				hit_2 = True
			if (crop[round(width / 2)][height - k] != 0):
				hit_4 = True

		print(img_name)
		print('Hit_1: ' + str(hit_1))
		print('Hit_2: ' + str(hit_2))
		print('Hit_3: ' + str(hit_3))
		print('Hit_4: ' + str(hit_4))
	
		letter = False

		if not(hit_1) and (hit_2) and not(hit_3) and not(hit_4):
			print('H')
			shutil.copyfile('images/insert/' + img_name, 'images/letter/H/' + img_name)
			letter = True
			return 'H'
		if (hit_1) and not(hit_2) and (hit_3) and (hit_4):
			print('S')
			shutil.copyfile('images/insert/' + img_name, 'images/letter/S/' + img_name)
			letter = True
			return 'S'
		if not(hit_1) and (hit_2) and (hit_3) and (hit_4):
			print('U')
			shutil.copyfile('images/insert/' + img_name, 'images/letter/U/' + img_name)
			letter = True
			return 'U'
		if not(letter):
			shutil.copyfile('images/insert/' + img_name, 'images/#/' + img_name)
			print('no letter')
			return 'e'
	
		print('')
	else:
		shutil.copyfile('images/insert/' + img_name, 'images/#/' + img_name)
		print('weg')
		print('')
		return 'e'


def letter_search(img_name):

	img = Image.open('images/insert/' + img_name)       #liest Image ein
	img.thumbnail((102, 80))                            #verkleinert Image auf 128x96 Pixel (102x80)
	img = img.convert('L')                              #wandelt Bild in Graustufen um
	
	height, width = img.size                            #liest groessee des Bildes aus

	data = list(img.getdata())
	data = [data[offset:offset+height] for offset in range(0, width*height, height)] #erzeugt ein Zweidimensionales Array mit den Helligkeitswerten des entsprechenden Pixels

	#print('Bild:' + img_name)
	#print('Format:' + str(width) + '/' + str(height))

	edit = data

	function.write(data, '/unbearbeitet/unbearbeitet_' + img_name + '.txt')

	for i in range(width):          #geht jeden Farbwert des Arrays durch, unter einem bestimmten Grenzwert wird dieser Pixel als 1 bzw. schwarz definiert
		for j in range(height):     #ueber dem Grenzwert als weiss bzw. 0
			if (edit[i][j] < 200):   #Grenzwet: 75
				edit[i][j] = 1
			else:
				edit[i][j] = 0

	function.write(edit, '/row/row_' + img_name + '.txt') #schreibt Textdatei des Bildes mit 1 und 0

	num = 1
	for i  in range(width):         #sucht nach schwarzen Flaechen(blops)
		for j in range(height):     #jeder Blop erhaelt einen eigenen Index
			if (edit[i][j] == 1):
				num = num + 1
				function.blop_naming(i, j, num, height, width, edit)
	
	function.write(edit, '/letter/letter_' + img_name + '.txt') #schreibrt Datei mit duchrnummerierten Blops

	for i in range(height):     #geht die Raender des Bildes ab   
		if (edit[0][i] >= 1):   #sucht nach Blops, die am Rand liegen
			function.blop_deleting(0, i, 0, height, width, edit)    #ueberschreibt diese Blops rekursiev mit Null
		if (edit[width - 1][i] >= 1):
			function.blop_deleting(width - 1, i, 0, height, width, edit)
			
				
	index = []
	for i in range(0, num + 1):    #Index Array wird it dumping Werten gefuellt
		index.append(0)

	for i in range(0, width):       #das Array wird nach den Indexen abgesucht
		for j in range(0, height):  #Haeufigkeit des einzelnen Indexen wird in Array festgehalten
			for k in range(0, num + 1):
				if (data[i][j] == k):
					index[k] = index[k] + 1

	#print(img_name)
	#for l in range(0, len(index)):
	#	print(str(l) + ': ' + str(index[l]))

	#letter_index = 0

	index2 = []

	for k in range(1, len(index)):
		if (index[k] > 300) or (index[k] < 30):	#maximal bzw minimal Groesse eines Blopes
			for i in range(1, width):
				for j in range(1, height):      #ueberschreibt jeden uebrigen Blop der zu Gross bzw. zu Klein ist mit Null
					if (edit[i][j] == k):
						edit[i][j] = 0
		else:
			index2.append(k)

	for k in range(len(index2)):
		blop_search(index2[k], edit, img_name)

def colour_search(img_name):
	count_red = 0
	count_green = 0
	count_yellow = 0

	img = Image.open('images/#/' + img_name)
	img.thumbnail((102, 80))	#verkleinert Image auf 128x96 Pixel (102x80)

	height, width = img.size    #liest groesse des Bildes aus

	colours = []
	colours = [[0] * height for i in range(width)]

	for i in range(1, width - 1):
		for j in range(1, height - 1):
			red, green, blue = img.getpixel(i,j)
			#print('Kooardinaten: ' + str(i) + ' | ' + str(j) + ' _ ' + str(red) + ', ' + str(green) + ', ' + str(blue))
			if (red > green) and (red > blue) and (green < 100) and (blue < 100):
				colours[i][j] = 'r'
				#print('red')
				count_red = count_red + 1
			else:
				if (green > blue) and (green > red) and (blue < 100) and (red < 100):
					colours[i][j] = 'g'
					#print('green')
					count_green = count_green + 1
				else:
					if (red > blue) and (green > blue) and (blue < 100):
						colours[i][j] = 'y'
						#print('yellow')
						count_yellow = count_yellow + 1
					else:
						colours[i][j] = 'u'
						#print('empty')

	#function.write(colours, 'colour_' + img_name + '.txt')
	colour = False
	if (count_red > count_green) and (count_red > count_yellow):
		shutil.copyfile('images/insert/' + img_name, 'images/colour/red' + img_name)
		colour = True
		return 'r'
	if (count_green > count_red) and (count_green > count_yellow):
		shutil.copyfile('images/insert/' + img_name, 'images/colour/green' + img_name)
		colour = True
		return 'g'
	if (count_yellow > count_red) and (count_yellow > count_green):
		shutil.copyfile('images/insert/' + img_name, 'images/colour/yellow' + img_name)
		colour = True
		return 'y'
	if not(colour):
		return 'e'