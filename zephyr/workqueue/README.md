# Workqueue Demo (Zephyr RTOS)

## Objective

This project demonstrates how to defer work from Interrupt Service Routine (ISR) context to thread context using Zephyr's Workqueue API.

Instead of performing time-consuming operations inside the GPIO interrupt handler, the ISR submits work to the system workqueue. The workqueue thread later executes the requested task.

This follows a common embedded firmware design where ISRs remain short and deterministic.

---

## RTOS Concepts

- GPIO Interrupts
- ISR Design
- Workqueue (`k_work`)
- Deferred Execution
- System Workqueue
- DeviceTree
- GPIO Driver

---

## Why Workqueues?

Interrupts should execute as quickly as possible.

Avoid doing the following inside an ISR:

- Long calculations
- SPI transactions
- I2C communication
- Flash writes
- Dynamic memory allocation
- Complex logging

Instead:

```
ISR
 |
 |  k_work_submit()
 |
 +------------------------+
                          |
                          v
                System Workqueue Thread
                          |
                          v
                  Execute Heavy Work
```



## APIs Used

### Workqueue

```c
k_work_init()

k_work_submit()
```

### GPIO

```c
gpio_pin_configure_dt()

gpio_pin_interrupt_configure_dt()

gpio_init_callback()

gpio_add_callback()

gpio_pin_toggle_dt()
```

---

## Execution Flow

1. Configure LED GPIO.
2. Configure Button GPIO.
3. Register GPIO interrupt callback.
4. Initialize work item.
5. Button interrupt occurs.
6. ISR submits work.
7. ISR exits immediately.
8. System workqueue thread executes the handler.
9. Handler toggles the LED and prints a message.

## Learning Outcomes

- Understand why ISRs must remain short.
- Learn deferred execution using `k_work`.
- Understand the relationship between interrupts and worker threads.
- Learn how Zephyr's system workqueue operates.
- Separate interrupt context from thread context.

---

## Real-World Applications

Workqueues are commonly used for:

- Button debouncing
- SPI display updates
- I2C sensor reads
- Flash memory writes
- BLE event processing
- Filesystem operations
- USB event handling
- Network packet processing

---

## Future Improvements

- Create a dedicated workqueue instead of using the system workqueue.
- Queue multiple work items.
- Use delayed work (`k_work_delayable`).
- Combine with message queues for event-driven applications.
- Add software debouncing using delayed work.