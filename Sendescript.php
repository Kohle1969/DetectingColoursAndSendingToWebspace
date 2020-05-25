<?php
class farben{
 public $link='';
 function __construct($r, $g, $b, $ge, $o, $a){
  $this->connect();
  $this->storeInDB($r, $g, $b, $ge, $o, $a);//Erstellung der Variablen für die Arduino Werte und die benötigten Funktionen
 }

 function connect(){
  $this->link = mysqli_connect('localhost','user','password') or die('Cannot connect to the DB');
  mysqli_select_db($this->link,'farben') or die('Cannot select the DB'); //Verbindung zur passenden Datenbank
 }

 function storeInDB($r, $g, $b, $ge, $o, $a){
  $query = "insert into table_name set ROT='".$r."', GRUEN='".$g."', BLAU='".$b."', GELB='".$ge."', ORANGE='".$o."', ANZAHL='".$a."'";
  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query); //Die Varialen werden in die entsprechenden Spalten der Datenbank eingesetzt
 }

}
if($_GET['r'] != '' and  $_GET['g'] != '' and  $_GET['b'] != '' and  $_GET['ge'] != '' and  $_GET['o'] != '' and  $_GET['a'] != ''){
$farben=new farben($_GET['r'],$_GET['g'],$_GET['b'],$_GET['ge'],$_GET['o'],$_GET['a']);
} //Wenn alle Variablen vom Arduino in der Adresszeile gesetzt werden, werden diese im Script gespeichert


?>
