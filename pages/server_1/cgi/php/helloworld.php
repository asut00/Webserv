<?php
echo '
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
					<h1>Hello World!</h1>
					<div class="buttons">
						<a href="/cgi/index_cgi.html" role="button">Back to Index</a>
					</div>
				</div>
			</body>
			</html>
		';
?>
