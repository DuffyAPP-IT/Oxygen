<html>
<head>
<title> oxygenWeb Connect </title>
<link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
        <h1>oxygenWeb - VM Control</h1>
        <h2>Authorised Users Only </h2>

        <form action="process.php" method="POST">
			
			<label> Username:
              <input type="text" name="Username" accesskey="u" required>
            </label>
			<label> VMID:
              <input type="password" name="password" accesskey="p" required>
            </label>
            <label> CODE:
              <input type="text" name="com" value="START" accesskey="p" readonly required>
            </label>
			
            <input value="Start (Connect Via SSH)" type="submit">
        </form>

</body>

</html>
