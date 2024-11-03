<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Datei-Upload-Bereich
    if (isset($_FILES["fileToUpload"])) {
        $target_dir = "uploads/";
        $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);

        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
            echo "Die Datei " . htmlspecialchars(basename($_FILES["fileToUpload"]["name"])) . " wurde erfolgreich hochgeladen.";
        } else {
            echo "Es gab einen Fehler beim Hochladen der Datei.";
        }
    }

    // Nachricht-Eintrag-Bereich
    if (isset($_POST['name']) && isset($_POST['message'])) {
        $name = htmlspecialchars($_POST['name']);
        $message = htmlspecialchars($_POST['message']);

        echo "<p>Danke, <strong>$name</strong>! Ihre Nachricht: \"$message\" wurde erfolgreich eingereicht.</p>";
    }
}
?>
