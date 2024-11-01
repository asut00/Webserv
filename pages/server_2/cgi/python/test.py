# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    test.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thelaine <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 11:37:49 by thelaine          #+#    #+#              #
#    Updated: 2024/10/01 11:40:16 by thelaine         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/usr/bin/env python3
import cgi
import cgitb
import time

# Active l'affichage des erreurs CGI
cgitb.enable()

# Lire les paramètres de l'URL (les données envoyées via GET)
form = cgi.FieldStorage()

# Récupérer le nombre soumis via le formulaire
number = form.getvalue('number')

# Vérifier si un nombre a été soumis
if number:
	try:
		# while(1):
		#     print("coucou")
		#     time.sleep(1)
		number = int(number)
		result = number * 2

		# Générer la réponse HTML
		html_content = f"""
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
					<h1>🧮 The double of {number} is {result} 🧮</h1>
					<div class="buttons">
						<a href="../cgi_py.html" role="button">Back to Form</a>
					</div>
				</div>
			</body>
			</html>
		"""

		# Écrire la réponse dans un fichier
		print(html_content)
		# with open("./pages/cgi_answer_template.html", "w") as file:
		#     # file.write("Content-type: text/html\n\n")  # Optionnel si vous écrivez uniquement du HTML
		#     file.write(html_content)

	except ValueError:
		html_error_content = """
		<!DOCTYPE html>
		<html>
		<head>
			<title>Erreur</title>
		</head>
		<body>
			<h1>Erreur : Veuillez entrer un entier valide.</h1>
		</body>
		</html>
		"""
		print(html_error_content)
