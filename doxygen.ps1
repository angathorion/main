Set-StrictMode -version 2

# Modified from
# http://www.howtogeek.com/tips/how-to-extract-zip-files-using-powershell/
function UnzipFile($file, $destination) {
	mkdir $destination >$null
	$file = (Resolve-Path $file)[0].Path
	$destination = (Resolve-Path $destination)[0].Path

	Write-Progress ('Extracting ' + $file + ' => ' + $destination)

	$shell = new-object -com shell.application
	$zip = $shell.NameSpace($file)
	foreach ($item in $zip.items()) {
		$shell.Namespace($destination).copyhere($item)
	}
}

$doxygen_dir = 'doxygen'
$doxygen_zip = $doxygen_dir + '.zip'
$doxygen_exists = Test-Path $doxygen_dir
if ($doxygen_exists -ne $true) {
	Invoke-WebRequest ftp://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8.windows.bin.zip -OutFile $doxygen_zip
	UnzipFile $doxygen_zip $doxygen_dir
}

.\doxygen\doxygen.exe .\Doxyfile 2>&1 | %{
	if ($_.gettype().Name -eq "ErrorRecord") {
		if ($_ -match 'warning:') {
			$Host.UI.WriteWarningLine($_)
		} else {
			$Host.UI.WriteErrorLine($_)
		}
	} else {
		echo $_
	}
}
if ($LastExitCode -ne 0) {
	Throw "doxygen completed with errors."
}

$documentation_dir = 'docs'
$github_pages_dir = 'you_documentation'
if ($env:YOU_DOCUMENTATION_KEY -and
	($env:APPVEYOR_REPO_NAME -eq 'cs2103aug2014-w10-1c/main') -and # only main
	($env:APPVEYOR_REPO_BRANCH -eq 'master')) {
	&git clone git@github.com:cs2103aug2014-w10-1c/you-documentation.git $github_pages_dir --quiet

	Remove-Item $github_pages_dir -Exclude *.git -Recurse
	pushd $github_pages_dir
	move ('..\' + $documentation_dir + '\html\*') '.'
	&git add --all
	&git commit -m 'Latest documentation'
	&git push

	popd
}
