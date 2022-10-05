//------------------------------------------------------------------------------------------
const char timeSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN JAM DAN TANGGAL</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <div class="form">
    <div class="title">PENGATURAN JAM DAN TANGGAL</div>
      <form action="/get">   
        <div class="input-container ic1">
          <input type="time" name="time" class="input" id="timeId" required>
          <div class="cut"></div>
          <label class="placeholder">Jam</label>
        </div>
        <div class="input-container ic2">
          <input type="date" name="date" class="input" id="dateId" required>
          <div class="cut"></div>
          <label class="placeholder">Tanggal</label>
        </div>
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  var date = new Date();

  var day = date.getDate();
  var month = date.getMonth() + 1;
  var year = date.getFullYear();
  var hour = date.getHours();
  var minute = date.getMinutes();

  if (month < 10) month = "0" + month;
  if (day < 10) day = "0" + day;
  if (hour < 10) hour = "0" + hour;
  if (minute < 10) minute = "0" + minute;

  var today = year + "-" + month + "-" + day;
  var totime = hour + ":" + minute;
  document.getElementById("dateId").value = today;
  document.getElementById("timeId").value = totime;  
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
