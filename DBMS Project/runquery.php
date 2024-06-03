<?php
session_start();
if(isset($_SESSION['username']) && ($_SESSION['username']=='admin'||$_SESSION['username']=='root')){
	$username=$_SESSION['username'];
}else{
	header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/index.php");
	exit();
} 
?>
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
table, th, td {
	border: 1px solid black;
	padding: 2px 2px;	
}
</style>
<title>Admin Console</title>
</head>
<body>
<div id="logindiv">
<center>
<h2 id="admc">Admin Console</h2></center>
<input type="button" value="Logout" style="background-color: #4CAF50; border: none; color: white; padding: 10px 10px; margin-right:20px; margin-bottom:20px; text-align: center;  text-decoration: none;  display: inline-block;  font-size: 16px; float: right;" id="logoutb1" name="logoutb1" onclick='logout()'>
</div>
<br><br></center>
<h2>Results</h2>
<?php
function table( $result ) {
    $result->fetch_array( MYSQLI_ASSOC );
    echo '<table>';
    tableHead( $result );
    tableBody( $result );
    echo '</table>';
}

function tableHead( $result ) {
    echo '<thead>';
    foreach ( $result as $x ) {
    echo '<tr>';
    foreach ( $x as $k => $y ) {
        echo '<th>' . ucfirst( $k ) . '</th>';
    }
    echo '</tr>';
    break;
    }
    echo '</thead>';
}

function tableBody( $result ) {
    echo '<tbody>';
    foreach ( $result as $x ) {
    echo '<tr>';
    foreach ( $x as $y ) {
        echo '<td>' . $y . '</td>';
    }
    echo '</tr>';
    }
    echo '</tbody>';
}
$rawq=$_POST["rawquery"];
$conn = new mysqli("localhost", "root", "");
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 
mysqli_select_db($conn,"pharmacydb") or die("DB Selection Fail.");
$result = mysqli_query($conn,$rawq);
if($result == False) {echo "Query Failed.";}
else{
	if($result == True)
	{	echo "Query Successful.<br>"; }
	if (is_object($result) && mysqli_num_rows($result) > 0) {
		//while($row = mysqli_fetch_array($result,MYSQLI_NUM)) {
		//	foreach($row as $i)
		//		echo $i."  ";
		//	echo "<br>";
		//}
		table($result);
	} else {
    echo "No results returned.";
	}
}
mysqli_close($conn);
?>
<script>
function logout(){
	window.location.href = '/dbmsphp/logout.php';
}
</script>
</body>
</html>

