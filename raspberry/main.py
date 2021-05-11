import os
import take_image
import edit_images
import output

#########################################
# nimmt Bilder auf und speichert sie ab #
#########################################

#take_image.take_image(10,5) #num,pause

#########################
# helllt die Bilder auf #
#########################

#data_light = os.listdir('images/insert')
#for i in range(len(data_light)):
#    edit_images.light_image(data_light[i])	

#################################
# sortiert zu dunkle Bilder aus #
#################################

#data_sort = os.listdir('images/insert')
#for i in range(len(data_sort)):
#	edit_images.sort_image(data_sort[i])

###############################
# sucht die Buchstaben heraus #
###############################

#data_letter = os.listdir('images/insert')
#for i in range(len(data_letter)):
#	edit_images.letter_search(data_letter[i])

#############################################
# sucht in den uebrigen Bildern nach Farben #
#############################################

#data_colour = os.listdir('images/#')
#for i in range(len(data_colour)):
#	edit_images.colour_search(data_colour[i])

data = os.listdir('images/insert')
for i in range(len(data)):
	print('###############')
	state_letter = 'e'
	state_colour = 'e'
	state = True

	edit_images.light_image(data[i])
	state = edit_images.sort_image(data[i])
	print(state)
	if (state):
		state_letter = edit_images.letter_search(data[i])

	output.output(state_letter)

	#data_colour = os.listdir('images/#')
	#for i in range(len(data_colour)):
	#	state_colour = edit_images.colour_search(data_colour[i])

	#if (state_letter != 'e') and (state_colour == 'e'):
	#	output.output(state_letter)
	#if (state_letter == 'e') and (state_colour != 'e'):
	#	output.output(state_colour)
	#if (state_letter == 'e') and (state_colour == 'e'):
	#	output.output('e')