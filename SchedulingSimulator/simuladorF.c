#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct process
{
    int id;
    int burst_time;
    int arrival_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    int throughput_time;
    int response_time;
    int remaining_time;
    int probability_interrupt;
    bool is_completed;
} Process;

Process* generate_processes(int n, int io_bound_percentage, int *total_burst_time)
{
    srand(time(NULL));
    Process *processes = (Process *) malloc(n * sizeof(Process));

    int num_io_bound = n * io_bound_percentage / 100;
    int num_cpu_bound = n - num_io_bound;
    printf("%d Procesos I/O y %d Procesos CPU\n", num_io_bound, num_cpu_bound);

    for (int i = 0; i < n; i++)
    {
        processes[i].id = i + 1;
        processes[i].burst_time = rand() % 100 + 1; // Random burst time between 1 and 100
        *total_burst_time += processes[i].burst_time;
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].arrival_time = rand() % 100; // Random arrival time between 0 and 99
        processes[i].is_completed = false;

        if (i < num_io_bound)
        {
            processes[i].probability_interrupt = 90; // I/O-bound process
        }
        else
        {
            processes[i].probability_interrupt = 10; // CPU-bound process
        }
    }

    return processes;
}

void FIFO()
{
    int n, io_bound;
    printf("Enter the number of processes and percentage of I/O-Bounds: ");
    scanf("%d %d", &n, &io_bound);

    int total_burst_time = 0; 
    Process *processes = generate_processes(n, io_bound, &total_burst_time);

    srand(time(0));
    int completed = 0;
    int current_time = 0;
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_response_time = 0;
    int total_interrupts = 0;
    double total_time = 0;
    int interrupt_duration = 1; // you decide

    while (completed != n)
    {
        int idx = -1;

        for (int i = 0; i < n; i++)
        {
            if (!processes[i].is_completed && processes[i].arrival_time <= current_time)
            {
                idx = i;
                break;
            }
        }

        if (idx != -1)
        {
            processes[idx].start_time = current_time;

            if (rand() % 100 < processes[idx].probability_interrupt)
            {
                interrupt_duration = rand() % 10 + 1;
                total_interrupts++;
                current_time += interrupt_duration;
                total_burst_time += interrupt_duration;
            }
            else
            {
                processes[idx].is_completed = true;
                completed++;
                current_time += processes[idx].burst_time;

                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;

                total_turnaround_time += processes[idx].turnaround_time;
                total_waiting_time += processes[idx].waiting_time;
                total_response_time += processes[idx].response_time;
                total_time += processes[idx].completion_time;
            }
        }
        else
        {
            current_time++;
        }
    }

    printf("\nScheduler report:\n");
    puts("\t+-----+------------+-------------+------------+-----------------+-------------------+--------------+---------------+");
    puts("\t| PID | Burst Time | Arrive Time | Start Time | Completion Time |  Turnaround Time  | Waiting Time | Response Time |");
    puts("\t+-----+------------+-------------+------------+-----------------+-------------------+--------------+---------------+");

    /* itera las veces del numero de procesos y genera los datos en formato */
    for (int i = 0; i < n; i++)
    {
        printf("\t| %2d  |     %3d    |     %3d     |    %3d     |     %3d         |      %3d          |      %3d     |      %3d      |\n",
               processes[i].id, processes[i].burst_time, processes[i].arrival_time, processes[i].start_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time, processes[i].response_time);

        puts("\t+-----+------------+-------------+------------+-----------------+-------------------+--------------+---------------+");
    }

    puts("\n");

    double throughput = (double)total_burst_time / n;

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Throughput Time:  %.2f\n", throughput);
    printf("Average Response Time: %.2f\n", total_response_time / n);
    printf("Total Interrupts: %d\n", total_interrupts);
    free(processes);
}

void RR()
{

    int n, io_bound;
    printf("Enter the number of processes and percentage of I/O-Bounds: ");
    scanf("%d %d", &n, &io_bound);

    int total_burst_time;
    Process *processes = generate_processes(n, io_bound, &total_burst_time);;

    int time_quantum;
    printf("Enter the time quantum: ");
    scanf("%d", &time_quantum);

    srand(time(0));
    int completed = 0;
    int current_time = 0;
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_response_time = 0;
    int total_interrupts = 0;
    double total_time = 0;
    int interrupt_duration = 0; // arbitrario

    while (completed != n)
    {
        bool all_waiting = true;

        for (int i = 0; i < n; i++)
        {
            if (!processes[i].is_completed && processes[i].arrival_time <= current_time)
            {
                all_waiting = false;
                if (processes[i].remaining_time == processes[i].burst_time)
                {
                    processes[i].start_time = current_time;
                }

                if (processes[i].remaining_time > time_quantum)
                {
                    if (rand() % 100 < processes[i].probability_interrupt)
                    {
                        interrupt_duration = rand() % 10 + 1;
                        total_interrupts++;
                        current_time += interrupt_duration;
                        total_burst_time += interrupt_duration;
                    }
                    else
                    {
                        current_time += time_quantum;
                        processes[i].remaining_time -= time_quantum;
                    }
                }
                else
                {
                    if (rand() % 100 < processes[i].probability_interrupt)
                    {
                        interrupt_duration = rand() % 10 + 1;
                        total_interrupts++;
                        current_time += interrupt_duration;
                        total_burst_time += interrupt_duration;
                    }
                    else
                    {
                        current_time += processes[i].remaining_time;
                        processes[i].remaining_time = 0;

                        processes[i].is_completed = true;
                        completed++;

                        processes[i].completion_time = current_time;
                        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                        processes[i].response_time = processes[i].start_time - processes[i].arrival_time;

                        total_turnaround_time += processes[i].turnaround_time;
                        total_waiting_time += processes[i].waiting_time;
                        total_response_time += processes[i].response_time;
                        total_time += processes[i].completion_time;
                    }
                }
            }
        }

        if (all_waiting)
        {
            current_time++;
        }
    }

    double throughput = total_time / n;

    printf("\nScheduler report:\n");
    printf("ID\tBT\tAT\tST\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].burst_time, processes[i].arrival_time,
               processes[i].start_time, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time, processes[i].response_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Throughput Time:  %.2f\n", throughput);
    printf("Average Response Time: %.2f\n", total_response_time / n);
    printf("Total Interrupts: %d\n", total_interrupts);
    free(processes);
}

void SJF()
{
    int n, io_bound;
    printf("Enter the number of processes and percentage of I/O-Bounds: ");
    scanf("%d %d", &n, &io_bound);

    int total_burst_time;
    Process *processes = generate_processes(n, io_bound, &total_burst_time);;
 
    srand(time(0));
    int completed = 0;
    int current_time = 0;
    double total_turnaround_time = 0;
    double total_waiting_time = 0;
    double total_response_time = 0;
    double total_time = 0;
    int total_interrupts = 0;
    int interrupt_duration = 2;

    while (completed != n)
    {
        int idx = -1;
        int min_burst_time = 1e9;
        for (int i = 0; i < n; i++)
        {
            if (!processes[i].is_completed && processes[i].arrival_time <= current_time && processes[i].burst_time < min_burst_time)
            {
                idx = i;
                min_burst_time = processes[i].burst_time;
            }
        }

        if (idx != -1)
        {
            processes[idx].start_time = current_time;

            if (rand() % 100 < processes[idx].probability_interrupt)
            {
                interrupt_duration = rand() % 10 + 1;
                total_interrupts++;
                current_time += interrupt_duration;
                total_burst_time += interrupt_duration;
            }
            else
            {
                processes[idx].is_completed = true;
                completed++;
                current_time += processes[idx].burst_time;

                processes[idx].completion_time = current_time;
                processes[idx].turnaround_time = processes[idx].completion_time - processes[idx].arrival_time;
                processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
                processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;

                total_turnaround_time += processes[idx].turnaround_time;
                total_waiting_time += processes[idx].waiting_time;
                total_response_time += processes[idx].response_time;
                total_time += processes[idx].completion_time;
            }
        }
        else
        {
            current_time++;
        }
    }

    double throughput = (double)total_burst_time / n;

    printf("\nScheduler report:\n");
    printf("ID\tBT\tAT\tST\tCT\tTAT\tWT\tRT\n");
    for (int i = 0; i < n; i++)
    {
        printf("P%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", processes[i].id, processes[i].burst_time, processes[i].arrival_time,
               processes[i].start_time, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time, processes[i].response_time);
    }

    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / n);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / n);
    printf("Throughput Time:  %.2f\n", throughput);
    printf("Average Response Time: %.2f\n", total_response_time / n);
    printf("Total Interrupts: %d\n", total_interrupts);
    free(processes);
}

int main()
{
    int choice;

    do
    {
        printf("\n");
        puts("┏                                                                                                                             ┓");
        printf("SIMULADOR:");
        printf("\n Para escoger el algoritmo FIFO ingrese   1");
        printf("\n Para escoger el algoritmo RR ingrese   2");
        printf("\n Para escoger el algoritmo SJF ingrese    3");
        printf("\n Para salir ingrese     4");
        printf("\n Escoge una opcion");
        printf("\n\n\n");

        puts("┗                                                                                                                             ┛");
        printf("Eleccion: ");

        scanf("%d", &choice);
        (void)getchar();

        switch (choice)
        {
        case 1:
            FIFO();
            break;
        case 2:
            RR();
            break;
        case 3:
            SJF();
            break;
        case 4:
            break;

        default:
            printf("Error! Por favor intenta de nuevo");
        }

    } while (choice != 4);

    return 0;
}