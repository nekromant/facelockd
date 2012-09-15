print(" === Necromant's facelock daemon 0.1, entering Lua Environment === \n");   

face = {
   cascades = {"./haarcascade_frontalface_alt2.xml", "./haarcascade_profileface.xml"}, 
   maxsize = { 30, 30 },
   nearonjs = 4,
   enabled = true,
   debug = true, 
}

frontcam = {
   camid = 0, 
   w = 320,
   h = 240,
   maxfps = 5,
   framedelay = 100,
   enabled = true,
   debug = true,
   cascadeprofiling=true,
   detect = { "face" },
}


print("boot: opening up cameras\n");   


