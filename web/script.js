

function sendCmd(param) {

  console.log('A enviar comando "' + param + '"...');

  fetch("/path", {
    method: "POST",
    headers: {
      "Content-Type": "application/x-www-form-urlencoded"
    },
    body: "param=" + encodeURIComponent(param)
  })
    .then(response => response.text())
    .then(text => {
      console.log(text);
      document.getElementById("status").innerText = text;
    })
    .catch(err => {
      console.log("Erro ao comunicar com o ESP32: " + err);
    });
}


function updateTemperature() {
    fetch("/path")
        .then(response => response.json())
        .then(data => {
            const tempEl = document.getElementById("temperature");
            tempEl.textContent = data.temperature.toFixed(2) + " °C";
        })
        .catch(err => {
            console.error("Temperature read error:", err);
        });
}


setInterval(updateTemperature, 1000);
