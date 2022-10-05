//------------------------------------------------------------------------------------------
const char styleCSS3[] PROGMEM = R"=====(

.switch {position: relative; display: inline-block; width: 46px; height: 26px} 

.switch input {display: none}

.slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 12px}

.slider:before {position: absolute; content: ""; height: 20px; width: 20px; left: 3px; bottom: 3px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 26px}

input:checked+.slider {background-color: #2196F3}

input:disabled+.slider {background-color: #666}

input:checked+.slider:before {-webkit-transform: translateX(20px); -ms-transform: translateX(20px); transform: translateX(20px)}

)=====";

//------------------------------------------------------------------------------------------
