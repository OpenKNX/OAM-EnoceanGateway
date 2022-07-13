$sourceName = "EnoceanGateway"
$targetName = $sourceName
$appRelease = "Release"

# check for working dir
if (Test-Path -Path release) {
    # clean working dir
    Remove-Item -Recurse release\*
} else {
    New-Item -Path release -ItemType Directory | Out-Null
}

# create required directories
New-Item -Path release/data -ItemType Directory | Out-Null

# get xml for kxnprod
~/bin/OpenKNXproducer.exe create --Debug --Output=release/EnoceanGateway.knxprod --HeaderFileName=src/EnoceanGateway.h src/EnoceanGateway.xml
if (!$?) {
    Write-Host "Error in knxprod, Release was not built!"
    exit 1
}
Move-Item src/EnoceanGateway.debug.xml release/data/EnoceanGateway.xml

# build firmware based on generated headerfile for SAMD
~/.platformio/penv/Scripts/pio.exe run -e build
if (!$?) {
    Write-Host "SAMD build failed, Release was not built!"
    exit 1
}
Copy-Item .pio/build/build/firmware.bin release/data/

# add necessary scripts
Copy-Item scripts/Readme-Release.txt release/
Copy-Item scripts/Build-knxprod.ps1 release/
Copy-Item scripts/Upload-Firmware*.ps1 release/

# cleanup
Remove-Item release/EnoceanGateway.knxprod

# calculate version string
$appVersion=Select-String -Path src/$sourceName.h -Pattern MAIN_ApplicationVersion
$appVersion=$appVersion.ToString().Split()[-1]
$appMajor=[math]::Floor($appVersion/16)
$appMinor=$appVersion%16
$appRev=Select-String -Path src/main.cpp -Pattern "const uint8_t firmwareRevision"
$appRev=$appRev.ToString().Split()[-1].Replace(";","")
$appVersion="$appMajor.$appMinor"
if ($appRev -gt 0) {
    $appVersion="$appVersion.$appRev"
}


# create package 
Compress-Archive -Path release/* -DestinationPath Release.zip
Remove-Item -Recurse release/*
Move-Item Release.zip "release/$targetName-$appRelease-$appVersion.zip"

Write-Host "Release $targetName-$appRelease-$appVersion successfully created!"
