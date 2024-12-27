<!-------------------------------------------------------------->
function setSequenceIndex(SequenceIndex){
	json = JSON.stringify(SequenceIndex)
	var xhr = new XMLHttpRequest();
	xhr.open("POST", "/api/SequenceIndex", true);
	xhr.setRequestHeader("Content-type", "application/json");
	xhr.send(json);
}

function getSequenceIndex(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		var json;
		json = JSON.parse(this.response)
		callback(json[0])
	};
	xhr.open("GET", "/api/SequenceIndex", true);
	xhr.send();
}

<!-------------------------------------------------------------->
function setSequenceTable(json,callback=null){
	var xhr = new XMLHttpRequest();
	xhr.onload = function(){	
		if(xhr.status == 200){
			modal_1.click()
		}else{
			modal_2.click()
		}
		callback(this)	
	};
	xhr.open("POST", "/api/SequenceTable", true);
	xhr.send(json);
}

function getSequenceTable(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("GET", "/api/SequenceTable", true);
	xhr.send();	
}
<!-------------------------------------------------------------->
function setStartSequence(Running){
	var json;
	json = JSON.stringify(Running)
	var xhr = new XMLHttpRequest();
	xhr.open("POST", "/api/Start", true);
	xhr.send(json);
}

function getStartSequence(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response)[0])
	};
	xhr.open("GET", "/api/Start", true);
	xhr.send();
}
<!-------------------------------------------------------------->
function getPingTest(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("GET", "/api/PingTest", true);
	xhr.send();
}
<!-------------------------------------------------------------->
function getSettings(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("GET", "/api/Settings", true);
	xhr.send();
}

function setSettings(json,callback=null){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("POST", "/api/Settings", true);
	xhr.send(json);
}
<!-------------------------------------------------------------->
function getNodeList(callback){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("GET", "/api/NodeList", true);
	xhr.send();
}

function setNodeList(json,callback=null){
	var xhr = new XMLHttpRequest();
	xhr.onload = function() {
		callback(JSON.parse(this.response))
	};
	xhr.open("POST", "/api/NodeList", true);
	xhr.send(json);
}