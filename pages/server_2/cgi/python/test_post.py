# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    ImageProcessor.py                                  :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thelaine <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/13 09:57:17 by thelaine          #+#    #+#              #
#    Updated: 2024/06/14 11:11:22 by thelaine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

from PIL import Image
import matplotlib.pyplot as plt
import numpy as np
import sys
import io
import os

class ImageProcessor:
	def load(self, path):
		image_array = Image.open(path)
		image_array = np.array(image_array)
		print(f"loading {len(image_array)}x{len(image_array[0])}")
		return image_array
	def display(self, array):
		plt.imshow(array)
		plt.axis('off')
		plt.show()

class ScrapBooker:
	def crop(self, array, dim, position=(0,0)):
		array = array[position[0]:(dim[0]+position[0]), position[1]:(dim[1] + position[1])]
		#array = array[:(dim[0]+position[0]), :(dim[1] + position[1])]
		print(f"loading {len(array)}x{len(array[0])}")
		return array
	def thin(self, array, n, axis):
		if axis == 1:
			for i in range(len(array)//n):
				array = np.delete(array, i * n, axis)
		else:
			for i in range(len(array[0])//n):
				array = np.delete(array, i * n, axis)
		return array
	def juxtapose(self, array, n, axis):
		final_array = array
		for i in range(n - 1):
			final_array = np.concatenate((final_array, array), axis)
		return final_array
	def mosaic(self, array, dim):
		final_array = self.juxtapose(array, dim[0], 1)
		final_array = self.juxtapose(final_array, dim[1], 0)
		return final_array
	def juxtapose_script(self, array, array_2, axis):
		final_array = np.concatenate((array, array_2), axis)
		return final_array
	def mosaic_script(self, array, array_2, array_3, array_4):
		top_array = self.juxtapose_script(array, array_2, 1)
		bottom_array = self.juxtapose_script(array_3, array_4, 1)
		final_array = self.juxtapose_script(top_array, bottom_array, 0)
		return final_array

class ColorFilter:
	def  invert(self, array):
		final_array = array
		for i in range(len(final_array) - 1):
			for j in range(len(final_array[0]) - 1):
				r, g, b = final_array[i][j][0], final_array[i][j][1], final_array[i][j][2]
				final_array[i][j][0], final_array[i][j][1], final_array[i][j][2] = 255 - r, 255 - g, 255 - b
				#print(f"nous avons array[{i}][{j}] = {array[i][j]}")
		return (final_array)
	def to_blue(self, array):
		final_array = array
		for i in range(len(final_array) - 1):
			for j in range(len(final_array[0]) - 1):
				r, g, b = final_array[i][j][0], final_array[i][j][1], final_array[i][j][2]
				final_array[i][j][0], final_array[i][j][1], final_array[i][j][2] = 0, 0, b
				#print(f"nous avons array[{i}][{j}] = {array[i][j]}")
		return (final_array)
	def to_green(self, array):
		for i in range(len(array) - 1):
			for j in range(len(array[0]) - 1):
				r, g, b = array[i][j][0], array[i][j][1], array[i][j][2]
				array[i][j][0], array[i][j][1], array[i][j][2] = 0, g, 0
				#print(f"nous avons array[{i}][{j}] = {array[i][j]}")
		return (array)
	def to_red(self, array):
		for i in range(len(array) - 1):
			for j in range(len(array[0]) - 1):
				r, g, b = array[i][j][0], array[i][j][1], array[i][j][2]
				array[i][j][0], array[i][j][1], array[i][j][2] = r, 0, 0
				#print(f"nous avons array[{i}][{j}] = {array[i][j]}")
		return (array)
	#def to_celluloid(self, array):
	def to_grayscale(self, array, filter, **kwargs):
		for i in range(len(array) - 1):
			for j in range(len(array[0]) - 1):
				r, g, b = array[i][j][0], array[i][j][1], array[i][j][2]
				if filter == 'mean' or filter == 'm':
					array[i][j][3] = int((r + g + b)/3)
				elif filter == 'weigth' or filter == 'w':
					array[i][j][3] = int(kwargs['weights'][0] * r +  kwargs['weights'][1] * g + kwargs['weights'][2] * b)
				#print(f"nous avons array[{i}][{j}] = {array[i][j]}")
		return array

def generate_page(message):
		return f"""\
				<!DOCTYPE html>
			<html lang="fr">
			<head>
				<meta charset="UTF-8">
				<meta name="viewport" content="width=device-width, initial-scale=1.0">
				<title>CGI Result | Webserv</title>
				<style>
					body {{
						font-family: Arial, sans-serif;
						margin: 0;
						padding: 0;
						display: flex;
						justify-content: center;
						align-items: center;
						height: 100vh;
						background-color: #000;
					}}
					h1 {{
						color: #f4f4f4;
						text-align: center;
					}}
					.buttons {{
						margin-top: 20px;
						display: flex;
						justify-content: center;
					}}
					.buttons a {{
						text-decoration: none;
						color: white;
						background-color: grey;
						padding: 10px 20px;
						margin: 0 10px;
						border-radius: 5px;
						display: inline-block;
					}}
					.buttons a:hover {{
						background-color: #0056b3;
					}}
				</style>
			</head>
			<body>
				<div>
					<h1>{message}</h1>
					<div class="buttons">
						<a href="../post_cgi_py.html" role="button">Back to Form</a>
					</div>
				</div>
			</body>
			</html>
		"""

def read_image_from_stdin():
	content_length = os.environ.get('CONTENT_LENGTH')

	if content_length:
		try:
			# Convertir la taille en entier
			content_length = int(content_length.strip())

			# Lire exactement 'content_length' octets depuis stdin
			image_data = sys.stdin.buffer.read(content_length)

			# Convertir les données en texte pour pouvoir gérer les boundaries et headers
			body = image_data.decode('latin1', errors='ignore')  # Utiliser 'latin1' pour manipuler les données binaires comme du texte brut

			# Séparer les différentes parties du body avec la boundary
			boundary = os.environ.get('CONTENT_TYPE').split('boundary=')[1]
			parts = body.split('--' + boundary)

			for part in parts:
				# Chercher la partie contenant l'image en vérifiant si elle contient un Content-Disposition
				if "Content-Disposition" in part and "filename" in part:
					# Séparer les headers du contenu binaire
					headers, image_binary_data = part.split("\r\n\r\n", 1)

					# Les headers se terminent par deux retours à la ligne, le reste est l'image
					image_binary_data = image_binary_data.rstrip('--\r\n')  # Enlever les extra whitespaces

					# Convertir les données en bytes pour charger avec Pillow
					image_data_bytes = image_binary_data.encode('latin1')  # latin1 to handle raw bytes

					# Charger les données binaires dans un objet image avec Pillow
					try:
						image = Image.open(io.BytesIO(image_data_bytes))
						print(generate_page("Success to load the image"))
						return image
					except IOError:
						print(generate_page("Error: error while loading the image"))
						return None

			print(generate_page("Error: No image found in the multipart body"))
			return None

		except ValueError:
			print(generate_page("Error: CONTENT LENGTH is not a valid int"))
			return None
	else:
		print(generate_page("Error: Missing content length"))
		return None

# Lire et afficher l'image
CF = ColorFilter()
SB = ScrapBooker()
image = read_image_from_stdin()
if image:
	array = np.asarray(image)
	array_invert = CF.invert(array.copy())
	array_blue = CF.to_blue(array.copy())
	array_red = CF.to_red(array.copy())
	array_green = CF.to_green(array.copy())
	# array = SB.juxtapose_script(array, array, 0)
	final_array = SB.mosaic_script(array_invert, array_blue, array_green, array_red)

	# array = SB.mosaic_script(CF.invert(array), CF.to_red(array), CF.to_blue(array), CF.to_green(array))
	image = Image.fromarray(final_array)
	image.save('pages/server_1/post/cgi_post_script_result.jpg')

