# Keyboard control for pca9685_servo_test (FT6325M, position servo)
#
# Usage (PowerShell):
#   powershell -ExecutionPolicy Bypass -File servo_keys.ps1 [-Port COM3]
#
# Close Arduino IDE Serial Monitor first (the port can be opened by one program only).
# Opening the port resets the board -> servo output starts OFF.

param(
  [string]$Port = 'COM3',
  [int]$Baud = 115200
)

[Console]::OutputEncoding = [Text.Encoding]::UTF8

$p = New-Object System.IO.Ports.SerialPort $Port, $Baud, 'None', 8, 'One'
$p.Encoding = [Text.Encoding]::UTF8
$p.Open()

Write-Host "Connected to $Port. Board resets now; servo output is OFF until you press a key."
Write-Host "  W / Up     : wind (fork up)   - hold to keep moving, release to hold position"
Write-Host "  S / Down   : unwind (fork down)"
Write-Host "  Space / X  : stop moving (hold position)"
Write-Host "  1 ~ 5      : speed level (200 ~ 1000 us/s)"
Write-Host "  O          : output OFF (servo goes limp - fork may drop)"
Write-Host "  ?          : status"
Write-Host "  Q / Esc    : quit"
Write-Host ""

$quit = $false
try {
  while (-not $quit) {
    if ($p.BytesToRead -gt 0) {
      Write-Host -NoNewline $p.ReadExisting()
    }

    if ([Console]::KeyAvailable) {
      $k = [Console]::ReadKey($true)
      $c = [char]::ToLower($k.KeyChar)

      if ($k.Key -eq 'UpArrow' -or $c -eq 'w') { $p.Write('w') }
      elseif ($k.Key -eq 'DownArrow' -or $c -eq 's') { $p.Write('s') }
      elseif ($c -eq ' ' -or $c -eq 'x') { $p.Write(' ') }
      elseif ($c -ge '1' -and $c -le '5') { $p.Write([string]$c) }
      elseif ($c -eq 'o') { $p.Write("o`n") }
      elseif ($c -eq '?') { $p.Write("?`n") }
      elseif ($k.Key -eq 'Escape' -or $c -eq 'q') { $quit = $true }
    }

    Start-Sleep -Milliseconds 10
  }
}
finally {
  if ($p.IsOpen) {
    $p.Write(' ')              # stop moving (servo keeps holding its position)
    Start-Sleep -Milliseconds 100
    $p.Close()
  }
  Write-Host "`nDisconnected."
}
