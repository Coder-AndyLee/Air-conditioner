<html xmlns="http://www.w3.org/1999/xhtml">	
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>分布式温控系统-日报表</title>
	<link rel="stylesheet" type=”text/css” href="../css/daily.css" />
<p style="position:relative;top:20px;left:50px;height:40px;width:70px;text-align:left;font-size:30px;font-weight:700;display:block;">
	<a href="index.php" style="text-decoration:none;color:black;">后退</a>
</p>
</head>

<body>
<?php 
include 'config.php';
for($day=1; $day<=30 ;$day++)
{
	$date = date('Y-m-d',strtotime('2015-06-01')+((int)$day-1)*24*60*60);
	$start_date = date('Y-m-d H:i:s',strtotime('2015-06-01 00:00:00')+($day-1)*24*60*60);
	$end_date = date('Y-m-d H:i:s',strtotime('2015-06-01 00:00:00')+($day)*24*60*60);
	
	for($i=0; $i<6 ;$i++)
	{
		$j=$i+1;
		$query="select sum(time) from on_off where room_num='$j' and date='$date'";
		@$result[$i]=mysql_query($query);
		@$row[$i]=mysql_fetch_array($result[$i]);
		
		$query1="select sum(fee) from request where room_num='$j' and start_time>'$start_date' and end_time<'$end_date'";
		@$result1[$i]=mysql_query($query1);
		@$row1[$i]=mysql_fetch_array($result1[$i]);
	}
	
	$query2="select count(*) from request where start_time>'$start_date' and end_time<'$end_date'";
	@$result2=mysql_query($query2);
	@$row2=mysql_fetch_array($result2);
	$request_num = $row2["count(*)"];

if($row[0]["sum(time)"]!=NULL || $row[1]["sum(time)"]!=NULL || $row[2]["sum(time)"]!=NULL 
	|| $row[3]["sum(time)"]!=NULL || $row[4]["sum(time)"]!=NULL || $row[5]["sum(time)"]!=NULL)
{?>
<h1 style="position:relative;font-size:52px;">6 月 <?php echo $day;?> 日 日报表</h1>
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
	<?php
	}
	
	$query3="select * from request where start_time>'$start_date' and start_time<'$end_date'";
	@$result3=mysql_query($query3);
	for($temp=0; $temp < $request_num ;$temp++)
	{
		@$row3[$temp]=mysql_fetch_array($result3);
	}
	for($temp=0; $temp < $request_num ;$temp++)
	{
	?>
	<!--封装好的区域起始点-->
	<!--以一条操作记录为单位-->
	<div class="one_room">
		<p class="data">
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
			房间号：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["room_num"]?></br>
			温控请求开始时间：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["start_time"]?></br>
			温控请求结束时间：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["end_time"]?></br>
			
			温控请求开始温度：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["start_temp"]?></br>
			温控请求结束温度：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["end_temp"]?></br>
			
			温控请求消耗能量：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo ($row3[$temp]["fee"]/5)?></br>
			温控请求产生费用：&nbsp;&nbsp;&nbsp;&nbsp;<?php echo $row3[$temp]["fee"]?></br>
		</p>
	</div>
	<p style="height:5px;"></p>
	<!--封装好的区域终止点-->
	<?php }?>
<?php }?>
</body>
</html>