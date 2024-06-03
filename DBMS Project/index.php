<?php
if ((session_status()) == PHP_SESSION_NONE) {
	session_start();
    if(isset($_SESSION['linkto'])){
		$nextlink = $_SESSION["linkto"];
		header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/".$nextlink);
	}
	else{
		session_unset(); 
		session_destroy();
	}
}
?>
<html>
<head>
<style>
#loginform1{  
	border:3px solid green; 
	border-radius: 8px; 
	width: 50%; 
	margin-bottom:50px; 
	margin-top:50px; 
	padding: 25px 50px 25px 50px;    
    text-align: center;
    font-size: 20px;}
#incorrect{
	color:red;
	text-align: center;
	font-size: 14px;
}
input[type=text], input[type=password] {
    width: 100%;
    padding: 12px 20px;
    margin: 8px 0;
    display: inline-block;
    border: 1px solid #ccc;
    box-sizing: border-box;
}
</style>
<title>Login - Pharmacy Database Management System</title>
</head>
<body>
<center><h1><br>Login</h1>
<div id="loginform1">
<input type="text" name="usrname" id="uname" placeholder="User Name" autofocus required><br> 
<input type="password" name="pass" id="passc" placeholder="Password" required><br><br><br>
<input type="button" value="Login" style="background-color:#ccc; border-radius: 5px; padding: 12px; font-weight: bold; font-size: 14px;" onclick="javascript: scrubdata()">
<h3 id="invalidnotify"></h3>
</div>
<form type="hidden" id="loginform" method="post" action="login.php">
<input type="hidden" id="usrn" name="usrn"/>
<input type="hidden" id="psw" name="psw"/>
</form>
<br><br>
</center>
<script>
var x="",y="",z;
const isAlphaNumeric = ch => {
	return ch.match(/^[A-Za-z0-9]+$/i) !== null;
}
function sha256(str) {
  // We transform the string into an arraybuffer.
  var buffer = new TextEncoder("utf-8").encode(str);
  return crypto.subtle.digest("SHA-256", buffer).then(function (hash) {
    return hex(hash);
  });
}

function hex(buffer) {
  var hexCodes = [];
  var view = new DataView(buffer);
  for (var i = 0; i < view.byteLength; i += 4) {
    // Using getUint32 reduces the number of iterations needed (we process 4 bytes each time)
    var value = view.getUint32(i)
    // toString(16) will give the hex representation of the number without padding
    var stringValue = value.toString(16)
    // We use concatenation and slice for padding
    var padding = '00000000'
    var paddedValue = (padding + stringValue).slice(-padding.length)
    hexCodes.push(paddedValue);
  }

  // Join all the hex strings into one
  return hexCodes.join("");
}


function scrubdata(){
	document.getElementById("invalidnotify").innerHTML = "";
	x=document.getElementById("uname").value;
	y=document.getElementById("passc").value;
	var n = isAlphaNumeric(x);
	var m = isAlphaNumeric(y);
	if(n==false || m==false || x.length<3 || y.length<3)
	{	document.getElementById("invalidnotify").innerHTML = "Invalid Username or Password";	}
	else{
		sha256(y).then(function(digest) {
		z=digest;
		});
		document.getElementById("usrn").innerHTML = x;
		setTimeout(function() {
			var formobj = document.forms['loginform'];
			formobj.elements["usrn"].value = x;
			formobj.elements["psw"].value = (z.toString());
			formobj.submit();
		}, 500);		
	}
}
</script>	
</body>
</html>

