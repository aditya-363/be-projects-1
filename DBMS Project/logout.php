<?php
session_start();
unset($_SESSION['username']);
unset($_SESSION['linkto']);
session_unset();
header("location:http://".$_SERVER['HTTP_HOST'].dirname($_SERVER['PHP_SELF'])."/index.php");
?>