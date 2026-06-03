# WindowsPrivilegeEscalationVulnerabilityDisplayBox

**Language**: [English](#English) | [中文](#中文)

---

## English

### What is WindowsPrivilegeEscalationVulnerabilityDisplayBox?

Windows Privilege Escalation Vulnerability Display Box is a Windows executable (.exe) program designed to detect and demonstrate privilege escalation vulnerabilities for users.
Trust me, WindowsPrivilegeEscalationVulnerabilityDisplayBox is strictly meant for demonstration purposes.

### Overview

* **Platform:** Windows
* **Type:** Privilege Escalation Demonstration Tool
* **Purpose:** Security Research / Educational Use
* **Language:** C++

### What is a Privilege Escalation Vulnerability?

A privilege escalation vulnerability is:
A bug within a system, program, service, or driver that allows an attacker to gain high-level privileges they should not otherwise possess.
**Key point:**
It is not about "already having permissions," but rather "getting permissions you shouldn't have."

### Currently Implemented Features

* Modify PowerShell execution policy
> If running other features throws an error, please try using this feature to change the PowerShell execution policy to Bypass first!
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Take ownership of files/folders as Administrator
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Elevate privileges to SYSTEM
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Elevate privileges to TrustedInstaller
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Check the current permissions of this program
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Elevate the permissions of this program to TrustedInstaller
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Forcefully enable the Administrator account (compatible with Windows 7, 8, 8.x, 10, and 11; supports Home editions)
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Grant this account full control permissions for a specified folder
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Print all privileged processes
> Currently unable to distinguish between TI and SYSTEM privileges.
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested

### Features Currently Being Implemented

* UAC bypass to Administrator without triggering prompts
> Experimental only, not universal, requires disabling antivirus software when using.
* Support for English
> AI translated, expressions might not be 100% accurate.
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Add the ability to launch specified programs with the current program's privileges
> Meaning if the program has Administrator privileges, it launches the specified program with those privileges; the same applies to TI privileges.<br>
> Currently does not support SYSTEM privileges.
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Support for X86_i386 architecture
> Please download the dedicated X86_i386 version from the releases page.
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Support for ARM64 architecture
> Please download the dedicated ARM64 version from the releases page.
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested
* Full support for Windows 7, 8, 8.x
> Untested
* Safer privilege escalation strategy (Try local escalation first, then fallback to cloud escalation)
> <br>Passed testing on<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> Other versions untested

### Regarding the Upcoming V6 Release

The V6 version will be divided into Pro and Lite versions. The Pro version will feature a GUI, while the Lite version will not, but this does not mean the Pro version will be a paid release.
I think the only difference between them is that the Lite version will receive updates faster, while the Pro version will update slower.

### Have your own ideas?

Feel free to submit Issues!

### Found a BUG?

Feel free to submit Issues!

### License and Thoughts

This project complies with the **GPL v3 open-source license**.
The original intention of this tool is to make privilege escalation simple and easy to demonstrate. I am well aware that there are people on GitHub who have researched this more thoroughly and perfectly than me, and who have better solutions.
I will keep working hard to learn! ~~~~

### Disclaimer

> This project is intended for educational and security research purposes only. The main purpose is to help understand and prevent related security issues.
> Any misuse or illegal use of this project is strictly prohibited. The user takes full responsibility for any consequences resulting from its use.

---

## 中文

### 什么是WindowsPrivilegeEscalationVulnerabilityDisplayBox？

WindowsPrivilegeEscalationVulnerabilityDisplayBox是一个Windows可执行（exe）程序，为用户检测、演示Windows里面的提权漏洞。
相信我，WindowsPrivilegeEscalationVulnerabilityDisplayBox是用来演示的。

### 概述

* **平台：** Windows
* **类型：** 提权演示工具
* **目的：** 安全研究 / 教育用途
* **语言：** C++

### 提权漏洞是什么？

提权漏洞就是：
系统、程序、服务、驱动里存在 bug，让攻击者不该有高权限，却能拿到高权限。
**关键点：**
不是“本来就有权限”，而是“不该有却拿到了”。

### 目前已实现的功能

* 更改PowerShell运行策略
> 如果运行其他功能报错，请优先尝试用此功能把powershell运行策略改至Bypass！
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 获取以Administrator接管文件/文件夹的功能
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 提权至SYSTEM
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 提权至TrustedInstaller
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 检测本程序的权限
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 将本程序提权到TrustedInstaller
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 强制打开Administrator账户（支持Windows 7、8、8.x、10、11，支持Home系列）
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 让此账户获取指定文件夹的完全控制权限
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 打印所有特权进程
> 暂时无法区分TI和SYSTEM特权
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试

### 正在实现的功能

* 不惊动UAC的情况下提权到Administrator
> 仅实验性，无法通杀，使用时需要关闭杀毒软件
* 支持英文
> AI翻译，表意可能不准
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 添加以程序当前权限启动指定程序功能
> 意思是如果程序是administrator权限，就用该权限启动指定程序，TI权限亦然<br>
> 暂不支持SYSTEM权限
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 支持X86_i386架构
> 请下载release页面X86_i386专版
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 支持ARM64架构
> 请下载release页面ARM64专版
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试
* 完全支持Windows 7 8 8.x
> 未测试
* 更加安全的提权策略(先尝试本地提权再尝试云端提权)
> <br>在<br>
> Windows 11 Pro 25H2 26200.6584 <br>
> Windows 10 Enterprise LTSC 21H2 19044.1288<br>
> 测试通过 其他未测试

### 对于即将发布的V6版本

V6版本将分为Pro和Lite版本，Pro版本拥有GUI，Lite没有，但这并不意味着Pro版本要收费。
我想他们唯一的区别就是Lite更新的快，Pro更新的慢吧。

### 有自己的想法吗？

欢迎提交Issues！

### 发现BUG了吗？

欢迎提交Issues！

### 开源协议与结语

本项目遵守 **GPL v3 开源协议**。
这个工具的初衷就是让提权简单化，易于演示。我也深知GitHub上也有比我研究的更彻底、更完美的人，他们有更好的方案。
我也会努力学习滴~~~~

### 免责声明

> 本项目仅用于安全研究与漏洞演示，主要目的是帮助理解和预防相关安全问题。
> 请勿将其用于任何非法用途，使用者需自行承担由此产生的风险。