# sussyOS

Welcome to **sussyOS** — a fully custom operating system built from the ground up. Designed for uniqueness and performance, sussyOS is an experimental project aiming to redefine what an operating system can be when unconstrained by legacy codebases and traditional architectures.

> 🚨 **NOTE**: sussyOS is under active development and is currently in a pre-alpha state. Major features, bootloader support, and package management are work-in-progress.

---

## 🚀 Project Vision

- ✅ Build a completely custom operating system (not based on Linux or Unix)
- 🛠️ Develop a functional GUI, CLI, and core OS kernel
- 📦 Add a built-in package manager (inspired by `vent`)
- 🔒 Closed-source with potential for open-source community editions

---

## 🧠 Features (Planned / WIP)

| Feature                | Status       |
|------------------------|--------------|
| Custom Bootloader      | ✅ Almost done |
| Kernel Development     | ✅ Started     |
| susScript Integration  | 🔲 Planned   |
| File System Support    | 🚧 In Progress   |
| GUI Interface          | 🔲 Planned     |
| Package Manager        | 🔲 Not Implemented |
| Installer (Windows UX) | ✅ Planned     |

---

## 📦 susScript Compatibility

> ⚠️ **This OS is not compadible with [susScript](https://github.com/sussyOS/susScript) yet!!!.**

sussyOS is natively compatible with **[susScript](https://github.com/sussyOS/susScript)** — a custom programming language built for this OS. susScript packages can be installed, interpreted, and compiled within sussyOS using the `sussy` compiler.

---

## 🛠️ Getting Started

> ⚠️ **This OS is not ready for real hardware or VM use yet.**  
> Development builds will be provided via GitHub Releases in the future.

### Prerequisites

- A working build of the `sussy` compiler
- Knowledge of low-level OS dev (C/C++/ASM)
- VirtualBox or QEMU for testing

### Cloning the Repository

```bash
git clone https://github.com/sussyOS/sussyOS.git
cd sussyOS
scons run
```
