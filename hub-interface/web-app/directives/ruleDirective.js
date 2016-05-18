(function(){
	var app = angular.module('rule', ['constraint','action' ]);

	app.controller('actionListCtrl', [ '$scope' ,'$http', function($scope,$http){

	$scope.actions = [];

		this.initActionList = function (rule) {
			var apiURL = 'http://192.168.1.30:3000/rules/' + rule.rule_id + '/actions'
			$http.get(apiURL).success(function(data){
				$scope.actions = data;
				console.log($scope.actions);
			});
		};

	}]);

	app.controller('constraintListCtrl', [ '$scope' ,'$http', function($scope,$http){

	$scope.constraints = [];

		this.initConstraintList = function (rule) {
			var apiURL = 'http://192.168.1.30:3000/rules/' + rule.rule_id + '/constraints'
			$http.get(apiURL).success(function(data){
				$scope.constraints = data;
				console.log($scope.constraints);
			});
		};

	}]);
	app.directive('rulePanel', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/rulePanel.html',
			controller: [ '$http', function($http){
				this.expand = 0;
				this.edit = 0;
				this.editRule = {};

				this.setEdit = function (inEdit) {
					this.edit = inEdit;
				};
				this.isEdit = function (inEdit) {
					return this.edit === inEdit;
				};

				this.setExpand = function (inExpand) {
					this.expand = inExpand;
				};
				this.isExpand = function (inExpand) {
					return this.expand === inExpand;
				};


				this.deleteRule = function (rule,rules) {
					$http.delete('http://192.168.1.30:3000/rules' + '/' + rule.rule_id);
					index = rules.findIndex(x => x.rule_id==rule.rule_id);
					rules.splice (index, 1);
				};

				this.initEdit = function (rule) {
					this.editRule.rule_id = rule.rule_id;
					this.editRule.rule_name = rule.rule_name;
					this.editRule.rule_description = rule.rule_description;
					this.editRule.rule_active = rule.rule_active;
					this.setEdit(1);
				};
				this.submitEdit = function (rule) {
					if (this.editRule.rule_name != null && this.editRule.rule_active != null)
					{
						rule.rule_id = this.editRule.rule_id;
						rule.rule_name = this.editRule.rule_name;
						rule.rule_description = this.editRule.rule_description;
						rule.rule_active = this.editRule.rule_active;
						$http.put('http://192.168.1.30:3000/rules' + '/' + rule.rule_id, rule)
						this.setEdit(0);
					}
				};

			}],
			controllerAs: 'ruleCtrl'
		};
	});


	app.directive('rulePanelCreate', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/rulePanelCreate.html',
			controller: [ '$http', function($http){
				this.expand = 0;
				this.edit = 0;
				this.editRule = {};

				this.setEdit = function (inEdit) {
					this.edit = inEdit;
				};
				this.isEdit = function (inEdit) {
					return this.edit === inEdit;
				};
				this.initCreate = function () {
					this.editRule.rule_name = "Rule name";
					this.editRule.rule_description = "Rule description";
					this.editRule.rule_active = 1;
					this.setEdit(1);
				};
				this.submitCreate = function (rule,rules) {
					if (this.editRule.rule_name != null  && this.editRule.rule_active != null)
					{
						this.editRule.rule_name;
						this.editRule.rule_description;
						this.editRule.rule_active;
						console.log(this.editRule);
						$http.post('http://192.168.1.30:3000/rules', this.editRule).success(function(data){
							console.log(data);
							rules.push(data);
						});
						this.setEdit(0);
					}
				};

			}],
			controllerAs: 'ruleCtrl'
		};
	});

})();