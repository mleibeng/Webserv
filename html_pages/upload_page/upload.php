<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $target_dir = "uploads/";
    $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);

    // Datei hochladen
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "Die Datei " . basename($_FILES["fileToUpload"]["name"]) . " wurde erfolgreich hochgeladen.";
    } else {
        echo "Es gab einen Fehler beim Hochladen der Datei.";
    }
}
?>
