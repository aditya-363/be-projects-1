<?php
session_start();
if(isset($_SESSION['username']) && $_SESSION['linkto']=='stockmgr.php'){
	$username=$_SESSION['username'];
	echo '<script>console.log(\''.$_SESSION['linkto'].' '.$_SESSION['username'].'\');</script>';
}else{
header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/index.php");
exit();
} 
?>
<html>
<head>
<style>
#logindiv {
    
    border: 1px solid #fff;
	overflow: auto;
}
</style>
<title>Stock Manager - Pharmacy Database Management System</title>
</head>
<body>
<div id="logindiv">
<center>
<h3 id="stockpg">Stock Manager</h3></center>
<input type='button' style="background-color: #4CAF50; border: none; color: white; padding: 10px 10px; margin-right:20px; margin-bottom:20px; text-align: center;  text-decoration: none;  display: inline-block;  font-size: 16px; float: right;" id="logoutb1" name="logoutb1" value='Logout' onclick='logout()'>
</div>
<br><br>
</center>


<script>
function logout(){
	window.location.href = '/dbmsphp/logout.php';
}
</script>
</body>
</html>