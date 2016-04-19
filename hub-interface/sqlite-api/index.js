var sqlite3 = require('sqlite3').verbose();
var db = new sqlite3.Database('test6.db');
var bodyParser = require('body-parser')

db.serialize(function() {
    //db.run("CREATE TABLE rule( rule_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, rule_name TEXT NOT NULL, rule_description TEXT, rule_active BOOLEAN NOT NULL)");

});



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


function jsonConcat(o1, o2) {
 for (var key in o2) {
  o1[key] = o2[key];
 }
 return o1;
}

//POST /rules Creates a new rule
restapi.post('/rules', function(req, res){
	console.log(req.params);
	console.log(req.body);
	console.log("POST /rules Creates a new rule");
	var sql = "INSERT INTO rule (rule_name,rule_description,rule_active) VALUES ('" + req.body.rule_name + "','" + req.body.rule_description + "'," + req.body.rule_active + ")";
	console.log(sql);
	//COULD RETURN NEW RULE OR JUST ID?
	db.get(sql, function(err, data){
        if (!err) {
        	db.get("SELECT last_insert_rowid()", function(err, data){
		        if (!err) {
		        	console.log(data);
		        	var rule = req.body;
		        	rule.rule_id = data['last_insert_rowid()'];
		        	console.log(rule);
		        	res.json(rule);
		        	console.log("Rule inserted");
		        }
		        else {
					console.log("Rule failed to insert");
					res.json({ message: 'Rule failed to insert' });
				}
		        });
        }
        else {
			console.log("Rule failed to insert");
			res.json({ message: 'Rule failed to insert' });
		}
    });
});

//GET /rules Returns all rules
restapi.get('/rules', function(req, res){
	console.log("GET /rules Returns all rules");
	var sql = "SELECT * FROM rule";
	console.log(sql);

	db.all(sql, function(err, rows){
		if (!err) {
            console.log("Rules returned");
            res.json(rows);
        }
        else {
			console.log("Rules failed to return");
			res.json({ message: 'Rules failed to return' });
		}
    });
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/rules/:rule_id', function(req, res){
	console.log("GET /rules/:rule_id Returns rule based on id:?",req.params.rule_id);
	var sql = "SELECT * FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	db.get(sql, function(err, row){
		if (!err) {
            console.log("Rule returned");
            res.json(row);
        }
        else {
			console.log("Rule failed to return");
			res.json({ message: 'Rule failed to return' });
		}
    });
});

//PUT /rules Updates rule based on id
restapi.put('/rules/:rule_id', function(req, res){
	console.log("PUT /rules Updates rule based on id:?",req.params.rule_id);
	var sql = "UPDATE rule SET rule_name = '" + req.body.rule_name + "', rule_description = '" + req.body.rule_description + "', rule_active = " + req.body.rule_active +" WHERE rule_id=" + req.params.rule_id;
	console.log(sql);
	db.run(sql, function(err, row){
        if (!err){
            console.log("Rule updated");
            res.json({ message: 'Rule updated' });
        } else {
			console.log("Rule failed to update");
			res.json({ message: 'Rule failed to update' });
		}
    });
});

//DELETE /rules deletes rule based on id
restapi.delete('/rules/:rule_id', function(req, res){
	console.log("DELETE /rules deletes rule based on id:?",req.params.rule_id);
	var sql = "DELETE FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);
	db.run(sql, function(err, row){
        if (!err){
            console.log("Rule deleted");
            res.json({ message: 'Rule deleted' });
        } else {
		console.log("Rule failed to delete");
		res.json({ message: 'Rule failed to delete' });
		}
    });
});




//



//POST /rules Creates a new rule
restapi.post('/rules/:rule_id/constraints', function(req, res){
	console.log("POST /rules Creates a new rule");
	var sql = "INSERT INTO rule_constraint (rule_id,field_id,constraint_operator,constraint_value) VALUES (" + req.query.rule_id + ",'" + req.query.field_id + "','" + req.query.constraint_operator + "'," + req.query.constraint_value + ")";
	console.log(sql);
	//COULD RETURN NEW RULE OR JUST ID?
	db.run(sql, function(err, data){
        if (!err) {
            console.log("Rule inserted");
            res.json({ message: 'Rule inserted' });
        }
        else {
			console.log("Rule failed to insert");
			res.json({ message: 'Rule failed to insert' });
		}
    });
});

//GET /rules/:rule_id/constraints Returns all contraints for rule id
restapi.get('/rules/:rule_id/constraints', function(req, res){
	console.log("GET /rules/:rule_id/constraints Returns all contraints for rule id"+ req.params.rule_id);
	var sql = "SELECT * FROM rule_constraint WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	db.all(sql, function(err, rows){
		if (!err) {
            console.log("Rules returned");
            res.json(rows);
        }
        else {
			console.log("Rules failed to return");
			res.json({ message: 'Rules failed to return' });
		}
    });
});

//GET /rules/:rule_id Returns rule based on id
restapi.get('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("GET /rules/:rule_id Returns rule based on id:?",req.params.rule_id);
	var sql = "SELECT * FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);

	db.get(sql, function(err, row){
		if (!err) {
            console.log("Rule returned");
            res.json(row);
        }
        else {
			console.log("Rule failed to return");
			res.json({ message: 'Rule failed to return' });
		}
    });
});

//PUT /rules Updates rule based on id
restapi.put('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("PUT /rules Updates rule based on id:?",req.params.rule_id);
	var sql = "UPDATE rule SET rule_name = '" + req.body.rule_name + "', rule_description = '" + req.body.rule_description + "', rule_active = " + req.body.rule_active +" WHERE rule_id=" + req.params.rule_id;
	console.log(sql);
	db.run(sql, function(err, row){
        if (!err){
            console.log("Rule updated");
            res.json({ message: 'Rule updated' });
        } else {
			console.log("Rule failed to update");
			res.json({ message: 'Rule failed to update' });
		}
    });
});

//DELETE /rules deletes rule based on id
restapi.delete('/rules/:rule_id/constraints/:constraint_id', function(req, res){
	console.log("DELETE /rules deletes rule based on id:?",req.params.rule_id);
	var sql = "DELETE FROM rule WHERE rule_id=" + req.params.rule_id;
	console.log(sql);
	db.run(sql, function(err, row){
        if (!err){
            console.log("Rule deleted");
            res.json({ message: 'Rule deleted' });
        } else {
		console.log("Rule failed to delete");
		res.json({ message: 'Rule failed to delete' });
		}
    });
});



restapi.listen(3000);

console.log("API started on http://localhost:3000");
