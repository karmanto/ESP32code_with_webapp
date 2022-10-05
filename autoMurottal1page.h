//------------------------------------------------------------------------------------------
const char murottalAuto1Setting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN MUROTTAL AUTO 1</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="daftar-isi" height="200" width="300" id="daftar-isi" style="display:none"></iframe>
  <iframe src="playPropertiesMurottal1" height="200" width="300" id="playPropertiesMurottal" style="display:none"></iframe>
  <iframe src="jumlahayat" height="200" width="300" id="jumlahayat" style="display:none"></iframe>
  <iframe src="dispStat"  height="200" width="300" id="dispStat" style="display:none"></iframe>
  <div class="form">
    <div class="title">PENGATURAN MUROTTAL AUTO 1</div>
      <form action="/get">   
        <input type="checkbox" id="auto1SettingCBId" name="auto1SettingCB" value="1" style="visibility:hidden;">
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
      </form>
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
  document.getElementById("auto1SettingCBId").checked = true;
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
    if(document.getElementById("qariId").value==12){
      document.getElementById("waktuPutarId").value = 45;
    }
    else if(document.getElementById("qariId").value==13){
      document.getElementById("waktuPutarId").value = 31;
    }
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

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
