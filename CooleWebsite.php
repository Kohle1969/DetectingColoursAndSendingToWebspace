<?php
$db_link = mysqli_connect ('localhost','user','password','db_name'); //Verbindung zur Datenbank
$last = "SELECT * FROM table WHERE ID = (SELECT MAX(ID) FROM table)"; //Auswählen von Tabelle und Datenbank, nur letzte Zeile
$db_erg1 = mysqli_query( $db_link, $last );
$werte = mysqli_fetch_array( $db_erg1, MYSQLI_ASSOC); //Abspeichern von den ausgelesenen Werten

$ID = $werte['ID'];
if ( ! $ID )
{
  $page = $_SERVER['PHP_SELF'];
  $sec = "2";
  header("Refresh: $sec; url=$page");
  die("Keine Werte in der Datenbank");
} //Falls ID nicht ausgelesen wird, also Datenbank leer, Website wird beendet und alle 2 Sekuden neu geladen
$rot = $werte['ROT'];
$gruen = $werte['GRUEN'];
$blau = $werte['BLAU'];
$gelb = $werte['GELB'];
$orange = $werte['ORANGE']; //Werte der letzten Zeile werden in den entsprechenden Variablen gespeichert

//-------------------PHPlot---------------------------------

require_once 'Zeug/phplot-6.2.0/phplot.php'; //Einbinden von PHPlot

$data = array(
 array('ROT', $rot),
 array('GRUEN', $gruen),
 array('BLAU', $blau),
 array('GELB', $gelb),
 array('ORANGE', $orange), //Eingegebene Daten werden als array gespeichert
);

$plot = new PHPlot(1100, 900);  //PHPlot Einstellungen
$plot->SetImageBorderType('plain');
$plot->SetPlotType('pie');
$plot->SetDataType('text-data-single');
$plot->SetDataValues($data);
$plot->SetDataColors(array('red', 'green', 'blue', 'yellow', 'orange'));
$plot->SetTitle("Relative Verteilung der gemessenen Farben:"); //Titel der Grafik
foreach ($data as $row)
$plot->SetLegend(implode(': ', $row));
$plot->SetLegendPixels(5, 5);
$plot->SetIsInline(true); // Grafik wird gespeichert und nicht angezeigt
$plot->SetOutputFile('Zeug/Statistik.png'); //Ordner zur Abspeicherung
$plot->DrawGraph(); //Grafik wird gezeichnet

?>

<!-------------------------------------------------------------------------------------------------------------->

<html>
<head>
  <meta http-equiv="refresh" content="5"> <!--Seite wird alle 5 Sekunden neu geladen, damit die Werte aktuell bleiben-->
  <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <title>Jonas' coole Website</title> <!--Einbinden von verschiedenen optischen Verbesserungen, nicht Coderelevant-->
  <style type="text/css" media="screen">
* {
  margin: 0px 0px 0px 0px;
  padding: 0px 0px 0px 0px;
}

body, html {

  padding: 3px 3px 3px 3px;

  background-color: #D8DBE2;

  font-family: Verdana, sans-serif;
  font-size: 11pt;
  text-align: center;
}

div.main_page {
  position: relative;
  display: table;

  width: 800px;

  margin-bottom: 3px;
  margin-left: auto;
  margin-right: auto;
  padding: 0px 0px 0px 0px;

  border-width: 2px;
  border-color: #212738;
  border-style: solid;

  background-color: #FFFFFF;

  text-align: left;
}

div.content_section_text {
  padding: 4px 8px 4px 8px;

  color: #000000;
  font-size: 100%;
}

div.content_section_text ul, div.content_section_text li {
  padding: 4px 8px 4px 16px;
}

div.section_header {
  padding: 3px 6px 3px 6px;

  background-color: #8E9CB2;

  color: #FFFFFF;
  font-weight: bold;
  font-size: 112%;
  text-align: center;
}

div.section_header_red {
  background-color: #CD214F;
}

div.table_of_contents_item a,
div.content_section_text a {
  text-decoration: none;
  font-weight: bold;
}

div.content_section_text a:link,
div.content_section_text a:visited,
 div.content_section_text a:active {
  background-color: #DCDFE6;

  color: #000000;
}
div.validator {
}
  </style>
</head>
<body>
<div class="main_page"> <!-- Aufrufen der vorher defiierten div classes-->
<div class="content_section_text">
  <div class="section_header section_header_red">
    Diagramm:
  </div>
<img src="Zeug/Statistik.png" alt="Anzeigefehler :)" hspace=20 vspace=20 style="float: left;width: 1100;height: 900;"> <?php //Das auf den Server gespeicherte Bild wird geldaen

  $num = 26;
  $min = $ID-$num; //Es sollen nr 25 Werte angezeigt wrden, da sonst die Seite abstürtzt
  if($min < 0) //Falls es keine 25 Werte in der Datenbank gibt, wird $min=0 gesetzt, damit nur die vorhandenen Werte ausgegeben werden
     { $min = 0; }
  $sql = "SELECT * FROM table ORDER BY ID ASC LIMIT $min ,$ID";

  $db_erg = mysqli_query( $db_link, $sql );
  if ( ! $db_erg )
  {
    die('Ungültige Abfrage'); //Falls nichts ausgelesen werden konnte wird das Script beendet
  }
?>

<div class="section_header section_header_red">
  Tabelle:
</div>
<br>
<?php
  echo '<table border="1">'; //Grenzen für die Tabelle
  while ($zeile = mysqli_fetch_array( $db_erg, MYSQLI_ASSOC)) //Während Daten ausgelesen werden können wird die Tabelle erstellt
  {
    echo "<tr>"; //Die ausgelesenen Spalten werden vertikal auf die Website geschrieben
    echo "<td>ID: ". $zeile['ID'] . "</td>";
    echo "<td>ROT: ". $zeile['ROT'] . "</td>";
    echo "<td>GRUEN: ". $zeile['GRUEN'] . "</td>";
    echo "<td>BLAU: ". $zeile['BLAU'] . "</td>";
    echo "<td>GELB: ". $zeile['GELB'] . "</td>";
    echo "<td>ORANGE: ". $zeile['ORANGE'] . "</td>";
    echo "<td>ANZAHL: ". $zeile['ANZAHL'] . "</td>";
    echo "</tr>";
  }
  echo "</table>";
  mysqli_free_result( $db_erg ); //Die Variable $db_erg wird wieder freigegeben
?>
<br>
<a href="http://jonaskohlenberger.de">Zurück zur Hauptwebsite</a>
</div>
</div>
</body>
</html>
