<?php
	session_start();
	$_SESSION['id'] = 'yahya';
	$_SESSION['login'] = 'yez-zain';
    echo "<html>";
    echo "<body>";
    echo "<h1>";
    echo "Hello world";
    echo "</h1>";
    echo "</body>";
    echo "</html>";
	echo getcwd()."\n";
	echo '---------$_GET\n';
	print_r($_GET);
	echo '---------$_POST\n';
    print_r($_POST);
	echo '----------$_ENV\n';
	print_r($_ENV);
	echo '-------$_REQUEST\n';
	print_r($_REQUEST);
	echo '----------$_SERVER\n';
	print_r($_SERVER);
	echo '----------$_SESSION\n';
    print_r($_SESSION);
    echo '----------$_COOKIE\n';
    print_r($_COOKIE);
    echo '----------$_FILES\n';
    print_r($_FILES);
?>
