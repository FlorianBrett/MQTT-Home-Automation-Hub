(function(){
var app = angular.module('ruleApp', []);

app.controller('ruleListCtrl', [ '$scope' ,'$http', function($scope,$http){

$scope.rules = [];
	$http.get('http:/localhost:3000/rules').success(function(data){
		$scope.rules = data;
	});

	this.outlog = function () {
		console.log($scope.rules);
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
