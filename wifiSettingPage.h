//------------------------------------------------------------------------------------------
const char wifiSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN WIFI</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="wifiPassword"  height="200" width="300" id="wifiPassword" style="display:none"></iframe>
  <div class="form">
    <div class="title">PENGATURAN WIFI</div>
      <form action="/get">   
        <div class="input-container ic1">
          <input type="text" id="wifiNameId" class="input" name="wifiName" pattern="[A-Za-z0-9]{3,20}" required>
          <div class="cut"></div>
          <label class="placeholder">Nama Wifi</label>
        </div>
        <div class="input-container ic2">
          <input type="text" id="wifiPasswordId" class="input" name="wifiPassword" pattern="[A-Za-z0-9]{8,20}" required>
          <div class="cut"></div>
          <label class="placeholder">Password Wifi</label>
        </div>
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  var iframe = document.getElementById('wifiPassword');
  var content = iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  document.getElementById("wifiNameId").value = contentArr[0];
  document.getElementById("wifiPasswordId").value = contentArr[1];
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
