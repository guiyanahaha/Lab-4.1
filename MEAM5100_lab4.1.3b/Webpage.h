const char body[] PROGMEM = R"===( 
<!DOCTYPE html>
<html>
  <body>
    <h1>LED Frequency/ DutyCycle Sliders</h1>

    <div class="slidecontainer">
      <p>Frequency 3Hz - 30Hz:</p>
      <input type="range" min="3" max="30" value="17" id="frequencySlider">
      <span id="frequencyValue">17 Hz</span> <br>

      <p>Duty Cycle 0% - 100%:</p>
      <input type="range" min="0" max="100" value="50" id="dutyCycleSlider">
      <span id="dutyCycleValue">50 %</span> <br>
    </div>
  </body>
  <script>
    var frequencySlider = document.getElementById("frequencySlider");
    var dutyCycleSlider = document.getElementById("dutyCycleSlider");

    frequencySlider.oninput = function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("frequencyValue").innerHTML = this.responseText + " Hz";
        }
      };
      var str = "/setFrequency?val="; // Modify this URL to match your backend endpoint
      var res = str.concat(this.value);
      xhttp.open("GET", res, true);
      xhttp.send();
    }

    dutyCycleSlider.oninput = function() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("dutyCycleValue").innerHTML = this.responseText + "%";
        }
      };
      var str = "/setDutyCycle?val="; // Modify this URL to match your backend endpoint
      var res = str.concat(this.value);
      xhttp.open("GET", res, true);
      xhttp.send();
    }
  </script>
</html>

)===";
