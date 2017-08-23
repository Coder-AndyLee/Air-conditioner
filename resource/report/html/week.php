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
for($i=0; $i<5 ;$i++)
{
	if($i==0)
	{
		$date_min="2015-06-00";
		$date_max="2015-06-07";
	}
	else if($i==1)
	{
		$date_min="2015-06-06";
		$date_max="2015-06-14";
	}
	else if($i==2)
	{
		$date_min="2015-06-13";
		$date_max="2015-06-21";
	}
	else if($i==3)
	{
		$date_min="2015-06-20";
		$date_max="2015-06-28";
	}
	else
	{
		$date_min="2015-06-27";
		$date_max="2015-06-31";
	}
	for($j=0; $j<6 ;$j++)
	{
		$k=$j+1;
		$query="select sum(time) from on_off where room_num='$k' and date>'$date_min' and date<'$date_max'";
		@$result[$i][$j]=mysql_query($query);
		@$row[$i][$j]=mysql_fetch_array($result[$i][$j]);
		$query1="select sum(fee) from request where room_num='$k' and start_time>'$date_min' and end_time<'$date_max'";
		@$result1[$i][$j]=mysql_query($query1);
		@$row1[$i][$j]=mysql_fetch_array($result1[$i][$j]);
	}
}
?>

<h1 style="position:relative;font-size:52px;">6 月 周报表</h1>
<html xmlns="http://www.w3.org/1999/xhtml">
<body>
	<p style="position:relative;font-size:52px;">6 月 第一周周报表</p>
	<p style="position:relative;font-size:32px;"></p>
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
			<?php echo $row[0][0]["sum(time)"]?></br>
			<?php echo $row[0][1]["sum(time)"]?></br>
			<?php echo $row[0][2]["sum(time)"]?></br>
			<?php echo $row[0][3]["sum(time)"]?></br>
			<?php echo $row[0][4]["sum(time)"]?></br>
			<?php echo $row[0][5]["sum(time)"]?></br>
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[0][0]["sum(fee)"]?></br>
			<?php echo $row1[0][1]["sum(fee)"]?></br>
			<?php echo $row1[0][2]["sum(fee)"]?></br>
			<?php echo $row1[0][3]["sum(fee)"]?></br>
			<?php echo $row1[0][4]["sum(fee)"]?></br>
			<?php echo $row1[0][5]["sum(fee)"]?></br>
		</div>
	</div>
	
	<p style="position:relative;font-size:52px;">6 月 第二周周报表</p>
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
			<?php echo $row[1][0]["sum(time)"]?></br>
			<?php echo $row[1][1]["sum(time)"]?></br>
			<?php echo $row[1][2]["sum(time)"]?></br>
			<?php echo $row[1][3]["sum(time)"]?></br>
			<?php echo $row[1][4]["sum(time)"]?></br>
			<?php echo $row[1][5]["sum(time)"]?></br>
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[1][0]["sum(fee)"]?></br>
			<?php echo $row1[1][1]["sum(fee)"]?></br>
			<?php echo $row1[1][2]["sum(fee)"]?></br>
			<?php echo $row1[1][3]["sum(fee)"]?></br>
			<?php echo $row1[1][4]["sum(fee)"]?></br>
			<?php echo $row1[1][5]["sum(fee)"]?></br>
		</div>
	</div>
	
	<p style="position:relative;font-size:52px;">6 月 第三周周报表</p>
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
			<?php echo $row[2][0]["sum(time)"]?></br>
			<?php echo $row[2][1]["sum(time)"]?></br>
			<?php echo $row[2][2]["sum(time)"]?></br>
			<?php echo $row[2][3]["sum(time)"]?></br>
			<?php echo $row[2][4]["sum(time)"]?></br>
			<?php echo $row[2][5]["sum(time)"]?></br>
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[2][0]["sum(fee)"]?></br>
			<?php echo $row1[2][1]["sum(fee)"]?></br>
			<?php echo $row1[2][2]["sum(fee)"]?></br>
			<?php echo $row1[2][3]["sum(fee)"]?></br>
			<?php echo $row1[2][4]["sum(fee)"]?></br>
			<?php echo $row1[2][5]["sum(fee)"]?></br>
		</div>
	</div>
	
	<p style="position:relative;font-size:52px;">6 月 第四周周报表</p>
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
			<?php echo $row[3][0]["sum(time)"]?></br>
			<?php echo $row[3][1]["sum(time)"]?></br>
			<?php echo $row[3][2]["sum(time)"]?></br>
			<?php echo $row[3][3]["sum(time)"]?></br>
			<?php echo $row[3][4]["sum(time)"]?></br>
			<?php echo $row[3][5]["sum(time)"]?></br>	
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[3][0]["sum(fee)"]?></br>
			<?php echo $row1[3][1]["sum(fee)"]?></br>
			<?php echo $row1[3][2]["sum(fee)"]?></br>
			<?php echo $row1[3][3]["sum(fee)"]?></br>
			<?php echo $row1[3][4]["sum(fee)"]?></br>
			<?php echo $row1[3][5]["sum(fee)"]?></br>
		</div>
	</div>	
	
	<p style="position:relative;font-size:52px;">6 月 第五周周报表</p>
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
			<?php echo $row[4][0]["sum(time)"]?></br>
			<?php echo $row[4][1]["sum(time)"]?></br>
			<?php echo $row[4][2]["sum(time)"]?></br>
			<?php echo $row[4][3]["sum(time)"]?></br>
			<?php echo $row[4][4]["sum(time)"]?></br>
			<?php echo $row[4][5]["sum(time)"]?></br>		
		</div>
		<div id="fee">
			当日产生费用</br></br>
			<?php echo $row1[4][0]["sum(fee)"]?></br>
			<?php echo $row1[4][1]["sum(fee)"]?></br>
			<?php echo $row1[4][2]["sum(fee)"]?></br>
			<?php echo $row1[4][3]["sum(fee)"]?></br>
			<?php echo $row1[4][4]["sum(fee)"]?></br>
			<?php echo $row1[2][5]["sum(fee)"]?></br>
		</div>
	</div>
	
</body>
</html>
