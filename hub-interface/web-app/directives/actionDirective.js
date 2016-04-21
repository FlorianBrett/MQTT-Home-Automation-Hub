(function(){
	var app = angular.module('action', [ ]);

	app.directive('actionRow', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/actionRow.html',
			controller: [ '$http', function($http){
				this.edit = 0;
				this.editAction = {};
				//naviagation
				this.setEdit = function (inEdit) {
					this.edit = inEdit;
				};
				this.isEdit = function (inEdit) {
					return this.edit === inEdit;
				};
				//edit action
				this.initEdit = function (action) {
					this.editAction.rule_id = action.rule_id;
					this.editAction.action_id = action.action_id;
					this.editAction.field_id = action.field_id;
					this.editAction.action_operator = action.action_operator;
					this.editAction.action_value = action.action_value;
					this.setEdit(1);
				};
				this.submitEdit = function (action) {
					if (this.editAction.field_id != null && this.editAction.action_operator != null && this.editAction.action_value != null)
					{
						action.rule_id = this.editAction.rule_id ;
						action.action_id = this.editAction.action_id ;
						action.field_id = this.editAction.field_id;
						action.action_operator = this.editAction.action_operator;
						action.action_value = this.editAction.action_value;
						console.log(action);
						$http.put('http:/localhost:3000/rules' + '/' + action.rule_id + '/actions/' + action.action_id , action)
						this.setEdit(0);
					}
					else
					{
						console.log("invalidEdit");
					}
				};

				//delete action
				this.deleteAction = function (action,actions) {
					console.log(action);
					$http.delete('http:/localhost:3000/rules' + '/' + action.rule_id + '/actions/' + action.action_id);
					index = actions.findIndex(x => x.action_id==action.action_id);
					console.log(index);
					actions.splice (index, 1);
				};

			}],
			controllerAs: 'actionCtrl'
		};
	});


	app.directive('actionRowCreate', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/actionRowCreate.html',
			controller: [ '$http', function($http){
				this.edit = 0;
				this.editAction = {};
				//naviagation
				this.setEdit = function (inEdit) {
					this.edit = inEdit;
				};
				this.isEdit = function (inEdit) {
					return this.edit === inEdit;
				};
			//create
				this.initCreate = function () {
					this.editAction.rule_id = 1;
					this.editAction.action_id = null;
					this.editAction.field_id = null;
					this.editAction.action_operator = null;
					this.editAction.action_value = null;
					this.setEdit(1);
				};
				this.createAction = function (rule,action,actions) {
					if (this.editAction.field_id != null && this.editAction.action_operator != null && this.editAction.action_value != null)
					{
						var action = {};
						action.rule_id = rule.rule_id ;
						action.action_id = this.editAction.action_id ;
						action.field_id = this.editAction.field_id;
						action.action_operator = this.editAction.action_operator;
						action.action_value = this.editAction.action_value;
						console.log(action);
						$http.post('http:/localhost:3000/rules' + '/' + action.rule_id + '/actions' , action).success(function(data){
							console.log(data);
							actions.push(data);
						});

						this.setEdit(0);
					}
				};

			}],
			controllerAs: 'actionCtrl'
		};
	});

})();