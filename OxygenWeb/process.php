<html>
<head>
	<title> oxygenWeb Connect </title>
    <link rel="stylesheet" type="text/css" href="style.css">
</head>
	<body>
		<h1>oxygenWeb Connect</h1>
		<?php
		include "db_connect.php";	
		ini_set('display_errors',0);
        error_reporting(E_ALL);
		
		$username = $_REQUEST['Username'];
		$password = $_REQUEST['password'];
        $code = $_REQUEST['com'];
        
        if($code == 'START'){
            $sql = ("SELECT UName,vmLimit,ID FROM Users where UName = '$username'");
            $result = $dbcon->query($sql);
            if ($result->num_rows > 0){
            while($row = $result->fetch_assoc()) {
                echo "<p>Welcome to Oxygen, <b>".$row['UName']."</b></p>";
                $limit = $row['vmLimit'];
                echo "<p>VMID $password sent to oxygenCore.</p>";
                $create = "cd /Users/oxygen/Documents/Oxygen/oxygenData && echo VM Starting && sudo ./Oxygen -s ".$row['ID']." ".$password."";
                
                include('Net/SSH2.php');

                $ssh = new Net_SSH2('localhost',22); //starting the ssh connection to localhost
                if (!$ssh->login('root', 'Oxygen000001')) { //if you can't log on...
                    exit('SERVER ERROR');
                }
                $output = $ssh->exec($create);
                echo $output;
                }
            }
            }
		
        if($password == 'e9eb377'){
            $sql = ("SELECT UName,vmLimit,ID FROM Users where UName = '$username'");
            $result = $dbcon->query($sql);
            if ($result->num_rows > 0){
            while($row = $result->fetch_assoc()) {
                echo "<p>Welcome to Oxygen, <b>".$row['UName']."</b></p>";
                echo "<p>Your Account Is Limited -> <b>".$row['vmLimit']."</b> VMs Allowed.</p>";
                $limit = $row['vmLimit'];
                if($limit==0){
                    echo "<p>You are not permitted to create VMs</p>";
                    echo "<p>Contact Oxygen Admin for manual approval</p>";
                } else{
                    $vmid = mt_rand(1000,9999);
                    echo "<p>VMID $vmid sent to oxygenCore.</p>";
                    $com = "Creating iOS VM!".$vmid."!";
                    system($com);
                    $create = "cd /Users/oxygen/Documents/Oxygen/oxygenData && echo VM Initialised && sudo ./Oxygen -c ".$row['ID']." ".$vmid." 0 0";
                    
                    include('Net/SSH2.php');

                    $ssh = new Net_SSH2('localhost',22); //starting the ssh connection to localhost
                    if (!$ssh->login('root', 'Oxygen000001')) { //if you can't log on...
                        exit('ssh Login Failed');
                    }
                    $output = $ssh->exec($create);
                    echo $output;
                    
                    echo "Hit start and lets celebrate!";
                }
            }
            }
            $dbcon->close();
        } else{
            echo "<p>Invalid Access Key</p>";
            exit();
        }
		?>

        <button onclick="location.href='/svm.php'" type="button">Start VM</button>
    
	</body>
</html>
