<?php
    include 'connect.php';

    // The HTTP GET SENT BY ARDUINO/SIM900 IS LIKE THIS:
    // http://mywebsite.com/receiver.php?speed=22.1&direction=SE&temp=22.1

	$speed = $_GET['speed'];
	$direction = $_GET['direction'];
	$temp = $_GET['temp'];
	
        // A Simple sql query to store the data in a table
		$sql = "UPDATE measures SET speed='$speed', direction='$direction', temperature='$temp' WHERE id=1";

		if ($conn->query($sql) === TRUE) {
			echo "OK";
			} else {
				echo "ERR " . $conn->error;
				}
?>