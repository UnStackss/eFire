<p align="center">
    <img src="https://i.imgur.com/41sRdEy.png" alt="eFire Logo" width="150">
</p>

# 🔥 eFire - Directory Scanner in Inferno Mode 🔥

[![Downloads](https://img.shields.io/github/downloads/UnStackss/eFire/total)](https://github.com/UnStackss/eFire/releases)
[![Forks](https://img.shields.io/github/forks/UnStackss/eFire)](https://github.com/UnStackss/eFire/forks)
[![Stars](https://img.shields.io/github/stars/UnStackss/eFire)](https://github.com/UnStackss/eFire/stargazers)

## 🚀 [Download eFire Now](https://github.com/UnStackss/eFire/releases/download/latest/eFire.exe)
> Direct download link for the latest release of eFire.

![eFire Screenshot](https://i.imgur.com/kJB3C1l.png)

## Overview
**eFire** is a powerful and interactive program that scans a specified directory, generates a structured YAML representation of its content, and uploads it to **Pastebin** for easy sharing. The program features:

- Animated loading bars 🔥
- Custom console icons (Windows only) 💻
- UTF-8 output support 📝
- User-friendly, interactive design 💀
- Option to skip Pastebin API key entry 🚫

## Features
- **Directory Scan**: Explore directories and visualize their structure.
- **YAML Export**: Save the directory structure in a readable YAML format.
- **Pastebin Upload**: Automatically upload the YAML file to Pastebin.
- **Console Animations**: Watch as progress bars and banners bring the console to life.

<p align="right">
  <img src="https://i.imgur.com/gxKEuMK.gif" alt="eFire Logo" width="150">
</p>

## Installation
You can either **download the program** directly from the **[Releases page](https://github.com/UnStackss/eFire/releases)**, or follow these steps to compile and run it locally.

### Option 1: Download from Releases
1. Go to the [Releases page](https://github.com/UnStackss/eFire/releases).
2. Download the latest release of the program for your operating system.
3. Extract and run the program.

### Option 2: Compile Locally
1. Clone or download the repository to your local machine.
2. Make sure to have a **Pastebin API key** for uploading files (optional).
3. Compile and run the program.

### Prerequisites
- C++ compiler with C++17 support
- Windows operating system (for console icon feature)
- **Pastebin API key** (optional for Pastebin upload)

```bash
git clone https://github.com/UnStackss/eFire.git
cd eFire
# Compile the program with your C++ compiler
g++ -o eFire eFire.cpp -lwininet
