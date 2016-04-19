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
	this.mode = 1;
	this.setMode = function (inMode) {
		this.mode = inMode;
	};
	this.isMode = function (inMode) {
		return this.mode === inMode;
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
	this.isMode = function (inMode) {
		return this.mode === inMode;
	};
}]);

  var newRules = [
    {'rule_id': '1',
     'rule_name': 'Rule1'},
    {'rule_id': '2',
     'rule_name': 'Rule2'},
    {'rule_id': '3',
     'rule_name': 'Rule3'}
  ];

})();
