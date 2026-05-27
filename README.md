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

### Features

* Modify PowerShell execution policy
* Take ownership of files and directories as Administrator
* Elevate privileges to SYSTEM
* Elevate privileges to TrustedInstaller
* Check the current permissions of this program
* Elevate the permissions of this program to TrustedInstaller
* Forcefully enable the Administrator account (compatible with Windows 7, 8, 8.x, 10, and 11; supports Home editions)
* Grant this account full control permissions for a specified folder
* Print all privileged processes

### Planned Features

* UAC bypass to Administrator
* Support for English
* Add the ability to launch programs with TrustedInstaller privileges

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
* 获取以Administrator接管文件/文件夹的功能
* 提权至SYSTEM
* 提权至TrustedInstaller
* 检测本程序的权限
* 将本程序提权到TrustedInstaller
* 强制打开Administrator账户（支持Windows 7、8、8.x、10、11，支持Home系列）
* 让此账户获取指定文件夹的完全控制权限
* 打印所有特权进程

### 正在实现的功能

* 不惊动UAC的情况下提权到Administrator
* 支持英文
* 添加以TrustedInstaller权限启动程序功能

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
