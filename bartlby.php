<?
$cfgfile="/storage/SF.NET/BARTLBY/bartlby-core/bartlby.cfg";
//$btl=bartlby_open();
//$info=bartlby_get_info($btl);
//var_dump($info);
//var_dump(bartlby_get_service($cfgfile, 1));
//var_dump(bartlby_get_info($cfgfile, 1));

$info=@bartlby_get_info($cfgfile);
if(!$info) {
	echo "NOT RUNN\n";
	exit;	
}
echo "bartlby_version() \n";
var_dump(bartlby_version());
echo "\n";


$info=bartlby_get_info($cfgfile);
echo "bartlby_get_info()\n";
var_dump($info);

$svc=bartlby_get_service($cfgfile,1);
echo "bartlby_get_service()\n";
var_dump($svc);

$wrk=bartlby_get_worker($cfgfile, 1);
echo "bartlby_get_worker()\n";
var_dump($wrk);


$cfg=bartlby_config($cfgfile, "data_library");
echo "bartlby_config()\n";
var_dump($cfg);


$lib=bartlby_lib_info($cfgfile);
echo "bartlby_lib_info()\n";
var_dump($lib);

$add_server=bartlby_add_server($cfgfile, "Name", "127.0.0.1", "9030");
echo "bartlby_add_server()\n";
var_dump($add_server);

$mod_server=bartlby_modify_server($cfgfile, $add_server, "Modified Name", "127.0.0.1", "9030");
echo "bartlby_modify_server()\n";
var_dump($mod_server);


$get_server=bartlby_get_server_by_id($cfgfile, $add_server);
echo "bartlby_get_server_by_id()\n";
var_dump($get_server);


$del_server=bartlby_delete_server($cfgfile,$add_server);
echo "bartlby_delete_server()\n";
var_dump($del_server);
?>
