# Semaphore Sync Solution

## Overview
This C++ program addresses a synchronization problem utilizing POSIX and "semaphore.h" libraries. It manages concurrent counting of incoming messages by mCounter threads, periodic retrieval and resetting of counts by mMonitor, and reading values from a buffer by mCollector thread. The program implements semaphores to handle race conditions, synchronization, and deadlock scenarios. Additionally, it randomizes thread activations to simulate real-world message arrivals.

## Problem Description
You are required to write a program to solve the following synchronization problem using POSIX and “semaphore.h” libraries.
N mCounter threads count independent incoming messages in a system and another thread mMonitor gets the count of threads at time intervals of size t1, and then resets the counter to 0. The mMonitor then places this value in a buffer of size b, and a mCollector thread reads the values from the buffer.
Any thread will have to wait if the counter is being locked by any other thread. Also, the mMonitor and mCollector threads will not be able to access the buffer at the same time or to add another entry if the buffer is full.
Assume that the messages come randomly to the system, this can be realized if the mCounter threads sleep for random times, and their activation (sleep time ends) corresponds to an email arrival. Similarly, the mMonitor and mCollector will be activated at random time intervals.

## Usage
1. Ensure you have a C++ compiler that supports POSIX and "semaphore.h" libraries installed on your system.
2. Clone this repository to your local machine.
3. Navigate to the directory containing the source code.
4. Compile the program manually with your C++ compiler.
   
