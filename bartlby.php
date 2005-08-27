<?
$cfgfile="/storage/SF.NET/BARTLBY/bartlby/bartlby.cfg";
//$btl=bartlby_open();
//$info=bartlby_get_info($btl);
//var_dump($info);
//var_dump(bartlby_get_service($cfgfile, 1));
//var_dump(bartlby_get_info($cfgfile, 1));

$info=@bartlby_get_info($cfgfile, 1);
if(!$info) {
	echo "NOT RUNN\n";
	exit;	
}
for($x=0; $x<$info[services]; $x++) {
	$svc=bartlby_get_service($cfgfile, $x);
	echo $svc[server_name] . ":" . $svc[client_port] . "/" . $svc[service_name] . " - " . $svc[new_server_text] . "\n";	
}
for($x=0; $x<$info[workers];$x++) {
	$wrk=bartlby_get_worker($cfgfile, $x);
	echo "\t" . $wrk[mail] . "\n";	
}

?>
