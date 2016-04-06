var fs = require('fs')  

function readCallBack(err,data)
{
var buf = data
var str = buf.toString();
var strArray = str.split("\n");
for (var i = 0; i < strArray.length; i++) {
	var tempStr = strArray[i].split(".");
		console.log(tempStr[1]);
}

}

fs.readdir(process.argv[2],readCallBack);
