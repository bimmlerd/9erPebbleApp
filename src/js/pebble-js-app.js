var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};

function getConnectionData() {
  // URL                                                  from ETH      to Bellevue, just one, only direct
  var url = "http://transport.opendata.ch/v1/connections?from=008591123&to=008576193&limit=1&direct=1";
  
  // send request
  xhrRequest(url, 'GET',
             function(responseText) {
               // responseText contains JSON object with connection info
               var json = JSON.parse(responseText);
               var date = new Date(json.connections[0].from.departureTimestamp*1000);
               
               var hour = date.getHours();
               var minutes = date.getMinutes();
               if (minutes < 10) {
                 minutes = "0" + minutes;
               }
               var departureTime = hour + ":" + minutes;
               var dictionary = {
                 "KEY_DEPARTURETIME": departureTime,
               };
               
               Pebble.sendAppMessage(dictionary,
                      function(e) {
                       console.log("Connection info sent to Pebble successfully!");
                     },
                      function(e) {
                       console.log("Error sending connection info to Pebble!");
                     });                   
               });
}


// Called when JS is ready
Pebble.addEventListener("ready",
                        function(e) {
                          getConnectionData();
                        });

// Called when incoming message from the Pebble is received
Pebble.addEventListener("appmessage",
                        function(e) {
                          console.log("Received Status: " + e.payload.status);
                        });