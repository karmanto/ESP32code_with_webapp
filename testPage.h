//------------------------------------------------------------------------------------------
const char testPageSetting[] PROGMEM = R"=====(


<!DOCTYPE HTML>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body style="background-color: black; text-align:center; color:aqua; font-family: sans-serif;">
<p id="testId"></p>

<script>
  setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = this.responseText;
      document.getElementById("testId").innerHTML = x;
    }
  };
  xhttp.open("GET", "/testData", true);
  xhttp.send();
  }, 250 ) ;
  </script>
  
</body>
</html>


)=====";

//------------------------------------------------------------------------------------------
