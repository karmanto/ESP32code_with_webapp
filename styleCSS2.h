//------------------------------------------------------------------------------------------
const char styleCSS2[] PROGMEM = R"=====(

.switch {position: relative; display: inline-block; width: 120px; height: 68px} 

.switch input {display: none}

.slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}

.slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}

input:checked+.slider {background-color: #2196F3}

input:disabled+.slider {background-color: #666}

input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}

)=====";

//------------------------------------------------------------------------------------------
