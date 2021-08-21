# Development of a mini operating system for x86 - Operating System course

## Run

```
make run
```

## Features

### Console
All control characters are implemented, you can change the font color and the background color,
and scrolling also works.

### Timer
A timer is displayed at the top right of the console.

### System calls
I have implemented the shutdown and write system calls.

### Processes
For the processes, I have implemented a collaborative system, the processes
themselves must call schedule(). To test the processes, I added 5 simple processes 
(they increment a counter and display it), the first one is started on launch.
