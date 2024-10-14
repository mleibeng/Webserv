<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>PHP Test Page</title>
</head>
<body>
	<h1>Welcome to My PHP Test Page</h1>
	<p>This is an HTML paragraph.</p>

	<?php
	// PHP code starts here
	$message = "Hello, World!";
	echo "<p>$message</p>";

	// Display the current date and time
	echo "<p>Current date and time: " . date('Y-m-d H:i:s') . "</p>";
	?>

	<p>This is another HTML paragraph.</p>
</body>
</html>
