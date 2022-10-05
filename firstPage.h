//------------------------------------------------------------------------------------------
const char murottalSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN MUROTTAL MANUAL</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
  <link rel="stylesheet" type="text/css" href="style2.css">
</head>
<body onload="myFunction()">
  <iframe src="daftar-isi" height="200" width="300" id="daftar-isi" style="display:none"></iframe>
  <iframe src="playPropertiesMurottal" height="200" width="300" id="playPropertiesMurottal" style="display:none"></iframe>
  <iframe src="jumlahayat" height="200" width="300" id="jumlahayat" style="display:none"></iframe>
  <iframe src="dispStat"  height="200" width="300" id="dispStat" style="display:none"></iframe>
  
  <div class="form">
    <div class="title">PENGATURAN MUROTTAL MANUAL</div>
      <form action="/get">   
        <input type="checkbox" id="murottalSettingCBId" name="murottalSettingCB" value="1" style="visibility:hidden;">    
        <div class="input-container ic1">
          <select id="qariId" class="input" name="qari" onChange="qariOnChange()">
            <option value="0">Qari - Alafasy (per ayat)</option>
            <option value="1">Qari - Akram Al Alaqimy (per ayat)</option>
            <option value="2">Qari - Asy Satree</option>
            <option value="3">Qari - Abdurrahman Sudais</option>
            <option value="4">Qari - Nasser Alqatami</option>
            <option value="5">Qari - Ahmad Misbahi (Qari Anak)</option>
            <option value="6">Qari - Ali Hudaifi</option>
            <option value="7">Qari - Kalifa Tunaiji</option>
            <option value="8">Qari - Al Mansary</option>
            <option value="9">Qari - Saad Al Ghamidi</option>
            <option value="10">Hadits Arbain</option>
            <option value="11">Asmaul Husna</option>
            <option value="12">Dzikir Pagi</option>
            <option value="13">Dzikir Petang</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Qari</label>
        </div>
        <div class="input-container ic2">
          <select id="firstSurahId" class="input" name="firstSurah" onChange="firstSurahOnChange()">
          </select>
          <div class="cut"></div>
          <label class="placeholder">Permulaan Surah</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="firstAyatId" class="input" name="firstAyat" onChange="firstAyatOnChange()" value=1 required>
          <div class="cut"></div>
          <label class="placeholder">Permulaan Ayat</label>
        </div>
        <div class="input-container ic2">
          <select id="lastSurahId" name="lastSurah" class="input" onChange="lastSurahOnChange()">
          </select>
          <div class="cut"></div>
          <label class="placeholder">Akhir Surah</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="lastAyatId" class="input" name="lastAyat" value=1 required>
          <div class="cut"></div>
          <label class="placeholder">Akhir Ayat</label>
        </div>
        <div class="input-container ic2">
          <select id="metodeId" name="metode" class="input" onChange="metodeOnChange()">
            <option value="0">Metode Biasa</option>
            <option value="1">Metode Tikrar</option>
          </select>
          <div class="cut"></div>
          <label class="placeholder">Metode Pengulangan</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="repeatNumId" class="input" name="repeatNum" value=1 min=1 max=20 required>
          <div class="cut"></div>
          <label class="placeholder">Jumlah Pengulangan (Tikrar)</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="waktuPutarId" class="input" name="waktuPutar" value=1 min=1 max=1000 required>
          <div class="cut"></div>
          <label class="placeholder">Waktu Putar (menit)</label>
        </div>
        <button type="submit" class="submit" id="sendButton">kirim</button>
      </form><br>
      <div style="text-align:center;">
      <p style="color:aqua; font-family: sans-serif;">Status Murottal <span id="state">%STATE%</span></p>
      %BUTTONPLACEHOLDER%
      </div>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
const input = document.querySelector('input');
input.addEventListener('input', updateValue);

function updateValue(e) {
  var iframe3 = document.getElementById('jumlahayat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  document.getElementById("lastAyatId").max = contentArr3[parseInt(y.value)-1];
  if(x.value==y.value || document.getElementById("qariId").value==10){
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
  }
  else{
    document.getElementById("lastAyatId").min = 1;
  }
}

function myFunction(){
  document.getElementById("murottalSettingCBId").checked = true;
  var iframe = document.getElementById('daftar-isi');
  var content = iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  var iframe2 = document.getElementById('playPropertiesMurottal');
  var content2 = iframe2.contentWindow.document.body.innerText;
  var contentArr2 = content2.split("\n");
  var iframe3 = document.getElementById('jumlahayat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");  
  var iframe4 = document.getElementById('dispStat');
  var content4 = iframe4.contentWindow.document.body.innerText;
  var contentArr4 = content4.split("\n");
  if(contentArr4[0]=="0"){
    document.getElementById("sendButton").disabled = true;
    document.getElementById("output").disabled = true;
  }
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  for (i = 0; i < contentArr.length; i++) {
    var option = document.createElement("option");
    option.text = "(" + (i+1) + ") " + contentArr[i] + " " + contentArr3[i] + " ayat";
    option.value = i+1;
    x.add(option);
    if(i+1==parseInt(contentArr2[0])){
      x.options.selectedIndex = i;
    }
  }
  for (i = 0; i < contentArr.length; i++) {
    var option2 = document.createElement("option");
    option2.text = "(" + (i+1) + ") " + contentArr[i] + " " + contentArr3[i] + " ayat";
    option2.value = i+1;
    y.add(option2);
    if(i+1==parseInt(contentArr2[2])){
      y.options.selectedIndex = i;
    }
  }
  var a = parseInt(x.value);
  for (i = 0; i < a-1; i++) {
    document.getElementById("lastSurahId").options[i].disabled = true;
  }
  document.getElementById("firstAyatId").value = parseInt(contentArr2[1]);
  document.getElementById("lastAyatId").value = parseInt(contentArr2[3]);
  document.getElementById("metodeId").value = contentArr2[4];
  document.getElementById("repeatNumId").value = parseInt(contentArr2[5]);
  document.getElementById("qariId").value = contentArr2[6];
  document.getElementById("waktuPutarId").value = parseInt(contentArr2[7]);
  document.getElementById("firstAyatId").max = contentArr3[parseInt(x.value)-1];
  document.getElementById("firstAyatId").min = 1;
  document.getElementById("lastAyatId").max = contentArr3[parseInt(y.value)-1];
  if(x.value==y.value){
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
  }
  else{
    document.getElementById("lastAyatId").min = 1;
  }

  if(document.getElementById("qariId").value>1 && document.getElementById("qariId").value<10){
    document.getElementById("firstAyatId").value = 1;
    document.getElementById("lastAyatId").value = contentArr3[parseInt(y.value)-1];
    document.getElementById("firstAyatId").disabled = true;
    document.getElementById("lastAyatId").disabled = true;
    document.getElementById("firstSurahId").disabled = false;
    document.getElementById("lastSurahId").disabled = false;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else if(document.getElementById("qariId").value==10){
    document.getElementById("firstAyatId").disabled = false;
    document.getElementById("lastAyatId").disabled = false;
    document.getElementById("firstSurahId").disabled = true;
    document.getElementById("lastSurahId").disabled = true;
    document.getElementById("firstAyatId").max = 42;
    document.getElementById("firstAyatId").min = 1;
    document.getElementById("lastAyatId").max = 42;
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else if(document.getElementById("qariId").value>10){
    document.getElementById("firstAyatId").disabled = true;
    document.getElementById("lastAyatId").disabled = true;
    document.getElementById("firstSurahId").disabled = true;
    document.getElementById("lastSurahId").disabled = true;
    document.getElementById("firstAyatId").max = 0;
    document.getElementById("firstAyatId").min = 0;
    document.getElementById("lastAyatId").max = 0;
    document.getElementById("lastAyatId").min = 0;
    document.getElementById("firstAyatId").value = 0;
    document.getElementById("lastAyatId").value = 0;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else{
    document.getElementById("firstAyatId").disabled = false;
    document.getElementById("lastAyatId").disabled = false;
    document.getElementById("firstSurahId").disabled = false;
    document.getElementById("lastSurahId").disabled = false;
    document.getElementById("metodeId").disabled = false;
    document.getElementById("repeatNumId").disabled = false;
  }
  if(document.getElementById("metodeId").value=="0"){
    document.getElementById("repeatNumId").disabled = true;
  }
}

function firstSurahOnChange(){
  var z = parseInt(document.getElementById("lastSurahId").value);
  document.querySelector('#lastSurahId').innerHTML = '';
  var iframe = document.getElementById('daftar-isi');
  var content = iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  var iframe3 = document.getElementById('jumlahayat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  for (i = 0; i < contentArr.length; i++) {
    var option2 = document.createElement("option");
    option2.text = "(" + (i+1) + ") " + contentArr[i] + " " + contentArr3[i] + " ayat";
    option2.value = i+1;
    y.add(option2);
  }
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  var a = parseInt(x.value);
  for (i = 0; i < a-1; i++) {
    document.getElementById("lastSurahId").options[i].disabled = true;
  }
  if(z<a){
    y.options.selectedIndex = a-1;
  }
  else{
    y.options.selectedIndex = z-1;
  }
  document.getElementById("firstAyatId").max = contentArr3[parseInt(x.value)-1];
  document.getElementById("firstAyatId").min = 1;
  document.getElementById("lastAyatId").max = contentArr3[parseInt(y.value)-1];
  if(x.value==y.value){
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
  }
  else{
    document.getElementById("lastAyatId").min = 1;
  }

  if(document.getElementById("qariId").value>1 && document.getElementById("qariId").value<10){
    document.getElementById("firstAyatId").value = 1;
    document.getElementById("lastAyatId").value = contentArr3[parseInt(y.value)-1];
  }
}

function firstAyatOnChange(){
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  if(x.value==y.value){
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
  }
  else{
    document.getElementById("lastAyatId").min = 1;
  }
}

function lastSurahOnChange(){
  var iframe3 = document.getElementById('jumlahayat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");
  document.getElementById("lastAyatId").max = contentArr3[parseInt(y.value)-1];
  if(x.value==y.value){
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
  }
  else{
    document.getElementById("lastAyatId").min = 1;
  }

  if(document.getElementById("qariId").value>1 && document.getElementById("qariId").value<10){
    document.getElementById("firstAyatId").value = 1;
    document.getElementById("lastAyatId").value = contentArr3[parseInt(y.value)-1];
  }
}

function qariOnChange(){
  var iframe3 = document.getElementById('jumlahayat');
  var content3 = iframe3.contentWindow.document.body.innerText;
  var contentArr3 = content3.split("\n");
  var x = document.getElementById("firstSurahId");
  var y = document.getElementById("lastSurahId");  
  if(document.getElementById("qariId").value>1 && document.getElementById("qariId").value<10){
    document.getElementById("firstAyatId").value = 1;
    document.getElementById("lastAyatId").value = contentArr3[parseInt(y.value)-1];
    document.getElementById("firstAyatId").disabled = true;
    document.getElementById("lastAyatId").disabled = true;
    document.getElementById("firstSurahId").disabled = false;
    document.getElementById("lastSurahId").disabled = false;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else if(document.getElementById("qariId").value==10){
    document.getElementById("firstAyatId").disabled = false;
    document.getElementById("lastAyatId").disabled = false;
    document.getElementById("firstSurahId").disabled = true;
    document.getElementById("lastSurahId").disabled = true;
    document.getElementById("firstAyatId").max = 42;
    document.getElementById("firstAyatId").min = 1;
    document.getElementById("lastAyatId").max = 42;
    document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else if(document.getElementById("qariId").value>10){
    document.getElementById("firstAyatId").disabled = true;
    document.getElementById("lastAyatId").disabled = true;
    document.getElementById("firstSurahId").disabled = true;
    document.getElementById("lastSurahId").disabled = true;
    document.getElementById("firstAyatId").max = 0;
    document.getElementById("firstAyatId").min = 0;
    document.getElementById("lastAyatId").max = 0;
    document.getElementById("lastAyatId").min = 0;
    document.getElementById("firstAyatId").value = 0;
    document.getElementById("lastAyatId").value = 0;
    document.getElementById("metodeId").disabled = true;
    document.getElementById("repeatNumId").disabled = true;
  }
  else{
    document.getElementById("firstAyatId").disabled = false;
    document.getElementById("lastAyatId").disabled = false;
    document.getElementById("firstSurahId").disabled = false;
    document.getElementById("lastSurahId").disabled = false;
    document.getElementById("metodeId").disabled = false;
    document.getElementById("repeatNumId").disabled = false;
    document.getElementById("firstAyatId").max = contentArr3[parseInt(x.value)-1];
    document.getElementById("firstAyatId").min = 1;
    document.getElementById("lastAyatId").max = contentArr3[parseInt(y.value)-1];
    if(x.value==y.value){
      document.getElementById("lastAyatId").min = document.getElementById("firstAyatId").value;
    }
    if(document.getElementById("metodeId").value == "0"){
      document.getElementById("repeatNumId").disabled = true;
    }
    else{
      document.getElementById("repeatNumId").disabled = false;
    }
  }
}

function metodeOnChange(){
  if(document.getElementById("metodeId").value == "0"){
    document.getElementById("repeatNumId").disabled = true;
  }
  else{
    document.getElementById("repeatNumId").disabled = false;
  }
}

function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ 
    xhr.open("GET", "/update1?state=0", true); 
    document.getElementById("state").innerHTML = "ON";  
  }
  else { 
    xhr.open("GET", "/update1?state=1", true); 
    document.getElementById("state").innerHTML = "OFF";      
  }
  xhr.send();
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------

const char logout[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">Kembali ke Halaman Utama</a>.</p>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
const char homePage[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>HALAMAN UTAMA</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">

  <div class="form">
    <div class="title">HALAMAN UTAMA</div>
    <button class="submit" onclick="murottalSettingButton()">Pengaturan Murottal Manual</button>
    <button class="submit" onclick="auto1SettingButton()">Pengaturan Murottal Auto 1</button>
    <button class="submit" onclick="auto2SettingButton()">Pengaturan Murottal Auto 2</button>
    <button class="submit" onclick="auto3SettingButton()">Pengaturan Murottal Auto 3</button>
    <button class="submit" onclick="generalSettingButton()">Pengaturan Umum</button>
    <button class="submit" onclick="timeSettingButton()">Pengaturan Jam dan Tanggal</button>
    <button class="submit" onclick="alarmSettingButton()">Pengaturan Alarm</button>
    <button class="submit" onclick="iqamahSettingButton()">Pengaturan Waktu Iqamah</button>
    <button class="submit" onclick="sahurSettingButton()">Pengaturan Sahur</button>
    <button class="submit" onclick="wifiSettingButton()">Pengaturan Wifi</button>
    <button class="submit" onclick="lisensiButton()">Lisensi</button>
    <button class="submit" onclick="updateButton()">Update Program</button>
  </div>

<script>
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}

function generalSettingButton() {
  window.open("/generalSetting","_self");
}

function murottalSettingButton() {
  window.open("/murottalSetting","_self");
}

function timeSettingButton() {
  window.open("/timeSetting","_self");
}

function alarmSettingButton() {
  window.open("/alarmSetting","_self");
}

function iqamahSettingButton() {
  window.open("/iqamahSetting","_self");
}

function sahurSettingButton() {
  window.open("/sahurSetting","_self");
}

function auto1SettingButton() {
  window.open("/auto1Setting","_self");
}

function auto2SettingButton() {
  window.open("/auto2Setting","_self");
}

function auto3SettingButton() {
  window.open("/auto3Setting","_self");
}

function wifiSettingButton() {
  window.open("/wifiSetting","_self");
}

function lisensiButton() {
  window.open("/lisensiSetting","_self");
}

function updateButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update3?state=128", true);
  xhr.send();
  setTimeout(function(){ window.open("/update","_self"); }, 500);
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
