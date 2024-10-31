<?php
// No headers output - let the server handle it
echo "<html><body>\n";
echo "<h1>CGI Test Page</h1>\n";
echo "Query string received: " . $_SERVER['QUERY_STRING'] . "<br>\n";
echo "Script filename: " . $_SERVER['SCRIPT_FILENAME'] . "<br>\n";

parse_str($_SERVER['QUERY_STRING'], $params);
if (!empty($params)) {
    echo "<h2>GET Parameters:</h2>\n";
    foreach ($params as $key => $value) {
        echo htmlspecialchars($key) . " = " . htmlspecialchars($value) . "<br>\n";
    }
}
echo "</body></html>\n";
?>
