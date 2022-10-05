//------------------------------------------------------------------------------------------
const char styleCSS[] PROGMEM = R"=====(

body {align-items: center; background-color: #000; display: flex; justify-content: center;}

.form {background-color: #15172b; border-radius: 20px; box-sizing: border-box; padding: 20px; width: 320px;}

.title {color: #eee; font-family: sans-serif; font-size: 25px; font-weight: 600; margin-top: 30px; text-align:center;}

.subtitle {color: #eee; font-family: sans-serif; font-size: 16px; font-weight: 600; margin-top: 10px;}

.input:disabled { background-color: #808080;}

.input {background-color: #303245; border-radius: 12px; border: 0; box-sizing: border-box; color: #eee; font-size: 14px; height: 100%; outline: 0; padding: 4px 20px 0; width: 100%;}

.input-container {height: 50px; position: relative; width: 100%;}

.ic1 {margin-top: 40px;}

.ic2 {margin-top: 30px;}

.ic3 {margin-top: 10px;}

.cut {background-color: #15172b; border-radius: 10px; height: 20px; left: 20px; position: absolute; top: -20px; transform: translateY(0); transition: transform 200ms; width: 170px;}

.cut-short {width: 50px;}

.input:focus ~ .cut,

.input:not(:placeholder-shown) ~ .cut {transform: translateY(8px);}

.placeholder {color: #65657b; font-family: sans-serif; left: 20px; line-height: 14px; pointer-events: none; position: absolute; transform-origin: 0 50%; transition: transform 200ms, color 200ms; top: 20px;}

.input:focus ~ .placeholder,

.input:not(:placeholder-shown) ~ .placeholder {transform: translateY(-30px) translateX(10px) scale(0.75);}

.input:not(:placeholder-shown) ~ .placeholder {color: #808097;}

.input:focus ~ .placeholder {color: #dc2f55;}

.submit {background-color: #08d; border-radius: 12px; border: 0; box-sizing: border-box; color: #eee; cursor: pointer; font-size: 14px; height: 50px; margin-top: 38px; // outline: 0; text-align: center; width: 100%;}

.submit:disabled {background-color: #666}

.submit:active {background-color: #06b;}

)=====";

//------------------------------------------------------------------------------------------
