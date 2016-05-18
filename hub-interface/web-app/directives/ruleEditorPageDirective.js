(function(){
	var app = angular.module('rule-editor-page', ['rule']);

	app.directive('ruleEditorPage', function(){
		return {
			restrict: 'E',
			templateUrl: 'views/ruleEditorPage.html',
			controller: [ '$scope' ,'$http', function($scope,$http){
				$scope.rules = [];
					$http.get('http://192.168.1.30:3000/rules').success(function(data){
						$scope.rules = data;
					});
				$scope.fields = [];
				    $http.get('http://192.168.1.30:3000/fieldIDName').success(function(data){
						$scope.fields = data;
					});
				$scope.fieldsCanSet = [];
				    $http.get('http://192.168.1.30:3000/fieldIDNameCanSet').success(function(data){
						$scope.fieldsCanSet = data;
					});
				$scope.constraintOperators = [
				      {id: '>', name: 'Greater than'},
				      {id: '<', name: 'Less than'},
				      {id: '=', name: 'Equal to'}
				    ];
				$scope.actionOperators = [
				      {id: '=', name: 'Equal to'},
				      {id: '+', name: 'Add'},
				      {id: '-', name: 'Minus'},
				      {id: 'toggle', name: 'Toggle'}
				    ];

				}],
			controllerAs: 'ruleEditorCtrl'
		};
	});

})();