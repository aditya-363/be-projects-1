<?php
$varusr = $_POST["usrn"];
$varpswd = $_POST["psw"];
$userquery = "SELECT username,passhash,linkto FROM usercred WHERE username = \"".$varusr."\";";
$conn = new mysqli("localhost", "root", "");
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
mysqli_select_db($conn,"pharmacydb") or die("DB Selection Fail.");
$result = $conn->query($userquery);
if ($result->num_rows > 0) {
	$row = $result->fetch_assoc();
	if ((strtoupper($row["passhash"])) == (strtoupper($varpswd))){
		session_start();
		$_SESSION['username'] = $varusr;
		$nextlink = $row["linkto"];
		$_SESSION['linkto'] = $nextlink;
		header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/".$nextlink);
	}
	else {
		echo "<font color=red size=20><center><br><br><br>Incorrect Username or Password</center></font>";
		echo "<script>setTimeout(function() {
				window.location.href = \"http://localhost/dbmsphp/index.php\";
				}, 1500);</script>";
	}
}
else {
		echo "<font color=red size=20><center><br><br><br>Incorrect Username or Password</center></font>";
		echo "<script>setTimeout(function() {
				window.location.href = \"http://localhost/dbmsphp/index.php\";
				}, 1500);</script>";
}
$conn->close();
?>