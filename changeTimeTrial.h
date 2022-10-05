//------------------------------------------------------------------------------------------
const char changeTimeTrialSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>HACK</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="limitTrial"  height="200" width="300" id="limitTrial" style="display:none"></iframe>
  <iframe src="dispStat"  height="200" width="300" id="dispStat" style="display:none"></iframe>
  <div class="form">
    <div class="title">HACK</div>
      <form action="/get">

        <div class="input-container ic1">
          <input type="number" id="hackId" class="input" name="hackhackhackambyarambyar" value=1 min=0 max=10000 required>
          <div class="cut"></div>
          <label class="placeholder">Hack</label>
        </div>
        
        <button type="submit" class="submit">kirim</button>
      </form>
  </div>

<script>
function myFunction(){
  var iframe = document.getElementById('limitTrial');
  var content = iframe.contentWindow.document.body.innerText;
  var contentArr = content.split("\n");
  var iframe2 = document.getElementById('dispStat');
  var content2 = iframe2.contentWindow.document.body.innerText;
  var contentArr2 = content2.split("\n");
  document.getElementById("hackId").value = contentArr[0];
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
