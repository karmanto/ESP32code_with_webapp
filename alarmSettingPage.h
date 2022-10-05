//------------------------------------------------------------------------------------------
const char alarmSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN ALARM</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
  <link rel="stylesheet" type="text/css" href="style3.css">
</head>
<body onload="myFunction()">
  <iframe src="timeAlarm" height="200" width="300" id="timeAlarm" style="display:none"></iframe>
  <iframe src="alarmWithMurottal" height="200" width="300" id="alarmWithMurottal" style="display:none"></iframe>
  <iframe src="dispStat"  height="200" width="300" id="dispStat" style="display:none"></iframe>

  <div class="form">
    <div class="title">PENGATURAN ALARM</div>
      <form action="/get">
        <input type="checkbox" id="alarmSettingCBId" name="alarmSettingCB" value="1" style="visibility:hidden;">
        
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 30px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Suara Adzan</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="adzanCBId" name="adzanCB" value="1"><span class="slider"></span></label>
          </div>
        </div>
        
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 30px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Hitung Mundur Waktu Sholat</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="countdownCBId" name="countdownCB" value="1"><span class="slider"></span></label>
          </div>
        </div>
        
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Sahur</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="sahurCBId" name="sahurCB" value="1" onChange="sahurCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeSahurId" class="input" name="timeSahur">
            <option value="30">30 menit menjelang imsak</option>
            <option value="45">45 menit menjelang imsak</option>
            <option value="60">60 menit menjelang imsak</option>
            <option value="90">90 menit menjelang imsak</option>
            <option value="120">120 menit menjelang imsak</option>
          </select>
        </div>   
           
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Subuh</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="subuhCBId" name="subuhCB" value="1" onChange="subuhCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeSubuhId" class="input" name="timeSubuh">
            <option value="5">5 menit menjelang subuh</option>
            <option value="10">10 menit menjelang subuh</option>
            <option value="15">15 menit menjelang subuh</option>
            <option value="20">20 menit menjelang subuh</option>
            <option value="30">30 menit menjelang subuh</option>
          </select>
        </div>
        <div class="input-container ic3">
          <select id="murottalSubuhId" class="input" name="murottalSubuh">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Zuhur</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="zuhurCBId" name="zuhurCB" value="1" onChange="zuhurCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeZuhurId" class="input" name="timeZuhur">
            <option value="5">5 menit menjelang zuhur</option>
            <option value="10">10 menit menjelang zuhur</option>
            <option value="15">15 menit menjelang zuhur</option>
            <option value="20">20 menit menjelang zuhur</option>
            <option value="30">30 menit menjelang zuhur</option>
          </select>
        </div>
        <div class="input-container ic3">
          <select id="murottalZuhurId" class="input" name="murottalZuhur">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Asar</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="asarCBId" name="asarCB" value="1" onChange="asarCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeAsarId" class="input" name="timeAsar">
            <option value="5">5 menit menjelang asar</option>
            <option value="10">10 menit menjelang asar</option>
            <option value="15">15 menit menjelang asar</option>
            <option value="20">20 menit menjelang asar</option>
            <option value="30">30 menit menjelang asar</option>
          </select>
        </div>
        <div class="input-container ic3">
          <select id="murottalAsarId" class="input" name="murottalAsar">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Magrib</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="magribCBId" name="magribCB" value="1" onChange="magribCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeMagribId" class="input" name="timeMagrib">
            <option value="5">5 menit menjelang magrib</option>
            <option value="10">10 menit menjelang magrib</option>
            <option value="15">15 menit menjelang magrib</option>
            <option value="20">20 menit menjelang magrib</option>
            <option value="30">30 menit menjelang magrib</option>
          </select>
        </div>
        <div class="input-container ic3">
          <select id="murottalMagribId" class="input" name="murottalMagrib">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Isya</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="isyaCBId" name="isyaCB" value="1" onChange="isyaCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <select id="timeIsyaId" class="input" name="timeIsya">
            <option value="5">5 menit menjelang isya</option>
            <option value="10">10 menit menjelang isya</option>
            <option value="15">15 menit menjelang isya</option>
            <option value="20">20 menit menjelang isya</option>
            <option value="30">30 menit menjelang isya</option>
          </select>
        </div>
        <div class="input-container ic3">
          <select id="murottalIsyaId" class="input" name="murottalIsya">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm Tahajjud</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="tahajjudCBId" name="tahajjudCB" value="1" onChange="tahajjudCBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeTahajjudId" class="input" name="timeTahajjud" min="00:00" max="04:00" required>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm 1</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="custom1CBId" name="custom1CB" value="1" onChange="custom1CBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeCustom1Id" class="input" name="timeCustom1" required>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm 2</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="custom2CBId" name="custom2CB" value="1" onChange="custom2CBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeCustom2Id" class="input" name="timeCustom2" required>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm 3</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="custom3CBId" name="custom3CB" value="1" onChange="custom3CBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeCustom3Id" class="input" name="timeCustom3" required>
        </div>
        <div class="input-container ic3">
          <select id="murottalCustom3Id" class="input" name="murottalCustom3">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm 4</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="custom4CBId" name="custom4CB" value="1" onChange="custom4CBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeCustom4Id" class="input" name="timeCustom4" required>
        </div>
        <div class="input-container ic3">
          <select id="murottalCustom4Id" class="input" name="murottalCustom4">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Alarm 5</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="custom5CBId" name="custom5CB" value="1" onChange="custom5CBonChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="time" id="timeCustom5Id" class="input" name="timeCustom5" required>
        </div>
        <div class="input-container ic3">
          <select id="murottalCustom5Id" class="input" name="murottalCustom5">
            <option value="0">murottal nonaktif</option>
            <option value="1">putar auto murottal 1</option>
            <option value="2">putar auto murottal 2</option>
            <option value="3">putar auto murottal 3</option>
          </select>
        </div>
        
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  document.getElementById("alarmSettingCBId").checked = true;
  var iframe = document.getElementById('timeAlarm');
  var content= iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  var iframe2 = document.getElementById('alarmWithMurottal');
  var content2= iframe2.contentWindow.document.body.innerText;
  var contentArr2 = content2.split("\n");
  var iframe4 = document.getElementById('dispStat');
  var content4 = iframe4.contentWindow.document.body.innerText;
  var contentArr4 = content4.split("\n");
  if(contentArr[0] == "999"){
    document.getElementById("sahurCBId").checked = false;
    document.getElementById("timeSahurId").disabled = true;    
  }
  else{
    document.getElementById("sahurCBId").checked = true;
    document.getElementById("timeSahurId").value = contentArr[0];
  }
  if(contentArr[1] == "999"){
    document.getElementById("subuhCBId").checked = false;
    document.getElementById("timeSubuhId").disabled = true;
    document.getElementById("murottalSubuhId").disabled = true;
  }
  else{
    document.getElementById("subuhCBId").checked = true;
    document.getElementById("timeSubuhId").value = contentArr[1];
    document.getElementById("murottalSubuhId").value = contentArr2[0];
  }
  if(contentArr[2] == "999"){
    document.getElementById("zuhurCBId").checked = false;
    document.getElementById("timeZuhurId").disabled = true;
    document.getElementById("murottalZuhurId").disabled = true;
  }
  else{
    document.getElementById("zuhurCBId").checked = true;
    document.getElementById("timeZuhurId").value = contentArr[2];
    document.getElementById("murottalZuhurId").value = contentArr2[1];
  }
  if(contentArr[3] == "999"){
    document.getElementById("asarCBId").checked = false;
    document.getElementById("timeAsarId").disabled = true;
    document.getElementById("murottalAsarId").disabled = true;
  }
  else{
    document.getElementById("asarCBId").checked = true;
    document.getElementById("timeAsarId").value = contentArr[3];
    document.getElementById("murottalAsarId").value = contentArr2[2];
  }
  if(contentArr[4] == "999"){
    document.getElementById("magribCBId").checked = false;
    document.getElementById("timeMagribId").disabled = true;
    document.getElementById("murottalMagribId").disabled = true;
  }
  else{
    document.getElementById("magribCBId").checked = true;
    document.getElementById("timeMagribId").value = contentArr[4];
    document.getElementById("murottalMagribId").value = contentArr2[3];
  }
  if(contentArr[5] == "999"){
    document.getElementById("isyaCBId").checked = false;
    document.getElementById("timeIsyaId").disabled = true;
    document.getElementById("murottalIsyaId").disabled = true;
  }
  else{
    document.getElementById("isyaCBId").checked = true;
    document.getElementById("timeIsyaId").value = contentArr[5];
    document.getElementById("murottalIsyaId").value = contentArr2[4];
  }
  if(contentArr[6] == "99:99"){
    document.getElementById("tahajjudCBId").checked = false;
    document.getElementById("timeTahajjudId").disabled = true;
  }
  else{
    document.getElementById("tahajjudCBId").checked = true;
    document.getElementById("timeTahajjudId").value = contentArr[6];
  }
  if(contentArr[7] == "99:99"){
    document.getElementById("custom1CBId").checked = false;
    document.getElementById("timeCustom1Id").disabled = true;
  }
  else{
    document.getElementById("custom1CBId").checked = true;
    document.getElementById("timeCustom1Id").value = contentArr[7];
  }
  if(contentArr[8] == "99:99"){
    document.getElementById("custom2CBId").checked = false;
    document.getElementById("timeCustom2Id").disabled = true;
  }
  else{
    document.getElementById("custom2CBId").checked = true;
    document.getElementById("timeCustom2Id").value = contentArr[8];
  }
  if(contentArr[9] == "99:99"){
    document.getElementById("custom3CBId").checked = false;
    document.getElementById("timeCustom3Id").disabled = true;
    document.getElementById("murottalCustom3Id").disabled = true;
  }
  else{
    document.getElementById("custom3CBId").checked = true;
    document.getElementById("timeCustom3Id").value = contentArr[9];
    document.getElementById("murottalCustom3Id").value = contentArr2[7];
  }
  if(contentArr[10] == "99:99"){
    document.getElementById("custom4CBId").checked = false;
    document.getElementById("timeCustom4Id").disabled = true;
    document.getElementById("murottalCustom4Id").disabled = true;
  }
  else{
    document.getElementById("custom4CBId").checked = true;
    document.getElementById("timeCustom4Id").value = contentArr[10];
    document.getElementById("murottalCustom4Id").value = contentArr2[8];
  }
  if(contentArr[11] == "99:99"){
    document.getElementById("custom5CBId").checked = false;
    document.getElementById("timeCustom5Id").disabled = true;
    document.getElementById("murottalCustom5Id").disabled = true;
  }
  else{
    document.getElementById("custom5CBId").checked = true;
    document.getElementById("timeCustom5Id").value = contentArr[11];
    document.getElementById("murottalCustom5Id").value = contentArr2[9];
  }
  if(contentArr[12] == "0"){
    document.getElementById("adzanCBId").checked = false;
  }
  else{
    document.getElementById("adzanCBId").checked = true;
  }
  if(contentArr[13] == "0"){
    document.getElementById("countdownCBId").checked = false;
  }
  else{
    document.getElementById("countdownCBId").checked = true;
  }

  
  if(contentArr4[0]=="0"){
    document.getElementById("adzanCBId").disabled = true;
    document.getElementById("adzanCBId").checked = false;
    document.getElementById("murottalSubuhId").disabled = true;
    document.getElementById("murottalZuhurId").disabled = true;
    document.getElementById("murottalAsarId").disabled = true;
    document.getElementById("murottalMagribId").disabled = true;
    document.getElementById("murottalIsyaId").disabled = true;
    document.getElementById("custom3CBId").checked = false;
    document.getElementById("custom4CBId").checked = false;
    document.getElementById("custom5CBId").checked = false;
    document.getElementById("custom3CBId").disabled = true;
    document.getElementById("custom4CBId").disabled = true;
    document.getElementById("custom5CBId").disabled = true;
    document.getElementById("timeCustom3Id").disabled = true;
    document.getElementById("murottalCustom3Id").disabled = true;
    document.getElementById("timeCustom4Id").disabled = true;
    document.getElementById("murottalCustom4Id").disabled = true;
    document.getElementById("timeCustom5Id").disabled = true;
    document.getElementById("murottalCustom5Id").disabled = true;
  }
}

function sahurCBonChange(){
  if(document.getElementById("sahurCBId").checked == false){
    document.getElementById("timeSahurId").disabled = true; 
  }
  else{
    document.getElementById("timeSahurId").disabled = false; 
  }
}

function subuhCBonChange(){
  if(document.getElementById("subuhCBId").checked == false){
    document.getElementById("timeSubuhId").disabled = true; 
    document.getElementById("murottalSubuhId").disabled = true;
  }
  else{
    document.getElementById("timeSubuhId").disabled = false; 
    document.getElementById("murottalSubuhId").disabled = false;
  }
}

function zuhurCBonChange(){
  if(document.getElementById("zuhurCBId").checked == false){
    document.getElementById("timeZuhurId").disabled = true; 
    document.getElementById("murottalZuhurId").disabled = true;
  }
  else{
    document.getElementById("timeZuhurId").disabled = false;
    document.getElementById("murottalZuhurId").disabled = false; 
  }
}

function asarCBonChange(){
  if(document.getElementById("asarCBId").checked == false){
    document.getElementById("timeAsarId").disabled = true; 
    document.getElementById("murottalAsarId").disabled = true;
  }
  else{
    document.getElementById("timeAsarId").disabled = false;
    document.getElementById("murottalAsarId").disabled = false; 
  }
}

function magribCBonChange(){
  if(document.getElementById("magribCBId").checked == false){
    document.getElementById("timeMagribId").disabled = true;
    document.getElementById("murottalMagribId").disabled = true; 
  }
  else{
    document.getElementById("timeMagribId").disabled = false;
    document.getElementById("murottalMagribId").disabled = false;  
  }
}

function isyaCBonChange(){
  if(document.getElementById("isyaCBId").checked == false){
    document.getElementById("timeIsyaId").disabled = true; 
    document.getElementById("murottalIsyaId").disabled = true; 
  }
  else{
    document.getElementById("timeIsyaId").disabled = false;
    document.getElementById("murottalIsyaId").disabled = false;  
  }
}

function tahajjudCBonChange(){
  if(document.getElementById("tahajjudCBId").checked == false){
    document.getElementById("timeTahajjudId").disabled = true; 
  }
  else{
    document.getElementById("timeTahajjudId").disabled = false; 
  }
}

function custom1CBonChange(){
  if(document.getElementById("custom1CBId").checked == false){
    document.getElementById("timeCustom1Id").disabled = true; 
  }
  else{
    document.getElementById("timeCustom1Id").disabled = false; 
  }
}

function custom2CBonChange(){
  if(document.getElementById("custom2CBId").checked == false){
    document.getElementById("timeCustom2Id").disabled = true; 
  }
  else{
    document.getElementById("timeCustom2Id").disabled = false; 
  }
}

function custom3CBonChange(){
  if(document.getElementById("custom3CBId").checked == false){
    document.getElementById("timeCustom3Id").disabled = true; 
    document.getElementById("murottalCustom3Id").disabled = true; 
  }
  else{
    document.getElementById("timeCustom3Id").disabled = false; 
    document.getElementById("murottalCustom3Id").disabled = false; 
  }
}

function custom4CBonChange(){
  if(document.getElementById("custom4CBId").checked == false){
    document.getElementById("timeCustom4Id").disabled = true; 
    document.getElementById("murottalCustom4Id").disabled = true; 
  }
  else{
    document.getElementById("timeCustom4Id").disabled = false; 
    document.getElementById("murottalCustom4Id").disabled = false; 
  }
}

function custom5CBonChange(){
  if(document.getElementById("custom5CBId").checked == false){
    document.getElementById("timeCustom5Id").disabled = true; 
    document.getElementById("murottalCustom5Id").disabled = true; 
  }
  else{
    document.getElementById("timeCustom5Id").disabled = false; 
    document.getElementById("murottalCustom5Id").disabled = false; 
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
