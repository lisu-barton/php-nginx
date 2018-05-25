<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('pcurl')) {
	dl('pcurl.' . PHP_SHLIB_SUFFIX);
}
$module = 'pcurl';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
foreach($functions as $func) {
    echo $func."$br\n";
}
echo "$br\n";

// php -r "dl('pcurl.so');var_dump(get_extension_funcs('pcurl'));"

// PHP Startup: Unable to load dynamic library so: undefined symbol: zend_new_interned_string in Unknown on line
?>

