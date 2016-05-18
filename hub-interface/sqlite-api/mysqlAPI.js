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
}
restapi.use(allowCrossDomain);


var mysql      = require('mysql');
var connection = mysql.createConnection({
  host     : 'localhost',
  user     : 'root',
  password : 'brettnet',
  database : 'ruleEngine',
  timezone  : 'Europe/London'
});

connection.connect(function(err) {
  if (err) {
    console.error('error connecting: ' + err.stack);
    return;
  }
  console.log('connected as id ' + connection.threadId);
});





//POST /rules Creates a new rule
restapi.post('/rules', function(req, res){
	console.log("POST /rules Creates a new rule");
	var sql = "INSERT INTO rule (rule_name,rule_description,rule_active) VALUES ('" + req.body.rule_name + "','" + req.body.rule_description + "'," + req.body.rule_active + ")";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			var rule = req.body;
			rule.rule_id = results.insertId;
			res.json(rule);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Rule failed to insert' });
		}
	});
});

//GET /rules Returns all rules
restapi.get('/rules', function(req, res){
	console.log("GET /rules Returns all rules");
	var sql = "SELECT * FROM rule";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Rule failed to return' });
		}
	});
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/rules/:rule_id', function(req, res){
	console.log("GET /rules/:rule_id Returns rule based on id:?",req.params.rule_id);
	var sql = "SELECT * FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Rule failed to return' });
		}
	});
});

//PUT /rules Updates rule based on id
restapi.put('/rules/:rule_id', function(req, res){
	console.log("PUT /rules Updates rule based on id:?",req.params.rule_id);
	var sql = "UPDATE rule SET rule_name = '" + req.body.rule_name + "', rule_description = '" + req.body.rule_description + "', rule_active = " + req.body.rule_active +" WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Rule updated' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Rule failed to update' });
		}
	});
});

//DELETE /rules deletes rule based on id
restapi.delete('/rules/:rule_id', function(req, res){
	console.log("DELETE /rules deletes rule based on id:?",req.params.rule_id);
	var sql = "DELETE FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Rule delete' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Rule failed to delete' });
		}
	});
});




//



//POST /rules Creates a new constraint
restapi.post('/rules/:rule_id/constraints', function(req, res){
	console.log("POST /rules/:rule_id/constraints");
	var sql = "INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (" + req.body.rule_id + ",'" + req.body.field_id + "','" + req.body.constraint_operator + "'," + req.body.constraint_value + ")";
	console.log(sql);
	
	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			var constraint = req.body;
			constraint.constraint_id = results.insertId;
			res.json(constraint);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Constraint failed to insert' });
		}
	});
});

//GET /rules/:rule_id/constraints Returns all contraints for rule id
restapi.get('/rules/:rule_id/constraints', function(req, res){
	console.log("GET /rules/:rule_id/constraints Returns all constraints for rule id" +  req.params.rule_id);
	var sql = "SELECT * FROM rule_constraint WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Constraints failed to return' });
		}
	});
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("GET /rules/:rule_id/constraints/:constraint_id RETURN");
	var sql = "SELECT * FROM rule WHERE constraint_id=" + req.params.constraint_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Constraint failed to return' });
		}
	});
});

//PUT /rules Updates rule based on id
restapi.put('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("PUT /rules/:rule_id/constraints/:constraint_id UPDATE");
	var sql = "UPDATE rule_constraint SET field_id = '" + req.body.field_id + "', constraint_operator = '" + req.body.constraint_operator + "', constraint_value = " + req.body.constraint_value +" WHERE constraint_id=" + req.params.constraint_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Constraint updated' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Constraint failed to update' });
		}
	});
});

//DELETE /rules deletes rule based on id
restapi.delete('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("DELETE /rules/:rule_id/constraints/:constraint_id DELETE");
	var sql = "DELETE FROM rule_constraint WHERE constraint_id=" + req.params.constraint_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Constraint deleted' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Constraint failed to delete' });
		}
	});
});




//POST /rules Creates a new action
restapi.post('/rules/:rule_id/actions', function(req, res){
	console.log("POST /rules Creates a new rule");
	var sql = "INSERT INTO rule_action (rule_id,field_id,action_operator,action_value) VALUES (" + req.body.rule_id + ",'" + req.body.field_id + "','" + req.body.action_operator + "'," + req.body.action_value + ")";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			var action = req.body;
			action.action_id = results.insertId;
			res.json(action);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Action failed to insert' });
		}
	});
});

//GET /rules/:rule_id/actions Returns all contraints for rule id
restapi.get('/rules/:rule_id/actions', function(req, res){
	console.log("GET /rules/:rule_id/actions Returns all contraints for rule id"+ req.params.rule_id);
	var sql = "SELECT * FROM rule_action WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Actions failed to return' });
		}
	});
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/rules/:rule_id/actions/:action_id', function(req, res){
	console.log("GET /rules/:rule_id Returns rule based on id:?",req.params.rule_id);
	var sql = "SELECT * FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Action failed to return' });
		}
	});
});

//PUT /rules Updates rule based on id
restapi.put('/rules/:rule_id/actions/:action_id', function(req, res){
	console.log("PUT /rules/:rule_id/actions/:action_id UPDATE");
	var sql = "UPDATE rule_action SET field_id = '" + req.body.field_id + "', action_operator = '" + req.body.action_operator + "', action_value = " + req.body.action_value +" WHERE action_id=" + req.params.action_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Action updated' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Action failed to update' });
		}
	});
});

//DELETE /rules deletes rule based on id
restapi.delete('/rules/:rule_id/actions/:action_id', function(req, res){
	console.log("DELETE /rules/:rule_id/actions/:action_id DELETE");
	var sql = "DELETE FROM rule_action WHERE action_id=" + req.params.action_id;
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Action deleted' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Action failed to delete' });
		}
	});
});



//GET /fieldSelect Returns an array of field ID and field name for field
restapi.get('/fieldIDName', function(req, res){
	console.log("GET /fieldIDName RETURN");
	var sql = "SELECT field.field_id, field.field_name, device.device_name FROM field INNER JOIN device ON field.device_id=device.device_id";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'fieldIDName failed to return' });
		}
	});
});

restapi.get('/fieldIDNameCanSet', function(req, res){
	console.log("GET /fieldIDNameCanSet Returns an array of field ID, field name and device name for fields that can be set");
	var sql = "SELECT field.field_id, field.field_name, device.device_name FROM field INNER JOIN device ON field.device_id=device.device_id WHERE field.field_can_set = 1";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'fieldIDNameCanSet failed to return' });
		}
	});
});

restapi.get('/countRules', function(req, res){
	//console.log("GET /countRules");
	var sql = "SELECT count(*) FROM rule";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'countRules failed to return' });
		}
	});
});

restapi.get('/countActiveRules', function(req, res){
	//console.log("GET /countActiveRule");
	var sql = "SELECT count(*) FROM rule WHERE rule_active = 1";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'countActiveRule failed to return' });
		}
	});
});

restapi.get('/countDevices', function(req, res){
	//console.log("GET /countDevices");
	var sql = "SELECT count(*) FROM device";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'countDevices failed to return' });
		}
	});
});

restapi.get('/countFields', function(req, res){
	//console.log("GET /countFields");
	var sql = "SELECT count(*) FROM field";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'countFields failed to return' });
		}
	});
});

restapi.get('/state-history/:limit', function(req, res){
	//console.log("GET /config Returns an array of config");
	var sql = "SELECT * FROM state_history ORDER BY state_time DESC LIMIT " + req.params.limit;
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'state-history failed to return' });
		}
	});
});

restapi.get('/rule-history/:limit', function(req, res){
	//console.log("GET /config Returns an array of config");
var sql = "SELECT rule_history.rule_history_id, rule_history.rule_history_time, rule.rule_name FROM rule_history INNER JOIN rule ON rule_history.rule_id=rule.rule_id ORDER BY rule_history.rule_history_time DESC LIMIT " + req.params.limit;
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'rule-history failed to return' });
		}
	});
});

restapi.get('/count-rule-history/:days', function(req, res){
	//console.log("GET /config Returns an array of config");
	var sql = "SELECT count(*) FROM rule_history WHERE rule_history_time > NOW() - INTERVAL " + req.params.days + " DAY";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'count-rule-history failed to return' });
		}
	});
});

restapi.get('/count-state-history/:days', function(req, res){
	//console.log("GET /config Returns an array of config");
	var sql = "SELECT count(*) FROM state_history WHERE state_time > NOW() - INTERVAL " + req.params.days + " DAY";
	//var sql = "select field_id,field_name from field";
	//console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			//console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'count-state-history failed to return' });
		}
	});
});


restapi.get('/fields', function(req, res){
	console.log("GET /config Returns an array of config");
	var sql = "SELECT * FROM field";
	//var sql = "select field_id,field_name from field";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Fields failed to return' });
		}
	});
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/config/:config_name', function(req, res){
	console.log("GET /config/:config_name");
	var sql = "SELECT * FROM config WHERE name = '" + req.params.config_name + "'";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json(results[0]);
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Config failed to return' });
		}
	});
});

//PUT /rules Updates rule based on id
restapi.put('/config/:config_name', function(req, res){
	console.log("PUT /config/:config_name");
	var sql = "UPDATE config SET value = '" + req.body.value +"' WHERE name = '" + req.params.config_name + "'";
	console.log(sql);

	connection.query(sql, function (error, results, fields) {
		if(error == null) {
			console.log("Success");
			res.json({ message: 'Config updated' });
		}
		else {
			console.log("Error" + error);
			res.json({ message: 'Config failed to update' });
		}
	});
});

restapi.listen(3000);

console.log("API started on http://localhost:3000");
