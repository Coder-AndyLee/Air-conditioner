<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>分布式温控系统-周报表</title>
	<link rel="stylesheet" type=”text/css” href="../css/daily.css" />
<p style="position:relative;top:20px;left:50px;height:40px;width:70px;text-align:left;font-size:30px;font-weight:700;display:block;">
	<a href="index.php" style="text-decoration:none;color:black;">后退</a>
</p>
</head>

<?php
include 'config.php';
for($i=0; $i<6 ;$i++)
{
	$j=$i+1;
	$query="select sum(time) from on_off where room_num='$j'";
	@$result[$i]=mysql_query($query);
	@$row[$i]=mysql_fetch_array($result[$i]);
	
	$query1="select sum(fee) from request where room_num='$j'";
	@$result1[$i]=mysql_query($query1);
	@$row1[$i]=mysql_fetch_array($result1[$i]);
}
?>
<h1 style="position:relative;font-size:52px;">6 月 月报表</h1>
<html xmlns="http://www.w3.org/1999/xhtml">
<body>
	<div id="time_area">
		<div id="room_num">
			房间号</br></br>
			1</br>
			2</br>
			3</br>
			4</br>
			5</br>
			6</br>
		</div>
		<div id="open_time">
			该房间开关机次数</br></br>
			<?php echo $row[0]["sum(time)"]?></br>
			<?php echo $row[1]["sum(time)"]?></br>
			<?php echo $row[2]["sum(time)"]?></br>
			<?php echo $row[3]["sum(time)"]?></br>
			<?php echo $row[4]["sum(time)"]?></br>
			<?php echo $row[5]["sum(time)"]?></br>
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[0]["sum(fee)"]?></br>
			<?php echo $row1[1]["sum(fee)"]?></br>
			<?php echo $row1[2]["sum(fee)"]?></br>
			<?php echo $row1[3]["sum(fee)"]?></br>
			<?php echo $row1[4]["sum(fee)"]?></br>
			<?php echo $row1[5]["sum(fee)"]?></br>
		</div>
	</div>

	
</body>
</html>
