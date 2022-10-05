//------------------------------------------------------------------------------------------
const char alarmAck[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>ALARM ACK</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">

  <div class="form">
    <div class="title">ALARM ACK</div>
    <button class="submit" onclick="alarmAckButton()">Alarm Ack Button</button>
  </div>

<script>
function alarmAckButton(){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update2?state=1", true); 
  xhr.send();
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
