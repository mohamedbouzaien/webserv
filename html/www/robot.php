<!DOCTYPE html>
<html>
<head>
<style>
/*fonts*/
@import url(https://fonts.googleapis.com/css?family=PT+Sans:400,400italic);

@import url(https://fonts.googleapis.com/css?family=Droid+Serif);

html, body{
  background:#2F1E27;
}

body{
  counter-reset:section;
  text-align:center;
}

.container{
  position:relative;
  top:100px;
}

.container h1, .container span{
  font-family:"Pt Sans", helvetica, sans-serif;
}

.container h1{
  text-align:center;
  color:#fff;
  font-weight:100;
  font-size:2em;
  margin-bottom:10px;
}

.container h2{
  font-family:"droid serif";
  font-style:italic;
  color:#d3b6ca;
  text-align:center;
  font-size:1.2em;
}

.container form span:before {
  counter-increment:section;
  content:counter(section);
  border:2px solid #4c2639;
  width:40px;
  height:40px;
  color:#fff;
  display:inline-block;
  border-radius:50%;
  line-height:1.6em;
  font-size:1.5em;
  position:relative;
  left:-22px;
  top:-11px;
  background:#2F1E27;
}

form{
  margin-top:25px;
  display:inline-block;
}

.fields{
  border-left:2px solid #4c2639
}

.container span{
    margin-bottom:22px;
    display:inline-block;
}

.container span:last-child{
   margin-bottom:-11px;
}

input{
  border:none;
  outline:none;
  display:inline-block;
  height:34px;
  vertical-align:middle;
  position:relative;
  bottom:14px;
  right:9px;
  border-radius:22px;
  width:220px;
  box-sizing:border-box;
  padding:0 18px;
}

input[type="submit"]{
  background:rgba(197,62,126,.33) ! important;
  color:#fff;
  position:relative;
  left:9px;
  top:25px;
  width:100px;
  cursor:pointer;
}
</style>
</head>
<body>
<div class="container">
<?php
  error_reporting(0);
  if( $_GET["name"] || $_GET["age"] || $_GET["job"])
  {
	echo "<h1>Hello ";
	  if ($_GET["name"])
		  echo $_GET["name"];
	  else
		  echo "young newcomer";
	echo ", how are you ?</h1><br>";
	  if ($_GET["age"])
		  echo "<h2>" . $_GET["age"] . " is a beautiful age.</h2><br>";
	  else
		  echo "<h2>I know some humans don't want to tell their age, no problem bwo !</h2><br>";
	if ($_GET["job"])
		echo "<h2> My father also worked as " . $_GET["job"] . "</h2>";
	else
		echo "<h2> My father was also unemployed :(</h2>";
      exit();
  }
?>
  <h2>Hello Human, Who are you ?</h2>
  <h1>Tell me who you are</h1>
  
  <form action="<?php $_PHP_SELF ?>" method="GET">
    <div class="fields">
      <span>
       <input placeholder="Name" type="text" name="name"/>
    </span>
    <br />
     <span>
       <input placeholder="Age" type="text" name="age"/>
	</span>
    <br />
     <span>
       <input placeholder="Job" type="text" name="job"/>
    </span>
    </div>
    <div class="submit">
      <input class="submit" value="Submit" type="submit" />
    </div>
  </form>
</div>

</body>
</html>
