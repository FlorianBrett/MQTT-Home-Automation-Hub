(function(){
var app = angular.module('ruleApp', ['nya.bootstrap.select','rule','frapontillo.bootstrap-switch']);

	app.directive('ruleList', function(){
		return {
			restrict: 'A',
			templateUrl: 'views/ruleListPage.html',
			controller: [ '$scope' ,'$http', function($scope,$http){
				$scope.rules = [];
					$http.get('http:/localhost:3000/rules').success(function(data){
						$scope.rules = data;
					});
				$scope.fields = [];
				    $http.get('http:/localhost:3000/fieldIDName').success(function(data){
						$scope.fields = data;
					});
				$scope.fieldsCanSet = [];
				    $http.get('http:/localhost:3000/fieldIDNameCanSet').success(function(data){
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
			controllerAs: 'ruleListCtrl'
		};
	});




})();
