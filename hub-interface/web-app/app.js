(function(){
var app = angular.module('ruleApp', []);

app.controller('ruleListCtrl', [ '$scope' ,'$http', function($scope,$http){

$scope.rules = [];
	$http.get('http:/localhost:3000/rules').success(function(data){
		$scope.rules = data;
	});


}]);

app.controller('constraintListCtrl', [ '$scope' ,'$http', function($scope,$http){

$scope.constraints = [];
	var apiURL = 'http:/localhost:3000/rules/' + 3 + '/constraints'
	$http.get(apiURL).success(function(data){
		$scope.constraints = data;
		console.log($scope.constraints);
	});

}]);

app.controller('constraintCtrl', [ '$http', function($http){
	this.edit = 0;
	this.editConstraint = {};
	//naviagation
	this.setEdit = function (inEdit) {
		this.edit = inEdit;
	};
	this.isEdit = function (inEdit) {
		return this.edit === inEdit;
	};
	//edit constraint
	this.initEdit = function (constraint) {
		this.editConstraint.rule_id = constraint.rule_id;
		this.editConstraint.constraint_id = constraint.constraint_id;
		this.editConstraint.field_id = constraint.field_id;
		this.editConstraint.constraint_operator = constraint.constraint_operator;
		this.editConstraint.constraint_value = constraint.constraint_value;
		this.setEdit(1);
	};
	this.submitEdit = function (constraint) {
		constraint.rule_id = this.editConstraint.rule_id ;
		constraint.constraint_id = this.editConstraint.constraint_id ;
		constraint.field_id = this.editConstraint.field_id;
		constraint.constraint_operator = this.editConstraint.constraint_operator;
		constraint.constraint_value = this.editConstraint.constraint_value;
		console.log(constraint);
		$http.put('http:/localhost:3000/rules' + '/' + constraint.rule_id + '/constraints/' + constraint.constraint_id , constraint)
		this.setEdit(0);
	};
//create
	this.initCreate = function () {
		this.editConstraint.rule_id = 1;
		this.editConstraint.constraint_id = null;
		this.editConstraint.field_id = null;
		this.editConstraint.constraint_operator = null;
		this.editConstraint.constraint_value = null;
		this.setEdit(1);
	};
	this.createConstraint = function (rule,constraint,constraints) {
		var constraint = {};
		constraint.rule_id = rule.rule_id ;
		constraint.constraint_id = this.editConstraint.constraint_id ;
		constraint.field_id = this.editConstraint.field_id;
		constraint.constraint_operator = this.editConstraint.constraint_operator;
		constraint.constraint_value = this.editConstraint.constraint_value;
		console.log(constraint);
		$http.post('http:/localhost:3000/rules' + '/' + constraint.rule_id + '/constraints' , constraint).success(function(data){
			console.log(data);
			constraints.push(data);
		});

		this.setEdit(0);
	};

	//delete constraint
	this.deleteConstraint = function (constraint,constraints) {
		console.log(constraint);
		$http.delete('http:/localhost:3000/rules' + '/' + constraint.rule_id + '/constraints/' + constraint.constraint_id);
		index = constraints.findIndex(x => x.constraint_id==constraint.constraint_id);
		console.log(index);
		constraints.splice (index, 1);
	};


}]);

app.controller('ruleNavCtrl', function () {
	this.expand = 0;
	this.mode = 1;
	this.setMode = function (inMode) {
		this.mode = inMode;
	};
	this.isMode = function (inMode) {
		return this.mode === inMode;
	};

	this.setExpand = function (inExpand) {
		this.expand = inExpand;
	};
	this.isExpand = function (inExpand) {
		return this.expand === inExpand;
	};

});
app.controller('ruleEditCtrl',  [ '$http', function($http){
	this.editRule = {};
	this.initEdit = function (rule) {
		this.editRule.rule_id = rule.rule_id;
		this.editRule.rule_name = rule.rule_name;
		this.editRule.rule_description = rule.rule_description;
		this.editRule.rule_active = rule.rule_active;
	};
	this.submitEdit = function (rule) {
		rule.rule_id = this.editRule.rule_id;
		rule.rule_name = this.editRule.rule_name;
		rule.rule_description = this.editRule.rule_description;
		rule.rule_active = this.editRule.rule_active;
		console.log(rule);
		$http.put('http:/localhost:3000/rules' + '/' + rule.rule_id, rule)
	};

}]);

app.controller('ruleDeleteCtrl',  [ '$http', function($http){

	this.deleteRule = function (rule,rules) {
		console.log(rule);
		$http.delete('http:/localhost:3000/rules' + '/' + rule.rule_id);
		index = rules.findIndex(x => x.rule_id==rule.rule_id);
		console.log(index);
		rules.splice (index, 1);
	};

}]);

app.controller('ruleCreateCtrl',  [ '$http', function($http){
	this.editRule = {};

	this.initEdit = function () {
		this.editRule.rule_active = 1;
		this.editRule.rule_name = null;
		this.editRule.rule_description = null;
		this.editRule.rule_active = 1;
	};
	this.submitEdit = function (rule,rules) {
		//rule.rule_id = this.editRule.rule_id;
		this.editRule.rule_name;
		this.editRule.rule_description;
		this.editRule.rule_active;
		console.log(this.editRule);
		$http.post('http:/localhost:3000/rules', this.editRule).success(function(data){
		console.log(data);
		rules.push(data);
		});

	};
}]);


})();
