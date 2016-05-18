(function(){
	var app = angular.module('live-field-page', []);
	app.directive('liveFieldPage', function(){
		return {
			restrict: 'E',
			templateUrl: 'views/liveFieldPage.html',
			controller: [ '$scope' ,'$http', function($scope,$http){
				$scope.fields = [];
				$http.get('http://192.168.1.30:3000/fields').success(function(data){
					$scope.fields = data;
				});

				client = new Paho.MQTT.Client("192.168.1.30", 9001, "WebApp"+ Math.random());
				client.onConnectionLost = onConnectionLost;
				client.onMessageArrived = onMessageArrived;
				client.connect({onSuccess:onConnect});

				function onConnect() {
				  // Once a connection has been made, make a subscription and send a message.
				  console.log("onConnect");
				  client.subscribe("/#");
				  message = new Paho.MQTT.Message("Hello");
				  message.destinationName = "/World";
				  client.send(message); 
				};
				function onConnectionLost(responseObject) {
				  if (responseObject.errorCode !== 0)
					console.log("onConnectionLost:"+responseObject.errorMessage);
				};

				$scope.fieldChange = function (field) {
					if(field.last_field_value == null || field.last_field_value != field.field_value) {
						console.log("click");
					  message = new Paho.MQTT.Message(field.field_value.toString());
					  message.destinationName = "/" + field.device_id + "/" + field.field_id;
					  client.send(message); 
					  field.last_field_value = field.field_value;
					}
				};
				function onMessageArrived(message) {
					console.log("onMessageArrived:"+message.payloadString);
					var res = message.destinationName.split("/");
					console.log(res);

				    for(var i=0; i<$scope.fields.length; i++) {
				    	if ($scope.fields[i].device_id == res[1]) {
				    		if ($scope.fields[i].field_id == res[2]) {
				    			console.log("before" + $scope.fields[i].field_value);
				    			console.log($scope.fields[i].field_name);
				    			$scope.$apply(function() {
				    				$scope.fields[i].last_field_value = parseInt(message.payloadString, 10);
				    				$scope.fields[i].field_value = parseInt(message.payloadString, 10);
				    			});
				    			console.log("after" + $scope.fields[i].field_value);
				    		}
				    	}
					}

				};	

			}],
			controllerAs: 'liveFieldCtrl'
		};
	});
})();