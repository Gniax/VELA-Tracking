<?php 
// Sous Xamp ( rasbian)
try
{
	$bdd = new PDO('mysql:host=localhost;dbname=test;charset=utf8', 'root', 'password');
	   // On active la gestion des erreurs     
	$bdd->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch (Exception $e)
{    
       die('Erreur : ' . $e->getMessage());
}
	$reponse = $bdd->query('SELECT * FROM test1');

// On affiche chaque entrée une à une
	while ($donnees = $reponse->fetch())
	{
		?> 
<p>
        le nom est : <?php echo $donnees['nom']; ?><br />     
        le prenom est : <?php echo $donnees['prenom']; ?>,
        l'âge est <?php echo $donnees['age']; ?> ans !<br />  
        le sexe est :  <?php echo $donnees['sexe']; ?>
     <p>
 	<?php
 	 } 
 
$reponse->closeCursor(); // Termine le traitement de la requête 
 
?>