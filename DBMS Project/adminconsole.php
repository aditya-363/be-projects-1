<?php
session_start();
if(isset($_SESSION['username']) && ($_SESSION['username']=='admin'||$_SESSION['username']=='root')){
	$username=$_SESSION['username'];
}else{
header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/index.php");
exit();
} ?>
<html>
<head>
<style>
#rawquery{
    width: 100%;
    padding: 3px 3px;
    margin: 8px 2px;    
    border: 1px solid #ccc;
	height: 200px;
}
#execq{
	display: block;
    margin-left: auto;
    margin-right: auto;
}
#logindiv {
    
    border: 1px solid #fff;
	overflow: auto;
}
</style>
<title>Admin Console - Pharmacy Database Management System</title>
</head>
<body>
<div id="logindiv">
<center>
<h2 id="admc">Admin Console</h2></center>
<input type='button' style="background-color: #4CAF50; border: none; color: white; padding: 10px 10px; margin-right:20px; margin-bottom:20px; text-align: center;  text-decoration: none;  display: inline-block;  font-size: 16px; float: right;" id="logoutb1" name="logoutb1" value='Logout' onclick='logout()'>
</div>
<br><br>
</center>
<center>
<p>The textbox accepts raw SQL queries without any checks one query at a time. Use responsibly and perform commits as required.</p>
<br><br></center>
<form id="queryform" method="post" action="runquery.php">
<textarea id="rawquery" name="rawquery" placeholder="Type here"></textarea><br><br>
<input type="submit" id="execq" style="background-color:#eef; border-radius: 5px; padding: 12px; font-weight: bold; font-size: 14px;" value="Execute">
</form>
<script>
function logout(){
	window.location.href = '/dbmsphp/logout.php';
}
</script>
</body>
</html>

