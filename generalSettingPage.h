//------------------------------------------------------------------------------------------
const char generalSetting[] PROGMEM = R"=====(

<!DOCTYPE HTML><html>
<head>
  <title>PENGATURAN UMUM</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body onload="myFunction()">
  <iframe src="koordinat"  height="200" width="300" id="koordinat" style="display:none"></iframe>
  <iframe src="volume"  height="200" width="300" id="volume" style="display:none"></iframe>
  <iframe src="volume2"  height="200" width="300" id="volume2" style="display:none"></iframe>
  <iframe src="koreksi"  height="200" width="300" id="koreksi" style="display:none"></iframe>
  <iframe src="dispStat"  height="200" width="300" id="dispStat" style="display:none"></iframe>
  <iframe src="brightness"  height="200" width="300" id="brightness" style="display:none"></iframe>
  <div class="form">
    <div class="title">PENGATURAN UMUM</div>
      <form action="/get">   
        <div class="input-container ic1">
          <input type="range" id="volumeId" class="input" name="volume" min="1" max="30">
          <div class="cut"></div>
          <label class="placeholder">Volume Alarm : <span id="volLabelId"></span></label>
        </div>
        <div class="input-container ic2">
          <input type="range" id="volume2Id" class="input" name="volume2" min="1" max="30">
          <div class="cut"></div>
          <label class="placeholder">Volume Murottal : <span id="volLabel2Id"></span></label>
        </div>
        <div class="input-container ic2">
          <input type="range" id="brightnessId" class="input" name="brightness" min="1" max="25">
          <div class="cut"></div>
          <label class="placeholder">Brightness : <span id="brightnessLabelId"></span></label>
        </div>
        <div class="input-container ic2">
          <select id="koordinatId" class="input" name="koordinat"> 
          </select>
          <div class="cut"></div>
          <label class="placeholder">Lokasi Kota/Kabupaten</label>
        </div>
        <div class="input-container ic2">
          <select id="timezoneId" class="input" name="timezone">
          </select>
          <div class="cut"></div>
          <label class="placeholder">Timezone</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiHijriId" class="input" name="koreksiHijri" value=1 min=-1 max=1 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Hijriah</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiImsakId" class="input" name="koreksiImsak" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Imsak</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiSubuhId" class="input" name="koreksiSubuh" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Subuh</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiTerbitId" class="input" name="koreksiTerbit" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Syuruq</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiDhuhaId" class="input" name="koreksiDhuha" value=1 min=0 max=30 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Dhuha</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiZuhurId" class="input" name="koreksiZuhur" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Zuhur</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiAsarId" class="input" name="koreksiAsar" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Asar</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiMagribId" class="input" name="koreksiMagrib" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Magrib</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="koreksiIsyaId" class="input" name="koreksiIsya" value=1 min=-10 max=10 required>
          <div class="cut"></div>
          <label class="placeholder">Koreksi Isya</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="sudutSubuhId" class="input" name="sudutSubuh" value=1 min=-20 max=20 required>
          <div class="cut"></div>
          <label class="placeholder">Sudut Subuh</label>
        </div>
        <div class="input-container ic2">
          <input type="number" id="sudutIsyaId" class="input" name="sudutIsya" value=1 min=-20 max=20 required>
          <div class="cut"></div>
          <label class="placeholder">Sudut Isya</label>
        </div>
        <button type="submit" class="submit">kirim</button>
      </form>
      <button class="submit" onclick="homeButton()">Halaman Utama</button>
  </div>

<script>
var kabupaten = "KAB. ACEH BARAT\nKAB. ACEH BARAT DAYA\nKAB. ACEH BESAR\nKAB. ACEH JAYA\nKAB. ACEH SELATAN\nKAB. ACEH SINGKIL\nKAB. ACEH TAMIANG\nKAB. ACEH TENGAH\nKAB. ACEH TENGGARA\nKAB. ACEH TIMUR\nKAB. ACEH UTARA\nKAB. BENER MERIAH\nKAB. BIREUEN\nKAB. GAYO LUES\nKAB. NAGAN RAYA\nKAB. PIDIE\nKAB. PIDIE JAYA\nKAB. SIMEULUE\nKOTA BANDA ACEH\nKOTA LANGSA\nKOTA LHOKSEUMAWE\nKOTA SABANG\nKOTA SUBULUSSALAM\nKAB. ASAHAN\nKAB. BATUBARA\nKAB. DAIRI\nKAB. DELI SERDANG\nKAB. HUMBANG HASUNDUTAN\nKAB. KARO\nKAB. LABUHANBATU\nKAB. LABUHANBATU SELATAN\nKAB. LABUHANBATU UTARA\nKAB. LANGKAT\nKAB. MANDAILING NATAL\nKAB. NIAS\nKAB. NIAS BARAT\nKAB. NIAS SELATAN\nKAB. NIAS UTARA\nKAB. PADANG LAWAS\nKAB. PADANG LAWAS UTARA\nKAB. PAKPAK BHARAT\nKAB. SAMOSIR\nKAB. SERDANG BEDAGAI\nKAB. SIMALUNGUN\nKAB. TAPANULI SELATAN\nKAB. TAPANULI TENGAH\nKAB. TAPANULI UTARA\nKAB. TOBA SAMOSIR\nKOTA BINJAI\nKOTA GUNUNGSITOLI\nKOTA MEDAN\nKOTA PADANGSIDEMPUAN\nKOTA PEMATANGSIANTAR\nKOTA SIBOLGA\nKOTA TANJUNGBALAI\nKOTA TEBING TINGGI\nKAB. AGAM\nKAB. DHARMASRAYA\nKAB. KEPULAUAN MENTAWAI\nKAB. LIMA PULUH KOTA\nKAB. PADANG PARIAMAN\nKAB. PASAMAN\nKAB. PASAMAN BARAT\nKAB. PESISIR SELATAN\nKAB. SIJUNJUNG\nKAB. SOLOK\nKAB. SOLOK SELATAN\nKAB. TANAH DATAR\nKOTA BUKITTINGGI\nKOTA PADANG\nKOTA PADANGPANJANG\nKOTA PARIAMAN\nKOTA PAYAKUMBUH\nKOTA SAWAHLUNTO\nKOTA SOLOK\nKAB. BENGKALIS\nKAB. INDRAGIRI HILIR\nKAB. INDRAGIRI HULU\nKAB. KAMPAR\nKAB. KEPULAUAN MERANTI\nKAB. KUANTAN SINGINGI\nKAB. PELALAWAN\nKAB. ROKAN HILIR\nKAB. ROKAN HULU\nKAB. SIAK\nKOTA DUMAI\nKOTA PEKANBARU\nKAB. BINTAN\nKAB. KARIMUN\nKAB. KEPULAUAN ANAMBAS\nKAB. LINGGA\nKAB. NATUNA\nKOTA BATAM\nKOTA TANJUNG PINANG\nPULAU TAMBELAN KAB. BINTAN\nPEKAJANG KAB. LINGGA\nPULAU SERASAN KAB. NATUNA\nPULAU MIDAI KAB. NATUNA\nPULAU LAUT KAB. NATUNA\nKAB. BATANGHARI\nKAB. BUNGO\nKAB. KERINCI\nKAB. MERANGIN\nKAB. MUARO JAMBI\nKAB. SAROLANGUN\nKAB. TANJUNG JABUNG BARAT\nKAB. TANJUNG JABUNG TIMUR\nKAB. TEBO\nKOTA JAMBI\nKOTA SUNGAI PENUH\nKAB. BENGKULU SELATAN\nKAB. BENGKULU TENGAH\nKAB. BENGKULU UTARA\nKAB. KAUR\nKAB. KEPAHIANG\nKAB. LEBONG\nKAB. MUKOMUKO\nKAB. REJANG LEBONG\nKAB. SELUMA\nKOTA BENGKULU\nKAB. BANYUASIN\nKAB. EMPAT LAWANG\nKAB. LAHAT\nKAB. MUARA ENIM\nKAB. MUSI BANYUASIN\nKAB. MUSI RAWAS\nKAB. MUSI RAWAS UTARA\nKAB. OGAN ILIR\nKAB. OGAN KOMERING ILIR\nKAB. OGAN KOMERING ULU\nKAB. OGAN KOMERING ULU SELATAN\nKAB. OGAN KOMERING ULU TIMUR\nKAB. PENUKAL ABAB LEMATANG ILIR\nKOTA LUBUKLINGGAU\nKOTA PAGAR ALAM\nKOTA PALEMBANG\nKOTA PRABUMULIH\nKAB. BANGKA\nKAB. BANGKA BARAT\nKAB. BANGKA SELATAN\nKAB. BANGKA TENGAH\nKAB. BELITUNG\nKAB. BELITUNG TIMUR\nKOTA PANGKAL PINANG\nKAB. LAMPUNG TENGAH\nKAB. LAMPUNG UTARA\nKAB. LAMPUNG SELATAN\nKAB. LAMPUNG BARAT\nKAB. LAMPUNG TIMUR\nKAB. MESUJI\nKAB. PESAWARAN\nKAB. PESISIR BARAT\nKAB. PRINGSEWU\nKAB. TULANG BAWANG\nKAB. TULANG BAWANG BARAT\nKAB. TANGGAMUS\nKAB. WAY KANAN\nKOTA BANDAR LAMPUNG\nKOTA METRO\nKAB. LEBAK\nKAB. PANDEGLANG\nKAB. SERANG\nKAB. TANGERANG\nKOTA CILEGON\nKOTA SERANG\nKOTA TANGERANG\nKOTA TANGERANG SELATAN\nKAB. BANDUNG\nKAB. BANDUNG BARAT\nKAB. BEKASI\nKAB. BOGOR\nKAB. CIAMIS\nKAB. CIANJUR\nKAB. CIREBON\nKAB. GARUT\nKAB. INDRAMAYU\nKAB. KARAWANG\nKAB. KUNINGAN\nKAB. MAJALENGKA\nKAB. PANGANDARAN\nKAB. PURWAKARTA\nKAB. SUBANG\nKAB. SUKABUMI\nKAB. SUMEDANG\nKAB. TASIKMALAYA\nKOTA BANDUNG\nKOTA BANJAR\nKOTA BEKASI\nKOTA BOGOR\nKOTA CIMAHI\nKOTA CIREBON\nKOTA DEPOK\nKOTA SUKABUMI\nKOTA TASIKMALAYA\nKOTA JAKARTA\nKAB. ADMINISTRASI KEPULAUAN SERIBU\nKAB. BANJARNEGARA\nKAB. BANYUMAS\nKAB. BATANG\nKAB. BLORA\nKAB. BOYOLALI\nKAB. BREBES\nKAB. CILACAP\nKAB. DEMAK\nKAB. GROBOGAN\nKAB. JEPARA\nKAB. KARANGANYAR\nKAB. KEBUMEN\nKAB. KENDAL\nKAB. KLATEN\nKAB. KUDUS\nKAB. MAGELANG\nKAB. PATI\nKAB. PEKALONGAN\nKAB. PEMALANG\nKAB. PURBALINGGA\nKAB. PURWOREJO\nKAB. REMBANG\nKAB. SEMARANG\nKAB. SRAGEN\nKAB. SUKOHARJO\nKAB. TEGAL\nKAB. TEMANGGUNG\nKAB. WONOGIRI\nKAB. WONOSOBO\nKOTA MAGELANG\nKOTA PEKALONGAN\nKOTA SALATIGA\nKOTA SEMARANG\nKOTA SURAKARTA\nKOTA TEGAL\nKAB. BANTUL\nKAB. GUNUNGKIDUL\nKAB. KULON PROGO\nKAB. SLEMAN\nKOTA YOGYAKARTA\nKAB. BANGKALAN\nKAB. BANYUWANGI\nKAB. BLITAR\nKAB. BOJONEGORO\nKAB. BONDOWOSO\nKAB. GRESIK\nKAB. JEMBER\nKAB. JOMBANG\nKAB. KEDIRI\nKAB. LAMONGAN\nKAB. LUMAJANG\nKAB. MADIUN\nKAB. MAGETAN\nKAB. MALANG\nKAB. MOJOKERTO\nKAB. NGANJUK\nKAB. NGAWI\nKAB. PACITAN\nKAB. PAMEKASAN\nKAB. PASURUAN\nKAB. PONOROGO\nKAB. PROBOLINGGO\nKAB. SAMPANG\nKAB. SIDOARJO\nKAB. SITUBONDO\nKAB. SUMENEP\nKAB. TRENGGALEK\nKAB. TUBAN\nKAB. TULUNGAGUNG\nKOTA BATU\nKOTA BLITAR\nKOTA KEDIRI\nKOTA MADIUN\nKOTA MALANG\nKOTA MOJOKERTO\nKOTA PASURUAN\nKOTA PROBOLINGGO\nKOTA SURABAYA\nKAB. BADUNG\nKAB. BANGLI\nKAB. BULELENG\nKAB. GIANYAR\nKAB. JEMBRANA\nKAB. KARANGASEM\nKAB. KLUNGKUNG\nKAB. TABANAN\nKOTA DENPASAR\nKAB. BIMA\nKAB. DOMPU\nKAB. LOMBOK BARAT\nKAB. LOMBOK TENGAH\nKAB. LOMBOK TIMUR\nKAB. LOMBOK UTARA\nKAB. SUMBAWA\nKAB. SUMBAWA BARAT\nKOTA BIMA\nKOTA MATARAM\nKAB. ALOR\nKAB. BELU\nKAB. ENDE\nKAB. FLORES TIMUR\nKAB. KUPANG\nKAB. LEMBATA\nKAB. MALAKA\nKAB. MANGGARAI\nKAB. MANGGARAI BARAT\nKAB. MANGGARAI TIMUR\nKAB. NGADA\nKAB. NAGEKEO\nKAB. ROTE NDAO\nKAB. SABU RAIJUA\nKAB. SIKKA\nKAB. SUMBA BARAT\nKAB. SUMBA BARAT DAYA\nKAB. SUMBA TENGAH\nKAB. SUMBA TIMUR\nKAB. TIMOR TENGAH SELATAN\nKAB. TIMOR TENGAH UTARA\nKOTA KUPANG\nKAB. BENGKAYANG\nKAB. KAPUAS HULU\nKAB. KAYONG UTARA\nKAB. KETAPANG\nKAB. KUBU RAYA\nKAB. LANDAK\nKAB. MELAWI\nKAB. MEMPAWAH\nKAB. SAMBAS\nKAB. SANGGAU\nKAB. SEKADAU\nKAB. SINTANG\nKOTA PONTIANAK\nKOTA SINGKAWANG\nKAB. BALANGAN\nKAB. BANJAR\nKAB. BARITO KUALA\nKAB. HULU SUNGAI SELATAN\nKAB. HULU SUNGAI TENGAH\nKAB. HULU SUNGAI UTARA\nKAB. KOTABARU\nKAB. TABALONG\nKAB. TANAH BUMBU\nKAB. TANAH LAUT\nKAB. TAPIN\nKOTA BANJARBARU\nKOTA BANJARMASIN\nKAB. BARITO SELATAN\nKAB. BARITO TIMUR\nKAB. BARITO UTARA\nKAB. GUNUNG MAS\nKAB. KAPUAS\nKAB. KATINGAN\nKAB. KOTAWARINGIN BARAT\nKAB. KOTAWARINGIN TIMUR\nKAB. LAMANDAU\nKAB. MURUNG RAYA\nKAB. PULANG PISAU\nKAB. SUKAMARA\nKAB. SERUYAN\nKOTA PALANGKARAYA\nKAB. BERAU\nKAB. KUTAI BARAT\nKAB. KUTAI KARTANEGARA\nKAB. KUTAI TIMUR\nKAB. MAHAKAM ULU\nKAB. PASER\nKAB. PENAJAM PASER UTARA\nKOTA BALIKPAPAN\nKOTA BONTANG\nKOTA SAMARINDA\nKAB. BULUNGAN\nKAB. MALINAU\nKAB. NUNUKAN\nKAB. TANA TIDUNG\nKOTA TARAKAN\nKAB. BOALEMO\nKAB. BONE BOLANGO\nKAB. GORONTALO\nKAB. GORONTALO UTARA\nKAB. POHUWATO\nKOTA GORONTALO\nKAB. BANTAENG\nKAB. BARRU\nKAB. BONE\nKAB. BULUKUMBA\nKAB. ENREKANG\nKAB. GOWA\nKAB. JENEPONTO\nKAB. KEPULAUAN SELAYAR\nKAB. LUWU\nKAB. LUWU TIMUR\nKAB. LUWU UTARA\nKAB. MAROS\nKAB. PANGKAJENE DAN KEPULAUAN\nKAB. PINRANG\nKAB. SIDENRENG RAPPANG\nKAB. SINJAI\nKAB. SOPPENG\nKAB. TAKALAR\nKAB. TANA TORAJA\nKAB. TORAJA UTARA\nKAB. WAJO\nKOTA MAKASSAR\nKOTA PALOPO\nKOTA PAREPARE\nKAB. BOMBANA\nKAB. BUTON\nKAB. BUTON SELATAN\nKAB. BUTON TENGAH\nKAB. BUTON UTARA\nKAB. KOLAKA\nKAB. KOLAKA TIMUR\nKAB. KOLAKA UTARA\nKAB. KONAWE\nKAB. KONAWE KEPULAUAN\nKAB. KONAWE SELATAN\nKAB. KONAWE UTARA\nKAB. MUNA\nKAB. MUNA BARAT\nKAB. WAKATOBI\nKOTA BAU-BAU\nKOTA KENDARI\nKAB. BANGGAI\nKAB. BANGGAI KEPULAUAN\nKAB. BANGGAI LAUT\nKAB. BUOL\nKAB. DONGGALA\nKAB. MOROWALI\nKAB. MOROWALI UTARA\nKAB. PARIGI MOUTONG\nKAB. POSO\nKAB. SIGI\nKAB. TOJO UNA-UNA\nKAB. TOLI-TOLI\nKOTA PALU\nKAB. BOLAANG MONGONDOW\nKAB. BOLAANG MONGONDOW SELATAN\nKAB. BOLAANG MONGONDOW TIMUR\nKAB. BOLAANG MONGONDOW UTARA\nKAB. KEPULAUAN SANGIHE\nKAB. KEPULAUAN SIAU TAGULANDANG BIARO\nKAB. KEPULAUAN TALAUD\nKAB. MINAHASA\nKAB. MINAHASA SELATAN\nKAB. MINAHASA TENGGARA\nKAB. MINAHASA UTARA\nKOTA BITUNG\nKOTA KOTAMOBAGU\nKOTA MANADO\nKOTA TOMOHON\nKAB. MAJENE\nKAB. MAMASA\nKAB. MAMUJU\nKAB. MAMUJU TENGAH\nKAB. MAMUJU UTARA\nKAB. POLEWALI MANDAR\nKAB. BURU\nKAB. BURU SELATAN\nKAB. KEPULAUAN ARU\nKAB. MALUKU BARAT DAYA\nKAB. MALUKU TENGAH\nKAB. MALUKU TENGGARA\nKAB. MALUKU TENGGARA BARAT\nKAB. SERAM BAGIAN BARAT\nKAB. SERAM BAGIAN TIMUR\nKOTA AMBON\nKOTA TUAL\nKAB. HALMAHERA BARAT\nKAB. HALMAHERA TENGAH\nKAB. HALMAHERA UTARA\nKAB. HALMAHERA SELATAN\nKAB. KEPULAUAN SULA\nKAB. HALMAHERA TIMUR\nKAB. PULAU MOROTAI\nKAB. PULAU TALIABU\nKOTA TERNATE\nKOTA TIDORE KEPULAUAN\nKAB. ASMAT\nKAB. BIAK NUMFOR\nKAB. BOVEN DIGOEL\nKAB. DEIYAI\nKAB. DOGIYAI\nKAB. INTAN JAYA\nKAB. JAYAPURA\nKAB. JAYAWIJAYA\nKAB. KEEROM\nKAB. KEPULAUAN YAPEN\nKAB. LANNY JAYA\nKAB. MAMBERAMO RAYA\nKAB. MAMBERAMO TENGAH\nKAB. MAPPI\nKAB. MERAUKE\nKAB. MIMIKA\nKAB. NABIRE\nKAB. NDUGA\nKAB. PANIAI\nKAB. PEGUNUNGAN BINTANG\nKAB. PUNCAK\nKAB. PUNCAK JAYA\nKAB. SARMI\nKAB. SUPIORI\nKAB. TOLIKARA\nKAB. WAROPEN\nKAB. YAHUKIMO\nKAB. YALIMO\nKOTA JAYAPURA\nKAB. YAPEN WAROPEN\nKAB. FAKFAK\nKAB. KAIMANA\nKAB. MANOKWARI\nKAB. MANOKWARI SELATAN\nKAB. MAYBRAT\nKAB. PEGUNUNGAN ARFAK\nKAB. RAJA AMPAT\nKAB. SORONG\nKAB. SORONG SELATAN\nKAB. TAMBRAUW\nKAB. TELUK BINTUNI\nKAB. TELUK WONDAMA\nKOTA SORONG";
var slider2 = document.getElementById("volumeId");
var output2 = document.getElementById("volLabelId");
var slider3 = document.getElementById("volume2Id");
var output3 = document.getElementById("volLabel2Id");
var slider4 = document.getElementById("brightnessId");
var output4 = document.getElementById("brightnessLabelId");

slider2.oninput = function() {
  output2.innerHTML = this.value;
}
slider3.oninput = function() {
  output3.innerHTML = this.value;
}
slider4.oninput = function() {
  output4.innerHTML = this.value;
}

function myFunction(){
  var contentArr = kabupaten.split("\n");
  var iframe2 = document.getElementById('koordinat');
  var content2 = iframe2.contentWindow.document.body.innerText;
  var contentArr2 = content2.split("\n");
  var iframe4 = document.getElementById('volume');
  var content4 = iframe4.contentWindow.document.body.innerText;
  var contentArr4 = content4.split("\n");
  var iframe5 = document.getElementById('koreksi');
  var content5 = iframe5.contentWindow.document.body.innerText;
  var contentArr5 = content5.split("\n");
  var iframe6 = document.getElementById('volume2');
  var content6 = iframe6.contentWindow.document.body.innerText;
  var contentArr6 = content6.split("\n");
  var iframe7 = document.getElementById('dispStat');
  var content7 = iframe7.contentWindow.document.body.innerText;
  var contentArr7 = content7.split("\n");
  var iframe8 = document.getElementById('brightness');
  var content8 = iframe8.contentWindow.document.body.innerText;
  var contentArr8 = content8.split("\n");
  if(contentArr7[0]=="0"){
    document.getElementById("volumeId").disabled = true;
    document.getElementById("volume2Id").disabled = true;
  }
  var x = document.getElementById("koordinatId");
  for (i = 0; i < contentArr.length; i++) {
    var option = document.createElement("option");
    option.text = contentArr[i].split(",")[0];
    option.value = i;
    x.add(option);
    if(i==parseInt(contentArr2[0])){
      x.options.selectedIndex = i;
    }
  }
  var y = document.getElementById("timezoneId");
  var z = ["WIB","WITA","WIT"];
  for (i = 7; i < 10; i++) {
    var option2 = document.createElement("option");
    option2.text = z[i-7];
    option2.value = i;
    y.add(option2);
    if(i==parseInt(contentArr2[1])){
      y.options.selectedIndex = i-7;
    }
  }
  document.getElementById("volumeId").value = parseInt(contentArr4[0]);
  document.getElementById("volLabelId").innerHTML = contentArr4[0];
  document.getElementById("volume2Id").value = parseInt(contentArr6[0]);
  document.getElementById("volLabel2Id").innerHTML = contentArr6[0];  
  document.getElementById("brightnessId").value = parseInt(contentArr8[0]);
  document.getElementById("brightnessLabelId").innerHTML = contentArr8[0];
  document.getElementById("koreksiImsakId").value = parseInt(contentArr5[0]);
  document.getElementById("koreksiSubuhId").value = parseInt(contentArr5[1]);
  document.getElementById("koreksiTerbitId").value = parseInt(contentArr5[2]);
  document.getElementById("koreksiDhuhaId").value = parseInt(contentArr5[3]);
  document.getElementById("koreksiZuhurId").value = parseInt(contentArr5[4]);
  document.getElementById("koreksiAsarId").value = parseInt(contentArr5[5]);
  document.getElementById("koreksiMagribId").value = parseInt(contentArr5[7]);
  document.getElementById("koreksiIsyaId").value = parseInt(contentArr5[8]);
  document.getElementById("sudutSubuhId").value = parseInt(contentArr5[9]);
  document.getElementById("sudutIsyaId").value = parseInt(contentArr5[10]);
  document.getElementById("koreksiHijriId").value = parseInt(contentArr5[11]);
}

function homeButton() {
  window.open("/","_self");
}

</script>
</body>
</html>

)=====";

//------------------------------------------------------------------------------------------
