<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
      "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">

   <head>		
      <title> A Simple Anemometer Page </title> 
	  <link rel="stylesheet" type="text/css" href="mystyle.css">
   </head>
   
	<div class="MainContainer">
		<div class="container">
		<div class="container-compass">
			<img src="resources/compass.png" class="compass">
			<img src="resources/compass-pointer.png" class="compass-pointer" id="compass-pointer" >
		
			<?php
				include 'connect.php';
	  
				$sql_direction = "SELECT direction FROM measures";
				$result_direction = mysqli_query($conn, $sql_direction);
				
                if (mysqli_num_rows($result_direction) > 0) {
                    // output data of each row
                    while($row = mysqli_fetch_assoc($result_direction)) {
                        //echo "direction: " . $row["direction"]. "<br>";
                        $GLOBALS['direction'] = $row["direction"];
                    }
				} 
                else { echo "0 results"; } 
            ?>
		</div>
		
		<script type="text/javascript">
			var looper1;
			var degrees1 = 3;
			var NE_limit = 45;
			var NN_limit = 4;
			var EE_limit = 90;
			var SE_limit = 135;
			var SS_limit = 180;
			var SO_limit = 225;
			var OO_limit = 270;
			var NO_limit = 315;
			var my_direction = <?php echo json_encode($GLOBALS['direction']); ?>;
		
			function rotateDirectionAnimation(el1, speed1) {
				var elem = document.getElementById("compass-pointer");
			
				if(my_direction == "NE"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > NE_limit) {
						degrees1--;
					}
				
				} else if(my_direction == "NN"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > NN_limit) {
						degrees1--;
					}
				} else if(my_direction == "EE"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > EE_limit) {
						degrees1--;
					}
				} else if(my_direction == "SE"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > SE_limit) {
						degrees1--;
					}
				} else if(my_direction == "SS"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > SS_limit) {
						degrees1--;
					}
				} else if(my_direction == "SO"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > SO_limit) {
						degrees1--;
					}
				} else if(my_direction == "OO"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > OO_limit) {
						degrees1--;
					}
				} else if(my_direction == "NO"){
					elem.style.WebkitTransform = "rotate("+degrees1+"deg)";
					looper1 = setTimeout('rotateDirectionAnimation(\''+el1+'\','+speed1+')',speed1);
					degrees1++;
					if(degrees1 > NO_limit) {
						degrees1--;
					}
				}
			
			}
		</script>
		
		<script>rotateDirectionAnimation("compass-pointer",10); 
		</script>
		
		<div class="container-anemometer">
			<img src="resources/anemometer1.png" class="anemometer">
			<img src="resources/anemometer-pointer1.png" class="anemometer-pointer" id="anem-pointer" >
			<div class="anemometer-speed">
			<?php
				include 'connect.php';
				$sql_speed = "SELECT speed FROM measures";
				$result_speed = mysqli_query($conn, $sql_speed);

				if (mysqli_num_rows($result_speed) > 0) {
				// output data of each row
				while($row = mysqli_fetch_assoc($result_speed)) {
				echo $row["speed"]. "<br>";
				$GLOBALS['speed'] = $row["speed"];
	
					}
				} else { echo "0 results"; } ?></div>
				
			<div class="knt"> knt </div>
		</div>
		
		<script type="text/javascript">
			var looper; 
			var degrees = 3;
			var my_speed = <?php echo json_encode($GLOBALS['speed']); ?>;
			var limit = my_speed*4.90; //wind_velocity parameter on 50
		
		
			function rotateSpeedAnimation(el, speed) {
				var elem = document.getElementById("anem-pointer");
				elem.style.WebkitTransform = "rotate("+degrees+"deg)";
				looper = setTimeout('rotateSpeedAnimation(\''+el+'\','+speed+')',speed);
				degrees++;
				if(degrees > limit) {
					degrees--;
				}
			}
		</script>
		
		<script>rotateSpeedAnimation("anem-pointer",15); 
		</script>
	
		<div class="container-temp">
			<img src="resources/term.png" class="term">
			<img src="resources/term-pointer.png" class="term-pointer" id="temp">
		
			<?php
				include 'connect.php';
				$sql_temperature = "SELECT temperature FROM measures";
				$result_temperature = mysqli_query($conn, $sql_temperature);

				if (mysqli_num_rows($result_temperature) > 0) {
				// output data of each row
				while($row = mysqli_fetch_assoc($result_temperature)) {
				$GLOBALS['temperature'] = $row["temperature"];
					}
				} else { echo "0 results"; } ?>
		</div>
		
		<script>(function(d, s, id) {
			var js, fjs = d.getElementsByTagName(s)[0];
			if (d.getElementById(id)) return;
			js = d.createElement(s); js.id = id;
			js.src = "//connect.facebook.net/it_IT/sdk.js#xfbml=1&version=v2.8";
			fjs.parentNode.insertBefore(js, fjs);
			}
            (document, 'script', 'facebook-jssdk'));
		</script>

		<script type="text/javascript">
		
			var my_temp = <?php echo json_encode($GLOBALS['temperature']); ?>;
			var my_temp1 = Math.floor(my_temp);
			var my_tempa = (my_temp1+8) + '%';
			var my_tempb = (my_temp1+6) + '%';
			var my_temp2 = (my_temp1+4) + '%';
			var my_temp3 = (my_temp1+3) + '%';
			var my_temp4 = (my_temp1+1) + '%';
			var my_temp5 = (my_temp1) + '%';
			var my_temp6 = (my_temp1-4) + '%';
		
			if(my_temp1 <= 0) {
			document.getElementById('temp').style.width = '0';
			} else if(my_temp1 >= 0 && my_temp1<=4) {
				document.getElementById('temp').style.width = my_tempa;
			} else if(my_temp1 > 4 && my_temp1<=10) {
				document.getElementById('temp').style.width = my_tempb;
			} else if(my_temp1 > 10 && my_temp1<= 19) {
				document.getElementById('temp').style.width = my_temp2;	
			} else if(my_temp1 > 19 && my_temp1 <= 25) {
				document.getElementById('temp').style.width = my_temp3;
			} else if(my_temp1 > 25 && my_temp1 <= 27) {
				document.getElementById('temp').style.width = my_temp4;
			} else if(my_temp1 > 27 && my_temp1 <= 32) {
				document.getElementById('temp').style.width = my_temp5;
			} else if(my_temp1 > 32 && my_temp1 <= 39) {
				document.getElementById('temp').style.width = my_temp6;
			} else if(my_temp1 > 40 && my_temp1 <= 45) {
				document.getElementById('temp').style.width = my_temp6;
			} else if(my_temp1 > 45) {
				document.getElementById('temp').style.width = '50%';
			} 
		</script>
		</div>
		&nbsp <br>
	</div>		
</html>