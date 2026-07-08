# Thread Safe Logger (Zephyr RTOS)

## Objective

This project demonstrates how to safely share a UART logging interface between multiple RTOS threads using a Zephyr mutex.

Instead of allowing every thread to access `printk()` directly, all logging is routed through a dedicated logger module that internally protects the UART using a mutex.

This mirrors how production embedded firmware centralizes logging while preventing message corruption.

---

## RTOS Concepts

- Zephyr Threads
- Mutex (`k_mutex`)
- Resource Protection
- Modular Driver Design
- Thread-safe Logging

---


## Folder Structure

```text
10-thread-safe-logger

includes/
    logger.h

src/
    logger.c
    main.c

CMakeLists.txt
prj.conf
README.md
```

---





## Learning Outcomes

- Protect shared resources from concurrent access
- Build reusable firmware components (logger)
- Design software using layered architecture

---

## Future Improvements

- Timestamp each log entry
- implement CLI to config system 
- implemeting CLI changeable logger frequency
- Asynchronous logger using a message queue