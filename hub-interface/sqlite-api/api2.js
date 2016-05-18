var sqlite3 = require('sqlite3').verbose();
//ar db = new sqlite3.Database('test6.db');

var bodyParser = require('body-parser')





var express = require('express');


var restapi = express();
// parse application/x-www-form-urlencoded
restapi.use(bodyParser.urlencoded({ extended: false }))

// parse application/json
restapi.use(bodyParser.json())
var allowCrossDomain = function(req, res, next) {
    res.header('Access-Control-Allow-Origin', '*');
    res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE,OPTIONS');
    res.header('Access-Control-Allow-Headers', 'Content-Type, Authorization, Content-Length, X-Requested-With');

    // intercept OPTIONS method
    if ('OPTIONS' == req.method) {
      res.send(200);
    }
    else {
      next();
    }
};

restapi.use(allowCrossDomain);



//PUT /rules Updates rule based on id
restapi.put('/config/:config_name', function(req, res){
	var db = new sqlite3.Database('../../hub-core/Debug/test6.db');
	var sql = "UPDATE config SET value = '" + req.body.value +"' WHERE name = '" + req.params.config_name + "'";
	db.serialize(function() {
	    db.exec("BEGIN");
	    db.run(sql, function(err, row){
		    if (!err){
		        console.log("Rule updated");
		        res.json({ message: 'Rule updated' });
		    } else {
				console.log("Rule failed to update");
				res.json({ message: 'Rule failed to update' });
			}
			
	    });
	    db.exec("COMMIT");
	    db.close();
	});
	
});


restapi.listen(3000);

console.log("API started on http://localhost:3000");
