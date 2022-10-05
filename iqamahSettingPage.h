//------------------------------------------------------------------------------------------
const char iqamahSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN WAKTU IQAMAH</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="timeIqamah" height="200" width="300" id="timeIqamah" style="display:none"></iframe>
  <div class="form">
    <div class="title">PENGATURAN WAKTU IQAMAH</div>
      <form action="/get">   
        <input type="checkbox" id="iqamahSettingCBId" name="iqamahSettingCB" value="1" style="visibility:hidden;"> 
        <div class="input-container ic1">
          <select id="iqamahSubuhId" class="input" name="iqamahSubuh">
            <option value="0">tidak digunakan</option>
            <option value="5">5 menit</option>
            <option value="10">10 menit</option>
            <option value="15">15 menit</option>
            <option value="20">20 menit</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Iqamah Subuh</label>
        </div>
        <div class="input-container ic2">
          <select id="iqamahZuhurId" class="input" name="iqamahZuhur">
            <option value="0">tidak digunakan</option>
            <option value="5">5 menit</option>
            <option value="10">10 menit</option>
            <option value="15">15 menit</option>
            <option value="20">20 menit</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Iqamah Zuhur</label>
        </div>
        <div class="input-container ic2">
          <select id="iqamahAsarId" class="input" name="iqamahAsar">
            <option value="0">tidak digunakan</option>
            <option value="5">5 menit</option>
            <option value="10">10 menit</option>
            <option value="15">15 menit</option>
            <option value="20">20 menit</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Iqamah Asar</label>
        </div>
        <div class="input-container ic2">
          <select id="iqamahMagribId" class="input" name="iqamahMagrib">
            <option value="0">tidak digunakan</option>
            <option value="5">5 menit</option>
            <option value="10">10 menit</option>
            <option value="15">15 menit</option>
            <option value="20">20 menit</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Iqamah Magrib</label>
        </div>
        <div class="input-container ic2">
          <select id="iqamahIsyaId" class="input" name="iqamahIsya">
            <option value="0">tidak digunakan</option>
            <option value="5">5 menit</option>
            <option value="10">10 menit</option>
            <option value="15">15 menit</option>
            <option value="20">20 menit</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Iqamah Isya</label>
        </div>
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  document.getElementById("iqamahSettingCBId").checked = true;
  var iframe = document.getElementById('timeIqamah');
  var content= iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  document.getElementById("iqamahSubuhId").value = contentArr[0];
  document.getElementById("iqamahZuhurId").value = contentArr[1];
  document.getElementById("iqamahAsarId").value = contentArr[2];
  document.getElementById("iqamahMagribId").value = contentArr[3];
  document.getElementById("iqamahIsyaId").value = contentArr[4];
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
