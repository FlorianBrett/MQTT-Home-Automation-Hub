(function(){
var app = angular.module('ruleApp', ['rule-editor-page','overview-page','settings-page','live-field-page','frapontillo.bootstrap-switch','nya.bootstrap.select','rzModule','ui.router']);

app.config(function($stateProvider, $urlRouterProvider){
      
      // For any unmatched url, send to /route1
      $urlRouterProvider.otherwise("/overview")
      
      $stateProvider
        .state('overview', {
            url: "/overview",
            template: "<overview-page>"
        })
        .state('ruleEditor', {
            url: "/rule-editor",
            template: "<rule-editor-page>"
        })
        .state('settings', {
            url: "/settings",
            template: "<settings-page>"
        })
        .state('live', {
            url: "/live",
            template: "<live-field-page>"
        })

    });



})();
