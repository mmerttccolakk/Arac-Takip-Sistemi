<?php $dosya = fopen('veri.txt', 'r');
$icerik = fread($dosya, filesize('veri.txt'));
$bolunmus = explode(":", $icerik);
fclose($dosya);

?>
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<title>Araç Takip Sistemi</title>
<link href="site/genel.css" rel="stylesheet" type="text/css">
</head>

<body class="arka_plan"><div align="center">
	
	<table width="1200" border="0" cellspacing="0" cellpadding="0">
  <tbody>
	  	<tr>
		<td colspan="2"><img src="site/banner.fw.png" width="1200" height="200" alt=""/></td>
    </tr>
    <tr>
      <td width="400" bgcolor="#5C5C5C" align="center">
	  <img src="site/st.png" width="100" height="100" alt=""/><h2>Bağlı Uydu :</h2><h3><?php 
	  echo $bolunmus[2]; 
	  ?></h3></td>
      <td width="800" rowspan="5"><iframe src="https://www.google.com/maps/embed/v1/view
?key=AIzaSyClt0MRBtvV4wrVr8REDHjV_4Nc2-BNU1o&center=<?php echo $bolunmus[0]; ?>,<?php 
echo $bolunmus[1];
 ?>&zoom=19&maptype=roadmap" width="800" height="600" frameborder="0" style="border:0" allowfullscreen></iframe></td>
    </tr>
    <tr bgcolor="#5C5C5C">
      <td align="center"><h2>Kordinatlar :</h2><h3><?php 
	  echo $bolunmus[0];
	  ?> , <?php 
	  echo $bolunmus[1];
	  ?></h3><h2>HDOP :</h2><h3><?php 
	  echo $bolunmus[3];
	  ?></h3></td>
    </tr>
    <tr bgcolor="#5C5C5C">
      <td align="center"><h2>Yükseklik:</h2><h3><?php echo $bolunmus[4]; ?> mt</h3></td>
    </tr>
    <tr bgcolor="#5C5C5C">
      <td align="center"><h2>Hız :</h2><h3><?php echo $bolunmus[5]; ?> km/s</h3></td>
    </tr>
    <tr bgcolor="#5C5C5C">
      <td align="center"><h2>Gecikme :</h2><h3><?php echo $bolunmus[6]; ?> ms</h3></td>
    </tr>
	<tr>
		<td colspan="2" align="center" bgcolor="#5C5C5C">Copyright © 2019 Mert Çolak. Her hakkı saklıdır.</td>
    </tr>
  </tbody>
</table>
</div>
</body>
</html>
