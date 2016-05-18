(function(){
	var app = angular.module('settings-page', []);
	app.directive('settingsPage', function(){
		return {
			restrict: 'E',
			templateUrl: 'views/settingsPage.html',
			controller: [ '$scope' ,'$http', function($scope,$http){
				$scope.mqttAddress = {};
				$http.get('http://192.168.1.30:3000/config/mqtt_address').success(function(data){
					$scope.mqttAddress = data;
				});

				$scope.mqttUsername = {};
				$http.get('http://192.168.1.30:3000/config/mqtt_username').success(function(data){
					$scope.mqttUsername = data;
				});

				$scope.mqttPassword = {};
				$http.get('http://192.168.1.30:3000/config/mqtt_password').success(function(data){
					$scope.mqttPassword = data;
				});

				$scope.mqttClientId = {};
				$http.get('http://192.168.1.30:3000/config/mqtt_client_id').success(function(data){
					$scope.mqttClientId = data;
				});


				$scope.loggingGlobal = -1;
				$http.get('http://192.168.1.30:3000/config/global_logging_level').success(function(data){
					$scope.loggingGlobal = parseInt(data.value, 10);
				});

				$scope.loggingDB = -1;
				$http.get('http://192.168.1.30:3000/config/db_logging_level').success(function(data){
					$scope.loggingDB = parseInt(data.value, 10);
				});

				$scope.loggingRule = -1;
				$http.get('http://192.168.1.30:3000/config/rule_logging_level').success(function(data){
					$scope.loggingRule = parseInt(data.value, 10);
				});

				$scope.loggingTimer = -1;
				$http.get('http://192.168.1.30:3000/config/timer_logging_level').success(function(data){
					$scope.loggingTimer = parseInt(data.value, 10);
				});

				$scope.loggingMQTT = -1;;
				$http.get('http://192.168.1.30:3000/config/mqtt_logging_level').success(function(data){
					$scope.loggingMQTT = parseInt(data.value, 10);
				});


				$scope.stateHistory = 0;
				$http.get('http://192.168.1.30:3000/config/state_history').success(function(data){
					$scope.stateHistory = parseInt(data.value, 10);
				});

				$scope.ruleHistory = 0;
				//$http.get('http://192.168.1.30:3000/config/rule_history').success(function(data){
				//	$scope.ruleHistory = parseInt(data.value, 10);
				//});

				this.submitMQTT = function () {
					$http.put('http://192.168.1.30:3000/config/mqtt_address', $scope.mqttAddress);
					$http.put('http://192.168.1.30:3000/config/mqtt_username', $scope.mqttUsername);
					$http.put('http://192.168.1.30:3000/config/mqtt_password', $scope.mqttPassword);
					$http.put('http://192.168.1.30:3000/config/mqtt_client_id', $scope.mqttClientId);
				};

				this.submitLogging = function () {
					$http.put('http://192.168.1.30:3000/config/global_logging_level', {value: $scope.loggingGlobal});
					$http.put('http://192.168.1.30:3000/config/db_logging_level', {value: $scope.loggingDB});
					$http.put('http://192.168.1.30:3000/config/rule_logging_level', {value: $scope.loggingRule});
					$http.put('http://192.168.1.30:3000/config/timer_logging_level', {value: $scope.loggingTimer});
					$http.put('http://192.168.1.30:3000/config/mqtt_logging_level', {value: $scope.loggingMQTT});

				};

				this.submitHistory = function () {

					$http.put('http://192.168.1.30:3000/config/state_history', {value: $scope.stateHistory});
					//$http.put('http://192.168.1.30:3000/config/rule_history', {value: $scope.ruleHistory});

				};

				$scope.sliderOptions = {
					floor: 0,
					ceil: 8,
					stepsArray:['Trace', 'Debug', 'Informational (default)', 'Notice', 'Warning', 'Error', 'Critical', 'Alert', 'Emergency']
				}


			}],
			controllerAs: 'settingsCtrl'
		};
	});
})();