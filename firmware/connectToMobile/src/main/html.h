#ifndef HTML_H 
#define HTML_H 

const char html_code[] = R"rawliteral(
<!doctype html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>WiFi Setup</title>
    <style>
      #ssid, #pwd {
        color: aqua;
        font-size: 1.5em;
      }

      #OKbtn {
        width: 9em;
        height: 4em;
        font-size: 18px;
        letter-spacing: 2px;
        border-radius: 40px;
        background: linear-gradient(90deg, #0162c8, #55e7fc);
        color: #775bea;
        cursor: pointer;
      }

      #OKbtn:hover {
        background: linear-gradient(90deg, #775bea, #ff72c0);
        color: #fff;
      }
    </style>
  </head>

  <body>
    <h1>Enter Your SSID and Password</h1>

    <form action="/connect" method="GET">
      <input type="text" name="ssid" placeholder="YOUR SSID..." required><br><br>
      <input type="password" name="pwd" placeholder="YOUR PASSWORD..." required><br><br>
      <input type="submit" value="OKAY" id="OKbtn">
    </form>
  </body>
</html>
)rawliteral";

#endif