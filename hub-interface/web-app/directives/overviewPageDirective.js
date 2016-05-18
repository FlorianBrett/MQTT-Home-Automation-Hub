(function(){
	var app = angular.module('overview-page', []);
	app.directive('overviewPage', function(){
		return {
			restrict: 'E',
			templateUrl: 'views/overviewPage.html',
			controller: [ '$scope' ,'$http', function($scope,$http){
				$scope.ruleCount = -1;
					$http.get('http://192.168.1.30:3000/countRules').success(function(data){
						$scope.ruleCount = data["count(*)"];
					});
				$scope.ruleActiveCount = -1;
				    $http.get('http://192.168.1.30:3000/countActiveRules').success(function(data){
						$scope.ruleActiveCount = data["count(*)"];
					});
				$scope.deviceCount = -1;
					$http.get('http://192.168.1.30:3000/countDevices').success(function(data){
						$scope.deviceCount = data["count(*)"];
					});
				$scope.fieldCount = -1;
				    $http.get('http://192.168.1.30:3000/countFields').success(function(data){
						$scope.fieldCount = data["count(*)"];
					});
				$scope.dayRuleHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-rule-history/1').success(function(data){
						$scope.dayRuleHistoryCount = data["count(*)"];
					});
				$scope.weekRuleHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-rule-history/7').success(function(data){
						$scope.weekRuleHistoryCount = data["count(*)"];
					});
				$scope.monthRuleHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-rule-history/30').success(function(data){
						$scope.monthRuleHistoryCount = data["count(*)"];
					});

				$scope.dayStateHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-state-history/1').success(function(data){
						$scope.dayStateHistoryCount = data["count(*)"];
					});
				$scope.weekStateHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-state-history/7').success(function(data){
						$scope.weekStateHistoryCount = data["count(*)"];
					});
				$scope.monthStateHistoryCount = -1;
				    $http.get('http://192.168.1.30:3000/count-state-history/30').success(function(data){
						$scope.monthStateHistoryCount = data["count(*)"];
					});

				$scope.stateHistory = [];
				    $http.get('http://192.168.1.30:3000/state-history/10').success(function(data){
						$scope.stateHistory = data;
					});
				$scope.stateHistory = [];
				    $http.get('http://192.168.1.30:3000/rule-history/10').success(function(data){
						$scope.ruleHistory = data;
					});

				this.update = function () {
					$http.get('http://192.168.1.30:3000/countRules').success(function(data){
						$scope.ruleCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/countActiveRules').success(function(data){
						$scope.ruleActiveCount = data["count(*)"];
					});
					$http.get('http://192.168.1.30:3000/countDevices').success(function(data){
						$scope.deviceCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/countFields').success(function(data){
						$scope.fieldCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-rule-history/1').success(function(data){
						$scope.dayRuleHistoryCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-rule-history/7').success(function(data){
						$scope.weekRuleHistoryCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-rule-history/30').success(function(data){
						$scope.monthRuleHistoryCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-state-history/1').success(function(data){
						$scope.dayStateHistoryCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-state-history/7').success(function(data){
						$scope.weekStateHistoryCount = data["count(*)"];
					});
				    $http.get('http://192.168.1.30:3000/count-state-history/30').success(function(data){
						$scope.monthStateHistoryCount = data["count(*)"];
					});

				    $http.get('http://192.168.1.30:3000/state-history/10').success(function(data){
						$scope.stateHistory = data;
					});
				    $http.get('http://192.168.1.30:3000/rule-history/10').success(function(data){
						$scope.ruleHistory = data;
					});
				};

				setInterval(this.update,1000);
				}],
			controllerAs: 'overviewCtrl'
		};
	});
})();