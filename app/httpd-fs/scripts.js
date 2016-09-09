// JavaScript Document
function getHTTPRequestObject() {
	  var xmlHttpRequest;
	  /*@cc_on
	  @if (@_jscript_version >= 5)
	  try {
	    xmlHttpRequest = new ActiveXObject("Msxml2.XMLHTTP");
	  } catch (exception1) {
	    try {
	      xmlHttpRequest = new ActiveXObject("Microsoft.XMLHTTP");
	    } catch (exception2) {
	      xmlHttpRequest = false;
	    }
	  }
	  @else
	    xmlhttpRequest = false;
	  @end @*/
	 
	  if (!xmlHttpRequest && typeof XMLHttpRequest != 'undefined') {
	    try {
	      xmlHttpRequest = new XMLHttpRequest();
	    } catch (exception) {
	      xmlHttpRequest = false;
	    }
	  }
	  return xmlHttpRequest;
 }
	 
var httpRequester = getHTTPRequestObject();

function sendPostQuery(url, poststr, process){
	
	httpRequester.open('POST', url, true);
//	httpRequester.onreadystatechange = process;
//	httpRequester.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
//	httpRequester.setRequestHeader("Content-length", poststr.length);
//	httpRequester.setRequestHeader("Connection", "close");
	httpRequester.send(poststr);
}

function sendGetQuery(url, process){
	
	httpRequester.open('GET', url, true);
	httpRequester.onreadystatechange = process;
	httpRequester.send(null);
}

function processRequest(){
	var m_form = document.getElementById('setting_form');
	if(m_form==null)return;
	
	if ( httpRequester.readyState == 4 ) { 
 	    var values = httpRequester.responseText.split(",");
 	   if(values.length < 2)return false;
 	   m_form.di1.value = values[0];
 	   m_form.di2.value = values[1];
	}
}

function sendRequest(){
	sendGetQuery("param0", processRequest);
}


function showValue()
{
	var bigNumber = Number.MAX_VALUE * 2
	if (bigNumber == Number.POSITIVE_INFINITY) {
		alert("Value of bigNumber : " + bigNumber );
	}
}