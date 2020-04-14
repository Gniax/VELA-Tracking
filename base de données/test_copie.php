<?php

// BASE 2
$host2 = "localhost"; // voir hébergeur
$user2 = "jordan"; // vide ou "root" en local
$pass2 = "pi"; // vide en local
//$bdd2 = "terre"; // nom de la bdd

//BASE ORIGINAL
$host1 = "localhost"; // voir hébergeur
$user1 = "jordan"; // vide ou "root" en local
$pass1 = "pi"; // vide en local
//$bdd = "bdd_mer"; // nom de la BD


$bdd1 = mysql_connect($host1,$user1,$pass1);
$bdd2 = mysql_connect($host2,$user2,$pass2);
mysql_query("TRUNCATE table terre.updatedate2",$bdd2);

$res = mysql_query("SELECT * FROM bdd_mer.updatedate",$bdd1)or die("<pre><b>MySQL Error:<b/>". mysql_error() ."</pre>");
while ($row = mysql_fetch_array($res))
{
   mysql_query("INSERT INTO terre.updatedate2 (id, participant, temps, longitude, latitude, vitesse) VALUES ('".mysql_real_escape_string($row['id'])."','".mysql_real_escape_string($row['participant'])."','".mysql_real_escape_string($row['temps'])."','".mysql_real_escape_string($row['longitude'])."','".mysql_real_escape_string($row['latitude'])."','".mysql_real_escape_string($row['vitesse'])"')",$bdd2)or die("<pre><b>MySQL Error:<b/>". mysql_error() ."</pre>");
}

echo"hello";

?>