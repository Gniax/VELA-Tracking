<?php 
// Sous Xamp ( rasbian)
try
{
	$bdd = new PDO('mysql:host=localhost;dbname=bdd_mer;charset=utf8', 'jordan', 'pi');
}
catch (Exception $e)
{    
       die('Erreur : ' . $e->getMessage());
}
?>