print(" === Necromant's facelock daemon 0.1, entering Lua Environment === \n");   

face = {
   cascades = {"./cascades/haarcascade_frontalface_alt2.xml", "./cascades/haarcascade_profileface.xml"}, 
   maxsize = { 30, 30 },
   nearobjs = 4,
   enabled = true,
   debug = true, 
   simple_handlers = { "face_detected" },
   shape_handlers = { "shape_processing" },
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


function face_detected(count)
   print("I see " .. count .. " faces");
end

print("boot: opening up cameras\n");   


