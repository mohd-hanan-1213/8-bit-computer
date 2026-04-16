# 🏗️ Architecture of the 8-bit Computer

---

## 🧠 Overview

This system implements a simple CPU architecture where instructions are executed through coordinated data transfer between registers, ALU, and memory under the control of a control unit.

---

## 🔧 Main Components

### Program Counter (PC)

* Holds the address of the next instruction
* Increments after each fetch

---

### Memory

* Stores instructions and data
* Provides data to the system via the bus

---

### Instruction Register (IR)

* Holds the current instruction
* Splits into:

  * Opcode (operation)
  * Operand (address)

---

### Control Unit (Arduino)

* Decodes instructions
* Generates control signals
* Manages timing (T0–T4 steps)

---

### Registers

* Temporary storage for data
* Includes:

  * Accumulator (primary register)
  * B register (for ALU operations)

---

### ALU (Arithmetic Logic Unit)

* Performs arithmetic operations:

  * ADD
  * SUB
* Outputs result to accumulator

---

### Output Register

* Stores final result
* Sends output to display

---

## 🔄 Execution Cycle

### T0 (Fetch Step 1)

* PC → Bus
* MAR ← Bus

### T1 (Fetch Step 2)

* RAM → Bus
* IR ← Bus
* PC increment

---

### Instruction Execution

#### LDA

* T2: Address → MAR
* T3: RAM → Accumulator

#### ADD

* T2: Address → MAR
* T3: RAM → B register
* T4: ALU result → Accumulator

#### SUB

* T2: Address → MAR
* T3: RAM → B register
* T4: ALU (subtraction) → Accumulator

#### STA

* T2: Address → MAR
* T3: Accumulator → RAM

#### OUT

* T2: Accumulator → Output register

#### NOP

* No operation

#### HLT

* Stops clock and execution

---

## ⚡ Control Signals

Control signals coordinate all operations:

* Data transfer (Bus control)
* Register load/enable
* ALU operation selection
* Memory read/write
* Program counter increment

(Refer to `docs/` for full control signal definitions)

---

## 🧩 Summary

This project demonstrates how a basic CPU operates internally using simple components and timing control, making it an effective educational model for understanding computer architecture.
