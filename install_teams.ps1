# 
    .Synopsis Downloads and installs the latest version of Microsoft Teams. 
    .NOTES 
      TODO: Prevent Teams Firewall Popup 
      https://thoor.tech/2018/11/microsoft-teams-firewall-prompt/ 
#> 
 
$LogPath = "$env:WINDIR\Temp\Install-Teams.ps1.log" 
Start-Transcript -Path $LogPath 
$ec = 1 
 
$TeamsInstall = Get-Package 'Teams Machine-Wide Installer' -ErrorAction SilentlyContinue 
$TeamsInstall | Format-Table 
if ($TeamsInstall) { 
  Write-Host "Skipping installation as Teams version $($TeamsInstall.Version) was already installed." 
  $ec = 0 
} 
else { 
 
  Write-Host 'Create work directory.' 
  $folder = "$env:temp\Teams" 
  New-Item -Path $folder -ItemType Directory -ErrorAction SilentlyContinue 
  Push-Location $folder 
 
  Write-Host 'Download the installer.' 
  Invoke-WebRequest -Uri 'https://teams.microsoft.com/downloads/desktopurl?env=production&plat=windows&download=true&managedInstaller=true&arch=x64' -OutFile 'Teams_windows_x64.msi' 
 
  Write-Host 'Install' 
  msiexec.exe '/i' 'Teams_windows_x64.msi' '/qn' 'OPTIONS="noAutoStart=true"' | Out-String 
  $ec = $LASTEXITCODE 
  Write-Host "Teams install Exit Code: $ec" 
 
 
  Write-Host 'Cleanup.' 
  Pop-Location 
  Remove-Item $folder -Recurse 
 
  Write-Host 'Complete.' 
} 
 
Stop-Transcript 
exit $ec
