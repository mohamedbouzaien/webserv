<html>
<body>
<?php
echo 'Bonjour I am ' . htmlspecialchars($_GET["name"]) . ' and I am a ' . htmlspecialchars($_GET["job"]) . "!\n" ;
//print_r($_SERVER);
?>
</body>
</html>
