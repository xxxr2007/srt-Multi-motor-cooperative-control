[CmdletBinding(SupportsShouldProcess = $true)]
param(
    [Parameter(Mandatory = $true)][string]$VaultPath,
    [string]$CodexDirectory,
    [switch]$Update
)
$ErrorActionPreference = 'Stop'
if (-not $CodexDirectory) {
    if ($env:CODEX_HOME) { $CodexDirectory = $env:CODEX_HOME }
    else { $CodexDirectory = Join-Path ([Environment]::GetFolderPath('UserProfile')) '.codex' }
}
$taskVault = (Resolve-Path -LiteralPath $VaultPath).ProviderPath
if (-not (Test-Path -LiteralPath (Join-Path $taskVault '.obsidian') -PathType Container)) {
    throw 'Open this folder as an Obsidian vault first.'
}
if (-not (Test-Path -LiteralPath (Join-Path $taskVault '.git'))) {
    $taskAncestor = Split-Path -Parent $taskVault
    while ($taskAncestor) {
        if (Test-Path -LiteralPath (Join-Path $taskAncestor '.git')) {
            throw 'Vault is inside another Git repository. Use an independent folder.'
        }
        $taskNext = Split-Path -Parent $taskAncestor
        if ($taskNext -eq $taskAncestor) { break }
        $taskAncestor = $taskNext
    }
}
$taskCodex = [System.IO.Path]::GetFullPath($CodexDirectory)
$taskSource = Join-Path $PSScriptRoot 'obsidian-project-notes'
$taskDestination = Join-Path $taskCodex 'skills\obsidian-project-notes'
$taskGlobal = Join-Path $taskCodex 'AGENTS.md'
$taskOverride = Join-Path $taskCodex 'AGENTS.override.md'
if ((Test-Path -LiteralPath $taskOverride) -and ((Get-Item -LiteralPath $taskOverride).Length -gt 0)) { $taskGlobal = $taskOverride }
$taskExisting = ''
if (Test-Path -LiteralPath $taskGlobal) { $taskExisting = [System.IO.File]::ReadAllText($taskGlobal) }
$taskInstalled = Test-Path -LiteralPath $taskDestination
if ($taskInstalled -and -not $Update) { throw 'Skill already installed. Use -Update after reviewing the package.' }
$taskBegin = '<!-- BEGIN OBSIDIAN PROJECT NOTES -->'
$taskEnd = '<!-- END OBSIDIAN PROJECT NOTES -->'
$taskBeginCount = [regex]::Matches($taskExisting, [regex]::Escape($taskBegin)).Count
$taskEndCount = [regex]::Matches($taskExisting, [regex]::Escape($taskEnd)).Count
if ($taskBeginCount -ne $taskEndCount -or $taskBeginCount -gt 1) { throw 'Malformed or duplicate workflow blocks; inspect global instructions.' }
if ($taskBeginCount -gt 0 -and -not $Update) { throw 'Workflow already configured. Use -Update.' }
$taskBlock = [System.IO.File]::ReadAllText((Join-Path $PSScriptRoot 'global-instructions.md')).TrimEnd()
if ($taskBeginCount -eq 1) {
    $taskStart = $taskExisting.IndexOf($taskBegin)
    $taskFinish = $taskExisting.IndexOf($taskEnd)
    if ($taskFinish -lt $taskStart) { throw 'Workflow marker order invalid.' }
    $taskFinish += $taskEnd.Length
    $taskMerged = $taskExisting.Substring(0, $taskStart) + $taskBlock + $taskExisting.Substring($taskFinish)
} else { $taskMerged = ($taskExisting.TrimEnd() + "`r`n`r`n" + $taskBlock).TrimStart() }
$taskEncoding = New-Object System.Text.UTF8Encoding($false)
if ($taskEncoding.GetByteCount($taskMerged) -gt 24000) { throw 'Global instructions too large; merge manually.' }
$taskSettings = [System.IO.File]::ReadAllText((Join-Path $taskSource 'settings.json')) | ConvertFrom-Json
$taskOldSettings = Join-Path $taskDestination 'settings.json'
if ($taskInstalled -and (Test-Path -LiteralPath $taskOldSettings)) {
    $taskSettings = [System.IO.File]::ReadAllText($taskOldSettings) | ConvertFrom-Json
}
if (-not ($taskSettings.PSObject.Properties.Name -contains 'vault_path')) { throw 'Invalid settings: missing vault_path.' }
$taskSettings.vault_path = $taskVault
if (-not $PSCmdlet.ShouldProcess($taskCodex, 'Install or update Obsidian skill and workflow instructions')) { return }
$taskStamp = Get-Date -Format 'yyyyMMdd-HHmmssfff'
if (Test-Path -LiteralPath $taskGlobal) { Copy-Item -LiteralPath $taskGlobal -Destination ($taskGlobal + '.before-obsidian-v2-' + $taskStamp + '.bak') }
if ($taskInstalled) { Copy-Item -LiteralPath $taskDestination -Destination ($taskDestination + '.before-v2-' + $taskStamp) -Recurse }
New-Item -ItemType Directory -Path $taskDestination -Force | Out-Null
foreach ($taskName in @('SKILL.md', 'note-template.md')) {
    Copy-Item -LiteralPath (Join-Path $taskSource $taskName) -Destination (Join-Path $taskDestination $taskName) -Force
}
[System.IO.File]::WriteAllText((Join-Path $taskDestination 'settings.json'), ($taskSettings | ConvertTo-Json -Depth 10), $taskEncoding)
[System.IO.File]::WriteAllText($taskGlobal, $taskMerged, $taskEncoding)
Write-Output "Skill: $taskDestination"
Write-Output "Global instructions: $taskGlobal"
Write-Output "Vault: $taskVault"
Write-Output 'Installed locally. Start a new Codex chat. No files uploaded.'
