#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

#define WIFI_SSID ""
#define WIFI_PASS ""

const String chatbot = R"=====(
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
  </head>


    <style>
      body {
        background-color: #f8f9fa;
      }

      .main-container {
        min-height: 100vh;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
      }

      .spinner-border {
        display: none;
      }
    </style>

    <title>MicroFiler</title>
  </head>
  <body>
    <div class="container main-container">
      <div class="row">
        <div class="col-md-12 text-center">
          <h1 class="my-4">MicroFiler</h1>
          <p class="my-4">1 Download, 2 GB per file</h1>
          <form id="fileUploadForm">
            <div class="mb-3">
              <label for="fileInput" class="form-label">Select a file</label>
              <input type="file" class="form-control" id="fileInput" required>
            </div>
            <button type="submit" class="btn btn-primary">Upload</button>
          </form>
          <div class="spinner-border text-primary mt-3" role="status" id="spinner">
            <span class="visually-hidden">Loading...</span>
          </div>
          <div id="result" class="mt-4"></div>
        </div>
      </div>
    </div>
    <script src="https://code.jquery.com/jquery-3.6.0.min.js"></script>
    <script src="https://unpkg.com/@popperjs/core@2"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js" integrity="sha384-geWF76RCwLtnZ8qwWowPQNguL3RmwHVBC9FhGdlKrxdiJJigb/j/68SIy3Te4Bkz" crossorigin="anonymous"></script>

    <script>
      $(document).ready(function () {
        $('#fileUploadForm').on('submit', function (e) {
          e.preventDefault();

          const fileInput = document.getElementById('fileInput');
          const formData = new FormData();

          formData.append('file', fileInput.files[0]);
          formData.append('expires', '1y');
          formData.append('maxDownloads', '1');
          formData.append('autoDelete', 'true');

          document.getElementById('spinner').style.display = 'inline-block';

          fetch('https://file.io/', {
            method: 'POST',
            body: formData
          })
          .then(response => response.json())
          .then(data => {
            const resultDiv = document.getElementById('result');
            resultDiv.innerHTML = `<a href="${data.link}" target="_blank">${data.link}</a>`;
            document.getElementById('spinner').style.display = 'none';
          })
          .catch(error => {
            console.error('Error:', error);
            document.getElementById('spinner').style.display = 'none';
          });
        });
      });
    </script>
  </body>
</html>
)=====";
void handleRoot() {
  server.send(200, "text/html", chatbot);
}
 
void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
