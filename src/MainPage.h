const char* MainPageHtml1 = R"-----(
<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8" />
    <title>EagleDecorations Main</title>
</head>
<body>
    <h1>EagleDecorations Controller</h1>

    <h3><a href="/Provision">Controller Setup</a></h3>

<h1>Control</h1>

<a href="http://www.riderx.info/provisioning-and-using-the-esp8266-controller/">Command Documentation</a>

<h3><a href="/Color">Color Page</a></h3>

)-----";

const char* MainPageHtml2 = R"-----(

<h2>Examples</h2>

<p><b>Blend to violet color over one second</b>&nbsp;
<a href="/message?r=1&content=rgbx180,0,180,100">rgbx180,0,180,100</a></p>

<p><b>Yellow blue alternate</b>&nbsp;
<a href="/message?r=1&content=alt,150,150,000,000,000,150,250">alt,150,150,000,000,000,150,250</a></p>

<p><b>Individual addressing red green blue, 11 leds each</b>&nbsp;
<a href="/message?r=1&content=indx011,000044440000004400">indx011,000044440000004400</a></p>

<p><b>Flash and fade</b>&nbsp;
<a href="/message?r=1&content=fdcx250,10,500">fdcx250,10,500</a></p>

<p><b>Color rotate</b>&nbsp;
<a href="/message?r=1&content=colx5000,180">colx5000,180</a></p>

<p><b>Chunks - bottom/middle/top</b>&nbsp;
<a href="/message?r=1&content=chunkx15,0,255,0,15,0,0,255,3,255,0,0">chunkx15,0,255,0,15,0,0,255,3,255,0,0</a></p>

<p><b>Random flash and hold</b>&nbsp;
<a href="/message?r=1&content=rfx3,40,200">rfx3,40,200</a></p>

<p><b>White Breathe (compound command)</b>&nbsp;
<a href="/message?r=1&content=$200$rgbx255,255,255,200$200$rgbx0,0,0,200">$200$rgbx255,255,255,200$200$rgbx0,0,0,200</a></p>

<p><b>Chaser</b>&nbsp;
<a href="/message?r=1&content=chase_0,200,0,5,1,1,1">chasex0,200,0,5,1,1,1</a></p>

<p><b>Save current color/animation</b>&nbsp;
<a href="/message?r=1&content=s">s</a></p>

<br/>
<p><b>Change LED count to 10 (will reboot!)</b>&nbsp;
<a href="/message?r=1&content=nx10">nx10</a></p>

<h2>Command</h2>
<form action="/message">
    <input type="hidden" name="r" value="1" />
)-----";

const char* MainPageHtml3 = R"-----(
    <input type="submit" value="Execute" />
</form>

<h2>&nbsp;</h2>

</body>
</html>
)-----";

// PROVISIONING PAGE

const char* ProvisionPageHtml1 = R"-----(
<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8" />
    <title>EagleDecorations Setup</title>
</head>
<body>
    <h1>EagleDecorations Controller Setup</h1>

        <h3><a href="/">Main Page</a></h3>


)-----";

const char* ProvisionPageHtml2 = R"-----(

    <h1>Wireless Provisioning</h1>
<p>
    Chose a network from the list or enter one directly.
</p>
<h2>Visible Networks</h2>
<form>
    <select name="ssid">
)-----";

const char* ProvisionPageHtml3 = R"-----(
    </select>
    &nbsp;&nbsp; Password: <input type="text" name="password"/>
    <input type="submit" value="Register"/>
</form>
<h3>Other Network</h3>
<form>
    SSID: <input type="text" name="ssid" />
    &nbsp;&nbsp; Password: <input type="text" name="password"/>
    <input type="submit" value="Register"/>
</form>

<h2>Other</h2>

<p><b>Reset configuration (will reboot!): </b>&nbsp;
<a href="/message?r=1&content=reset">Reset</a></p>


<h3>Set LED count (will reboot!)</h3>
<form action="/message">
    <input type="hidden" name="r" value="1"/>
    <input type="hidden" name="ledcount" value="1"/>
    Count: <input type="text" name="content" />&nbsp;&nbsp;
    <input type="submit" value="Execute"/>
</form>

</body>
</html>
)-----";

const char* ColorPageHtml1 = R"-----(
<!DOCTYPE html>

<html>
<head>
    <meta charset="utf-8" />
    <title>EagleDecorations Color Page</title>
</head>
<body>
    <h1>Color Page</h1>

    <h3><a href="/">Main Page</a></h3>

<h2>Direct colors (single) - Direct colors (series)</h2>

<p><b>Note</b> that bright colors may overheat your esp controller or not be full brightness if you are powering through USB. Single colors will
set a single color; series will add the color to the current animation series.</p>
)-----";

const char* ColorPageHtml2 = R"-----(

<h2>&nbsp;</h2>

</body>
</html>
)-----";
