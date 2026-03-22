**Void Hunter – Modular Melee Combat System**
Unreal Engine 5 | C++ | Wave-Based Combat

---

### Overview

Void Hunter is a gameplay-focused project built to explore modular combat systems and wave-based encounters in Unreal Engine.

The goal of the project is to design clean, scalable gameplay code using C++, with a focus on **decoupled systems, reusable components, and reliable state handling**.

All gameplay logic and system design were implemented in C++, while visual/audio assets were integrated from external sources.

---

### Core Systems

**Damage System (Interface-Based)**

* Uses a custom damage interface to handle interactions between player, enemies, and environment
* Removes the need for hard casting between classes
* Keeps combat logic flexible and extensible

---

**Component-Based Architecture**

* Health Component: Handles health, death state, and event triggering
* Combat Component: Manages melee hit detection, hit-stop, and feedback systems
* Clear separation of responsibilities across systems

---

**Wave System (Data-Driven)**

* Custom wave structure for controlling enemy spawns
* Supports easy iteration and scaling directly from the editor
* Handles edge cases like failed spawns and simultaneous enemy deaths

---

### AI Behavior

* Two enemy types: melee and ranged
* Custom C++ logic for chasing, attacking, and maintaining distance
* Simple but reliable state-driven behavior system

---

### Technical Highlights

* Reliable state handling to prevent duplicate death events
* Timer-based wave control to avoid race conditions
* Clean communication between systems using components and interfaces
* Basic animation and VFX integration tied to gameplay events

---

### Development Info  
* Status: Completed (Core Systems)  
* Duration: March 1 – March 20, 2026 (~3 weeks)  
* Role: Solo Developer (Gameplay Programming & Systems Design)  
* Future Work: Boss fight, improved UI, expanded combat systems  
