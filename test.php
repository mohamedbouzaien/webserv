<?php
  error_reporting(0);
  if( $_GET["name"] || $_GET["weight"] )
  {
      echo "Welcome ". $_GET['name']. "<br>";
      echo "You are ". $_GET['weight']. " kgs in weight.";
      exit();
  }
?>
<html>
<body>
  <form action="<?php $_PHP_SELF ?>" method="GET">
    Name: <input type="text" name="name" />
    Weight:<input type="text" name="weight" />
           <input type="submit" />
  </form>
</body>
</html>
