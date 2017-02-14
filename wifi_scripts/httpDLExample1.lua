-- ESP8266-HTTP Library
-- Written 2014 by Tobias M�del (t.maedel@alfeld.de)
-- Licensed unter MIT

-- Clear the wtachdog times and Download a file
tmr.wdclr()

-- import a module from another script
httpDL = require("httpDL")

-- run garbage collecton
collectgarbage()


-- call the function from the other script (imported above) and download a jpg image of UBC campus and store
-- on the WiFi dongle file system as file "main2.jpg". The callback function related to completion is empty
httpDL.download("www.ubc.ca", 80, "/_assets/img/aerial-main-mall-800x253.jpg", "main2.jpg", function (s) end)


--tidy up after request
httpDL = nil
package.loaded["httpDL"]=nil
collectgarbage()