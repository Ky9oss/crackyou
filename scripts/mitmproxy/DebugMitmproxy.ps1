# Run this script from your project's root to debug a uv project with Python 3.14's pdb

uv python pin 3.14
Start-Process -FilePath "uv.exe" -ArgumentList "run mitmproxy" -PassThru | Out-Null
Start-Sleep -Milliseconds 2000

# process chanins: uv.exe -> mitmproxy.exe -> python.exe (PID:$1) -> python.exe (PID:$2)
$up = Get-CimInstance Win32_Process -Filter "Name = 'uv.exe' OR Name = 'uv'" | Select-Object -ExpandProperty ProcessId
$mp = Get-CimInstance Win32_Process -Filter "ParentProcessId = $up AND Name = 'mitmproxy.exe'" | Select-Object -ExpandProperty ProcessId
$pp = Get-CimInstance Win32_Process -Filter "ParentProcessId = $mp" | Select-Object -ExpandProperty ProcessId
$pp314 = Get-CimInstance Win32_Process -Filter "ParentProcessId = $pp" | Select-Object -ExpandProperty ProcessId
python -m pdb -p $pp314
