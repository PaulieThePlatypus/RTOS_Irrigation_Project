# RTOS_Project
We implement the  irrigation system as described belowed:
- folder project is for the simplest way to implement FSM
- folder project0 is for O(1) Cooperative Scheduler
# Description
An agricultural irrigation system needs to perform the following 5 tasks, for each irrigation session:
- Open fertilizer mixing Valve 1 for 5 seconds
-nClose Valve 1, open fertilizer mixing Valve 2 for 5 seconds
- Close Valve 2, open fertilizer mixing Valve 3 for 5 seconds
- Close valve 3, open pump 1 for 10 seconds
- Turn off pump 1, turn on pump 2 for 30 seconds.
- Finally, turn off pump 2.
At the end of this process, pump 2 will turn off. Suppose that the next 60 seconds the above process will be repeated
