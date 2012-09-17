front = {
   cascades = {"./cascades/haarcascade_frontalface_alt2.xml"}, 
   -- Objects smaller that are ignored
   minsize = { 30, 30 },
   nearobjs = 4,
   enabled = true,
   debug = true, 
   -- Set the haar classifier flags: 
   -- rough == do a rough search
   -- biggest == find biggest object
   -- cunny == do cunny pruning
   -- scale == scale image. 
   -- See OpenCV docs to make sense what it does 
   flags = { "rough", "biggest", "canny" }, 
   simple_handlers = { "face_detected" },
   shape_handlers = { "shape_processing" },
}

eyes = {
   cascades = {"./cascades/haarcascade_mcs_lefteye.xml"}, 
   minsize = { 30, 30 },
   nearobjs = 4,
   enabled = true,
   debug = true, 
   flags = { "rough", "canny" }, 
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
   detect = { "front" },
}


function face_detected(count)
   print("I see " .. count .. " faces");
end



