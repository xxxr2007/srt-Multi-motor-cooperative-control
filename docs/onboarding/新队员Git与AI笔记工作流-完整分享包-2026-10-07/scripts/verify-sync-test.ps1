[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$VaultPath,
    [Parameter(Mandatory = $true)][string]$RelativePath,
    [Parameter(Mandatory = $true)][string]$Marker,
    [string]$GitPath = 'git'
)
$ErrorActionPreference = 'Stop'
$taskOriginalPrompt = $env:GIT_TERMINAL_PROMPT
$taskOriginalInteractive = $env:GCM_INTERACTIVE
try {
$env:GIT_TERMINAL_PROMPT = '0'
$env:GCM_INTERACTIVE = 'never'
$taskVault = (Resolve-Path -LiteralPath $VaultPath).ProviderPath
$taskNote = $RelativePath.Replace([char]92, [char]47)
if ([System.IO.Path]::IsPathRooted($taskNote) -or ($taskNote.Split('/') -contains '..')) { throw 'Use a vault-relative note path without parent traversal.' }
if (-not (Test-Path -LiteralPath (Join-Path $taskVault $taskNote) -PathType Leaf)) { throw 'Test note not found.' }
$taskTop = & $GitPath -C $taskVault rev-parse --show-toplevel
if ($LASTEXITCODE -ne 0 -or [System.IO.Path]::GetFullPath($taskTop).TrimEnd([char]92) -ne $taskVault.TrimEnd([char]92)) { throw 'Vault must be the independent repository root.' }
$taskBranch = & $GitPath -C $taskVault branch --show-current
if ($LASTEXITCODE -ne 0 -or -not $taskBranch) { throw 'No checked-out branch.' }
$taskRemote = & $GitPath -C $taskVault remote get-url origin
if ($LASTEXITCODE -ne 0) { throw 'Missing origin remote.' }
$taskHead = & $GitPath -C $taskVault rev-parse HEAD 2>$null
if ($LASTEXITCODE -ne 0) { Write-Output 'NOT_COMMITTED: repository has no commits.'; return }
$taskLocal = & $GitPath -C $taskVault show "${taskHead}:$taskNote" 2>$null
$taskCommitted = $LASTEXITCODE -eq 0 -and (($taskLocal -join "`n").Contains($Marker))
Write-Output "Local HEAD: $taskHead"
Write-Output "Marker in local HEAD: $taskCommitted"
& $GitPath -C $taskVault -c core.quotepath=false status --short -- $taskNote
if (-not $taskCommitted) { Write-Output 'NOT_COMMITTED: marker not found in local HEAD.'; return }
$taskUpstream = & $GitPath -C $taskVault rev-parse --abbrev-ref --symbolic-full-name '@{upstream}' 2>$null
if ($LASTEXITCODE -ne 0 -or -not $taskUpstream.StartsWith('origin/')) { Write-Output 'UNVERIFIED: missing upstream on origin. Check git branch -vv.'; return }
$taskRemoteBranch = $taskUpstream.Substring('origin/'.Length)
$taskRemoteRef = & $GitPath -c http.lowSpeedLimit=1 -c http.lowSpeedTime=20 ls-remote $taskRemote "refs/heads/$taskRemoteBranch"
if ($LASTEXITCODE -ne 0) { Write-Output 'UNVERIFIED: live remote query failed; check network and authentication.'; return }
if (-not $taskRemoteRef) { Write-Output 'NOT_ON_REMOTE: upstream branch missing on remote.'; return }
$taskRemoteHead = ($taskRemoteRef -split '\s+')[0]
Write-Output "Live remote HEAD: $taskRemoteHead"
& $GitPath -C $taskVault cat-file -e "${taskRemoteHead}^{commit}" 2>$null
if ($LASTEXITCODE -ne 0) { Write-Output 'UNVERIFIED: live remote commit unavailable locally. Inspect the file on GitHub.'; return }
$taskRemoteContent = & $GitPath -C $taskVault show "${taskRemoteHead}:$taskNote" 2>$null
if ($LASTEXITCODE -eq 0 -and (($taskRemoteContent -join "`n").Contains($Marker))) {
    Write-Output 'SYNC_VERIFIED: live remote commit contains the test note and unique marker.'
} else { Write-Output 'NOT_ON_REMOTE: live remote commit does not contain the test marker.' }
Write-Output 'Read-only check completed. No commit, push or fetch performed.'
} finally {
    $env:GIT_TERMINAL_PROMPT = $taskOriginalPrompt
    $env:GCM_INTERACTIVE = $taskOriginalInteractive
}
