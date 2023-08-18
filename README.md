# Multi-Level Feedback Queue Scheduling
## Round Robin (RR) ➔ Highest Response Ratio Next (HRRN) ➔ First-Come, First-Served (FCFS) Implementation in C++

This project showcases the culmination of an Operating Systems (OS) course, where we delve into the implementation of a Multi-Level Feedback Queue (MLFQ) scheduling algorithm. Our approach combines three scheduling techniques: Round Robin, Highest Response Ratio Next (HRRN), and First-Come, First-Served (FCFS). The entire project is coded using the C++ programming language.

## Project Highlights

- **Dispatch Latency**: 2
- **Quantum for Round Robin Queue**: 6

The input, neatly organized in the 'assets/input.txt' file, follows this structure:

```MarkDown
2   # Dispatch latency
6   # Quantum for RR queue
P0: 0,5,7,2      # Process P0: Arrival(0), Burst(5, 7, 2)
P1: 8,8,7,1,11,4 # Process P1: Arrival(8), Burst(8, 7, 1, 11, 4)
P2: 4,9          # Process P2: Arrival(4), Burst(9)
P3: 60,7,2,3     # Process P3: Arrival(60), Burst(7, 2, 3)
```


The resulting output is meticulously composed in 'assets/output.txt' file, and includes:

### Gantt Chart
```
-------------------------------------------
|P0  |P2  |P1  |P0  |P2  |P1  |P3  |P3  |
-------------------------------------------
     5    13   21   25   30   61   69   79
```


### Performance Metrics
```
- **CPU Utilization**: 82.2785%
- **Average Waiting Time**: 37
- **Average Turnaround Time**: 30
- **Average Response Time**: 3
```

## Project Execution and Visualization

The Gantt chart above vividly captures the project's progression, displaying the timeline for executing each process in the Multi-Level Feedback Queue.

Key performance metrics provide a comprehensive understanding of the algorithm's efficiency. CPU utilization showcases effective CPU usage, while average waiting, turnaround, and response times gauge scheduling effectiveness.

## Institution Insights

This venture is nurtured within an Operating Systems course at Jahrom University, under the guidance of Dr. Parseh. Jahrom University is renowned for promoting academic excellence and innovation.

For more details, visit: [jahromu.ac.ir](https://jahromu.ac.ir/en)
