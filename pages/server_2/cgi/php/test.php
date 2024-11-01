<?php
header("Content-Type: text/html");

$query_string = getenv('QUERY_STRING');

parse_str($query_string, $params);

$string = isset($params['text']) ? $params['text'] : '';
$key = isset($params['letter']) ? $params['letter'] : '';

function encrypt_string($string, $key) {
	$result = "";
	$shift = ord(strtolower($key)) - ord('a');
	$alphabetLength = 26;

	for ($i = 0; $i < strlen($string); $i++) {
		$char = $string[$i];

		if (ctype_alpha($char)) {
			$base = ctype_lower($char) ? 'a' : 'A';
			$result .= chr(((ord($char) - ord($base) + $shift) % $alphabetLength) + ord($base));
		} else {
			$result .= $char;
		}
	}
	return $result;
}

if (empty($string) || empty($key) || strlen($key) != 1 || !ctype_alpha($key)) {
	echo "Invalid input. Please provide a valid string and a single letter key.";
	exit;
}

$encrypted_string = encrypt_string($string, $key);

echo <<<HTML
<!DOCTYPE html>
<html lang="fr">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>CGI Result | Webserv</title>
	<style>
		body {
			font-family: Arial, sans-serif;
			margin: 0;
			padding: 0;
			display: flex;
			justify-content: center;
			align-items: center;
			height: 100vh;
			background-color: #000;
		}
		h1 {
			color: #f4f4f4;
			text-align: center;
		}
		.buttons {
			margin-top: 20px;
			display: flex;
			justify-content: center;
		}
		.buttons a {
			text-decoration: none;
			color: white;
			background-color: grey;
			padding: 10px 20px;
			margin: 0 10px;
			border-radius: 5px;
			display: inline-block;
		}
		.buttons a:hover {
			background-color: #0056b3;
		}
	</style>
</head>
<body>
	<div>
		<h1>🔐 Encrypted Text: {$encrypted_string} 🔐</h1>
		<div class="buttons">
			<a href="../cgi_php.html" role="button">Back to Form</a>
		</div>
	</div>
</body>
</html>
HTML;
?>
