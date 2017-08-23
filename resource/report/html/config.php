<?php
@$server="localhost";
@$username="root";
@$password="";
@$database="report";

@$conn=mysql_connect($server,$username,$password)
	or die("could not connect mysql");

mysql_select_db($database,$conn)
	or die("could not open database");
?>