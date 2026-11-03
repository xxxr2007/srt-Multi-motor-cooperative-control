[CmdletBinding()]
param([Parameter(Mandatory = $true)][string]$VaultPath)
$ErrorActionPreference = 'Stop'
$taskVault = (Resolve-Path -LiteralPath $VaultPath).ProviderPath
if (-not (Test-Path -LiteralPath (Join-Path $taskVault '.obsidian') -PathType Container)) { throw 'Not an opened Obsidian vault.' }
$taskNow = [DateTimeOffset]::Now
$taskToken = [Guid]::NewGuid().ToString('N')
$taskMarker = 'OBSIDIAN-AUTO-SYNC-' + $taskToken
$taskRelative = 'AI 项目笔记/' + $taskNow.ToString('yyyy') + '/' + $taskNow.ToString('yyyy-MM-dd-HHmmss') + '-自动同步测试-' + $taskToken.Substring(0, 8) + '.md'
$taskPath = Join-Path $taskVault $taskRelative
New-Item -ItemType Directory -Path (Split-Path -Parent $taskPath) -Force | Out-Null
$taskText = @"
---
title: "Obsidian 自动同步测试"
date: "$($taskNow.ToString('yyyy-MM-dd'))"
status: "测试中"
tags:
  - 同步测试
---

# Obsidian 自动同步测试

- 创建时间（含本机时区偏移）：$($taskNow.ToString('o'))
- 唯一标记：$taskMarker
- 目的：检查 Git 插件是否自动提交并上传。

请保持 Obsidian 打开此库，等待约五分钟后核对远程内容。
本测试不手动提交或推送。文件中状态不会由验证脚本自动修改。
"@
$taskEncoding = New-Object System.Text.UTF8Encoding($false)
$taskBytes = $taskEncoding.GetBytes($taskText)
$taskStream = [System.IO.File]::Open($taskPath, [System.IO.FileMode]::CreateNew)
try { $taskStream.Write($taskBytes, 0, $taskBytes.Length) } finally { $taskStream.Dispose() }
Write-Output "File: $taskPath"
Write-Output "RelativePath: $taskRelative"
Write-Output "Marker: $taskMarker"
Write-Output 'No commit or push performed. Keep Obsidian open and verify after about five minutes.'
