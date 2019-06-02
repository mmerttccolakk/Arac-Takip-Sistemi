<?php

//gelen veriler degiskenlere atanıyor
$la=$_GET["la"];
$lo=$_GET["lo"];
$st=$_GET["st"];
$hd=$_GET["hd"];
$al=$_GET["al"];
$sp=$_GET["sp"];
$ag=$_GET["ag"];

$ekran = "$la:$lo:$st:$hd:$al:$sp:$ag";

//$gelen_veri .="\r\n";
//file_put_contents("izleme.txt",$gelen_veri,FILE_APPEND)


if(file_put_contents("veri.txt",$ekran)){
	echo "ok";
}else{
	echo "error";
}
?>