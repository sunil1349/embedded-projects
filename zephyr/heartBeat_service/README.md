# Heartbeat Service — Zephyr RTOS

## Objective

This project implements a reusable periodic heartbeat service using a Zephyr software timer and workqueue.

The timer callback performs only minimal work and submits a work item. The system workqueue later executes the heartbeat processing in thread context.

## Architecture

```text
Application
    |
    | heartbeat_start()
    v
Software Timer
    |
    | timer expiry
    v
k_work_submit()
    |
    v
System Workqueue Thread
    |
    v
Heartbeat Processing