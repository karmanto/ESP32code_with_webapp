//------------------------------------------------------------------------------------------
const char lisensiSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>LISENSI</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="serialNumber"  height="200" width="300" id="serialNumber" style="display:none"></iframe>
  <iframe src="lisensiSave"  height="200" width="300" id="lisensiSave" style="display:none"></iframe>
  <iframe src="lisensiStat"  height="200" width="300" id="lisensiStat" style="display:none"></iframe>
  <iframe src="limitTrial"  height="200" width="300" id="limitTrial" style="display:none"></iframe>


  <div class="form">
    <div class="title">LISENSI</div>
      <form action="/get">
        <input type="checkbox" id="sahurSettingCBId" name="sahurSettingCB" value="1" style="visibility:hidden;">
        
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 20px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Serial Number</label></div>
          <div style="color:aqua; font-family: sans-serif; text-align:right;"><label id="serialNumberId"></label></div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 20px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Status Lisensi</label></div>
          <div style="color:aqua; font-family: sans-serif; text-align:right;"><label id="lisensiStatId"></label></div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 20px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Batas Waktu Uji Coba</label></div>
          <div style="color:aqua; font-family: sans-serif; text-align:right;"><label id="limitTrialId"></label></div>
        </div>

        <div class="input-container ic1">
          <input type="text" id="lisensiId" class="input" name="lisensi" pattern="[0-9]{8}" required>
          <div class="cut"></div>
          <label class="placeholder">Masukkan Lisensi</label>
        </div>
        
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  var iframe = document.getElementById('serialNumber');
  var content = iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  var iframe2 = document.getElementById('lisensiSave');
  var content2 = iframe2.contentWindow.document.body.innerText;
  var contentArr2 = content2.split("\n");
  var iframe3 = document.getElementById('lisensiStat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");
  var iframe4 = document.getElementById('limitTrial');
  var content4 = iframe4.contentWindow.document.body.innerText;
  var contentArr4 = content4.split("\n");
  document.getElementById("serialNumberId").innerHTML = contentArr[0];
  document.getElementById("lisensiId").value = contentArr2[0];
  document.getElementById("lisensiStatId").innerHTML = contentArr3[0];
  if(contentArr3[0]=="berbayar"){
    document.getElementById("limitTrialId").innerHTML = "-";
  }
  else{
    document.getElementById("limitTrialId").innerHTML = contentArr4[0] + " menit";
  }
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
