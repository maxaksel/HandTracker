C:
cd C:\Program Files\FlightGear 2020.3

SET FG_ROOT=C:\Program Files\FlightGear 2020.3\data
.\\bin\fgfs --aircraft=c172p --fdm=null --native-fdm=socket,in,30,localhost,5502,udp --fog-fastest --disable-clouds --start-date-lat=2004:06:01:09:00:00 --disable-sound --in-air --enable-freeze --airport=PHNL --runway=08L --altitude=7224 --heading=113 --offset-distance=4.72 --offset-azimuth=0 --prop:/sim/rendering/shaders/quality-level=0
