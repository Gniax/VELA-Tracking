<?php 
// Sous Xamp ( rasbian)
try
{
	$bdd = new PDO('mysql:host=localhost;dbname=bdd_mer;charset=utf8', 'jordan', 'pi');
	   // On active la gestion des erreurs     
	$bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch (Exception $e)
{    
       die('Erreur : ' . $e->getMessage());
}
	$reponse = $bdd->query('SELECT * FROM bdd_mer');
?>