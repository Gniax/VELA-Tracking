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
	$reponse = $bdd->query('SELECT * FROM course');

// On affiche chaque entrée une à une
	while ($donnees = $reponse->fetch())
	{
		?> 
<p>
     <strong>l'id de la course est : </strong> : <?php echo $donnees['idcourse']; ?><br />     
     le temps du coureur : <?php echo $donnees['temps']; ?>,
      sa position en longitude : <?php echo $donnees['longitude']; ?> euros !<br />  
      sa position en lattitude :  <?php echo $donnees['latitude']; ?>
      d'une vitesse de :  <?php echo $donnees['vitesse']; ?>
     l'id de la balise est : <?php echo $donnees['nom']; ?> 
     l'id du coureur est : <?php echo $donnees['nom']; ?>
     <p>
 	<?php
 	 } 
 
$reponse->closeCursor(); // Termine le traitement de la requête 
 
?>
