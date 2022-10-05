//------------------------------------------------------------------------------------------
const char sahurSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN SAHUR</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
  <link rel="stylesheet" type="text/css" href="style3.css">
</head>
<body onload="myFunction()">
  <iframe src="sahurStat"  height="200" width="300" id="sahurStat" style="display:none"></iframe>
  <div class="form">
    <div class="title">PENGATURAN SAHUR</div>
      <form action="/get">
        <input type="checkbox" id="sahurSettingCBId" name="sahurSettingCB" value="1" style="visibility:hidden;">
        
        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Ramadan</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="ramadanCBId" name="ramadanCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Syawal</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="syawalCBId" name="syawalCB" value="1" onclick="syawalOnChange()"><span class="slider"></span></label>
          </div>
        </div>
        <div class="input-container ic3">
          <input type="number" id="awalSyawalId" class="input" name="awalSyawal" value=2 min=2 max=24 required>
          <div class="cut"></div>
          <label class="placeholder">Tanggal Awal Puasa Syawal</label>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Zulhijah</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="zulhijahCBId" name="zulhijahCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Arafah</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="arafahCBId" name="arafahCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Tasua</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="tasuaCBId" name="tasuaCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Asyuro</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="asyuroCBId" name="asyuroCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Ayamulbidh</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="ayamulbidhCBId" name="ayamulbidhCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Senin</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="seninCBId" name="seninCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Kamis</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="kamisCBId" name="kamisCB" value="1"><span class="slider"></span></label>
          </div>
        </div>

        <div style="display:grid; grid-template-columns: 50% 50%; margin-top: 40px;">
          <div style="color:aqua; font-family: sans-serif; text-align:left;"><label>Sahur Daud</label></div>
          <div style="text-align:right;">
          <label class="switch"><input type="checkbox" id="daudCBId" name="daudCB" value="1" onclick="daudOnChange()"><span class="slider"></span></label>
          </div>
        </div>
        
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
function myFunction(){
  document.getElementById("sahurSettingCBId").checked = true;
  var iframe = document.getElementById('sahurStat');
  var content= iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  document.getElementById("syawalCBId").checked = true;
  if(contentArr[0] == "0"){
    document.getElementById("syawalCBId").checked = false;    
  }
  document.getElementById("zulhijahCBId").checked = true;
  if(contentArr[1] == "0"){
    document.getElementById("zulhijahCBId").checked = false;    
  }
  document.getElementById("ramadanCBId").checked = true;
  if(contentArr[2] == "0"){
    document.getElementById("ramadanCBId").checked = false;    
  }
  document.getElementById("asyuroCBId").checked = true;
  if(contentArr[3] == "0"){
    document.getElementById("asyuroCBId").checked = false;    
  }
  document.getElementById("ayamulbidhCBId").checked = true;
  if(contentArr[4] == "0"){
    document.getElementById("ayamulbidhCBId").checked = false;    
  }
  document.getElementById("seninCBId").checked = true;
  if(contentArr[5] == "0"){
    document.getElementById("seninCBId").checked = false;    
  }
  document.getElementById("kamisCBId").checked = true;
  if(contentArr[6] == "0"){
    document.getElementById("kamisCBId").checked = false;    
  }
  document.getElementById("daudCBId").checked = true;
  if(contentArr[7] == "0"){
    document.getElementById("daudCBId").checked = false;    
  }
  document.getElementById("tasuaCBId").checked = true;
  if(contentArr[8] == "0"){
    document.getElementById("tasuaCBId").checked = false;    
  }
  document.getElementById("arafahCBId").checked = true;
  if(contentArr[9] == "0"){
    document.getElementById("arafahCBId").checked = false;    
  }
  document.getElementById("awalSyawalId").value = parseInt(contentArr[10]);

  if(document.getElementById("daudCBId").checked == true){
    document.getElementById("zulhijahCBId").disabled = true; 
    document.getElementById("asyuroCBId").disabled = true; 
    document.getElementById("ayamulbidhCBId").disabled = true; 
    document.getElementById("seninCBId").disabled = true; 
    document.getElementById("kamisCBId").disabled = true; 
    document.getElementById("tasuaCBId").disabled = true; 
    document.getElementById("arafahCBId").disabled = true; 
    document.getElementById("syawalCBId").disabled = true; 
    document.getElementById("awalSyawalId").disabled = true; 
  }

  if(document.getElementById("syawalCBId").checked == true){
    document.getElementById("awalSyawalId").disabled = false; 
  }
  else{
    document.getElementById("awalSyawalId").disabled = true; 
  }
}

function daudOnChange(){
  if(document.getElementById("daudCBId").checked == true){
    document.getElementById("zulhijahCBId").disabled = true; 
    document.getElementById("asyuroCBId").disabled = true; 
    document.getElementById("ayamulbidhCBId").disabled = true; 
    document.getElementById("seninCBId").disabled = true; 
    document.getElementById("kamisCBId").disabled = true; 
    document.getElementById("tasuaCBId").disabled = true; 
    document.getElementById("arafahCBId").disabled = true;
    document.getElementById("syawalCBId").disabled = true; 
    document.getElementById("awalSyawalId").disabled = true; 
  }
  else{
    document.getElementById("zulhijahCBId").disabled = false; 
    document.getElementById("asyuroCBId").disabled = false; 
    document.getElementById("ayamulbidhCBId").disabled = false; 
    document.getElementById("seninCBId").disabled = false; 
    document.getElementById("kamisCBId").disabled = false; 
    document.getElementById("tasuaCBId").disabled = false; 
    document.getElementById("arafahCBId").disabled = false; 
    document.getElementById("syawalCBId").disabled = false;
    document.getElementById("awalSyawalId").disabled = false; 
  }
}

function syawalOnChange(){
  if(document.getElementById("syawalCBId").checked == true){
    document.getElementById("awalSyawalId").disabled = false; 
  }
  else{
    document.getElementById("awalSyawalId").disabled = true; 
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
