<?php 
// Sous Xamp ( rasbian)
try
{
	$bdd = new PDO('mysql:host=localhost;dbname=bdd_mer;charset=utf8', 'jordan', 'pi');
	$bdd = new PDO('mysql:host=localhost;dbname=bdd_terre;charset=utf8', 'jordan', 'pi');
}
catch (Exception $e)
{    
       die('Erreur : ' . $e->getMessage());
}
$link = mysql_query('INSERT INTO ' . 'bdd_terre.' . 'balise' . 'bateau' . 'course' . ' SELECT * FROM ' . 'bdd_mer.' . 'balise' . 'bateau' . 'course'); 
?>
