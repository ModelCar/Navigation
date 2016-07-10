var fs = require("fs");

var jsonData;
fs.readFile("/Users/Alex/Dropbox/Study/LEGOcar/sat_data.txt", function(err, data) {
	if(err) throw err;
	jsonData = data;

	var data = JSON.parse(jsonData);
	console.log(JSON.stringify(data[0]));
	data.splice(0, 1);

	fs.writeFile("/Users/Alex/Dropbox/Study/LEGOcar/sat_data.txt", JSON.stringify(data), function() {

	});
});