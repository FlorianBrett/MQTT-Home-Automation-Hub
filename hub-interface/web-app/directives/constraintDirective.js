(function(){
	var app = angular.module('constraint', [ ]);

	app.directive('constraintRow', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/constraintRow.html',
			controller: [ '$http', function($http){
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
					if (this.editConstraint.field_id != null && this.editConstraint.constraint_operator != null && this.editConstraint.constraint_value != null)
					{
						constraint.rule_id = this.editConstraint.rule_id ;
						constraint.constraint_id = this.editConstraint.constraint_id ;
						constraint.field_id = this.editConstraint.field_id;
						constraint.constraint_operator = this.editConstraint.constraint_operator;
						constraint.constraint_value = this.editConstraint.constraint_value;
						console.log(constraint);
						$http.put('http://192.168.1.30:3000/rules' + '/' + constraint.rule_id + '/constraints/' + constraint.constraint_id , constraint)
						this.setEdit(0);
					}
					else
					{
						console.log("invalidEdit");
					}
				};

				//delete constraint
				this.deleteConstraint = function (constraint,constraints) {
					console.log(constraint);
					$http.delete('http://192.168.1.30:3000/rules' + '/' + constraint.rule_id + '/constraints/' + constraint.constraint_id);
					index = constraints.findIndex(x => x.constraint_id==constraint.constraint_id);
					console.log(index);
					constraints.splice (index, 1);
				};

			}],
			controllerAs: 'constraintCtrl'
		};
	});


	app.directive('constraintRowCreate', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/constraintRowCreate.html',
			controller: [ '$http', function($http){
				this.edit = 0;
				this.editConstraint = {};
				//naviagation
				this.setEdit = function (inEdit) {
					this.edit = inEdit;
				};
				this.isEdit = function (inEdit) {
					return this.edit === inEdit;
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
					if (this.editConstraint.field_id != null && this.editConstraint.constraint_operator != null && this.editConstraint.constraint_value != null)
					{
						var constraint = {};
						constraint.rule_id = rule.rule_id ;
						constraint.constraint_id = this.editConstraint.constraint_id ;
						constraint.field_id = this.editConstraint.field_id;
						constraint.constraint_operator = this.editConstraint.constraint_operator;
						constraint.constraint_value = this.editConstraint.constraint_value;
						console.log(constraint);
						$http.post('http://192.168.1.30:3000/rules' + '/' + constraint.rule_id + '/constraints' , constraint).success(function(data){
							console.log(data);
							constraints.push(data);
						});

						this.setEdit(0);
					}
				};

			}],
			controllerAs: 'constraintCtrl'
		};
	});

})();