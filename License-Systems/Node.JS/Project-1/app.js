/*
 * Author  : SwenenzY
 * Website : https://swenenzy.com
 * Repo    : https://github.com/SwenenzY/Hwid-System/
 */
const Http = require('http');
const Express = require('express');
const Fetch = require('node-superfetch');
const App = Express();

console.log("mrb"); 

// Heroku Glitch etc. App.listen(process.env.PORT);
App.listen(80); // 80 http for local  

App.get("/", (request, response) => {
	
  response.send( "Hoşgeldiniz efendim.." );
  
} ); 
 
/*
 * license for this project is manually defined
 * you can do anything you want instead of key
 * const Data = { Keys : [ "CPUID" , "CPUID2" ] };
 * const Data = { Keys : [ "CryptoID" , "CryptoID2", "CryptoID3" ] };
 */
App.get("/list", (Request, Response) => {

	const Data = { Keys : [ "key1","key2" ] };
	Response.json( Data );
	console.error( "Request!" );
	
} ); 
 
