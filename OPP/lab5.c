#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <mpi.h>
#include <cstring>

#define DELTA 50000
#define LISTS_COUNT 10
#define TASK_COUNT 1000
#define MIN_TASKS_TO_SHARE 5

#define EXECUTOR_FINISHED_WORK -1
#define NO_TASKS_TO_SHARE -2
#define SENDING_TASKS 1
#define SENDING_TASK_COUNT 2

void init_task_set(int* task_set, int task_count, int iter_counter);
void exec_task_set(int* task_set);
void* receiver_start_routine(void* args);
void* executor_start_routine(void* args);

pthread_t threads[2];
pthread_mutex_t mutex;
int* tasks;

double maximum_difference;
int finished_execution = 0;

int size;
int rank;
int remaining_tasks;
int executed_tasks;
int additional_tasks;
int executed_tasks_current;
int additional_tasks_sum;
int all_start_tasks = 0;
int all_end_tasks = 0;
double global_res;

int main(int argc, char** argv) {
	int thread_support;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &thread_support);
	if (thread_support != MPI_THREAD_MULTIPLE) {
		printf("Init error");
		MPI_Finalize();
		return 0;
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	pthread_attr_t thread_attributes;
	pthread_mutex_init(&mutex, NULL);

	double start = MPI_Wtime();
	pthread_attr_init(&thread_attributes);
	pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_JOINABLE);
	pthread_create(&threads[0], &thread_attributes, receiver_start_routine, NULL);
	pthread_create(&threads[1], &thread_attributes, executor_start_routine, NULL);
	pthread_join(threads[0], NULL);
	pthread_join(threads[1], NULL);
	pthread_attr_destroy(&thread_attributes);
	pthread_mutex_destroy(&mutex);

	int start_tasks;
	int end_tasks;
	MPI_Reduce(&all_start_tasks, &start_tasks, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&all_end_tasks, &end_tasks, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0) {
		printf("Time: %lf\n", MPI_Wtime() - start);
		printf("Maximum difference: %lf\n", maximum_difference);
		printf("All start tasks: %d\n", start_tasks);
		printf("All end tasks: %d\n", end_tasks);
	}

	MPI_Finalize();
	return 0;
}

void init_task_set(int* task_set, int task_count, int iter_counter) {
	for (int i = 0; i < task_count; i++) {
		task_set[i] =  abs(rank - (iter_counter % size)) * DELTA;
		all_start_tasks += task_set[i];
	}
}

void exec_task_set(int* task_set) {
	pthread_mutex_lock(&mutex);
	int n = remaining_tasks;
	pthread_mutex_unlock(&mutex);
	for (int i = 0; i < n; i++) {
	    pthread_mutex_lock(&mutex);
		int weight = task_set[i];
		pthread_mutex_unlock(&mutex);
		for (int j = 0; j < weight; j++) {
			global_res += sqrtf(0.1337);
		}
		all_end_tasks += weight;
		executed_tasks_current++;
		pthread_mutex_lock(&mutex);
		n = remaining_tasks;
		pthread_mutex_unlock(&mutex);
	}
	remaining_tasks = 0;
}

void* receiver_start_routine(void* args) {
	int asking_proc_rank;
	int answer;
	int pending_message;
	MPI_Status status;
	while (!finished_execution) {
		MPI_Recv(&pending_message, 1, MPI_INT, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);

		if (pending_message == EXECUTOR_FINISHED_WORK) {
			printf("executor completed work in process: %d\n", rank);
			break;
		}

		asking_proc_rank = pending_message;
		pthread_mutex_lock(&mutex);
		answer = (remaining_tasks - executed_tasks_current) / size;
		pthread_mutex_unlock(&mutex);
		if (answer >= MIN_TASKS_TO_SHARE) {
			pthread_mutex_lock(&mutex);
			remaining_tasks -= answer;
			pthread_mutex_unlock(&mutex);
			MPI_Send(&answer, 1, MPI_INT, asking_proc_rank, SENDING_TASK_COUNT, MPI_COMM_WORLD);
			MPI_Send(&tasks[remaining_tasks], answer, MPI_INT, asking_proc_rank, SENDING_TASKS, MPI_COMM_WORLD);
		}
		else {
			answer = NO_TASKS_TO_SHARE;
			MPI_Send(&answer, 1, MPI_INT, asking_proc_rank, SENDING_TASK_COUNT, MPI_COMM_WORLD);
		}
	}
	pthread_exit(NULL);
}

void* executor_start_routine(void* args) {
	tasks = (int*)calloc(TASK_COUNT, sizeof(int));
	double start_time;
	double end_time;
	double iter_duration;
	double shortest_iter;
	double longest_iter;
	int thread_ans;

	for (int i = 0; i < LISTS_COUNT; i++) {
		start_time = MPI_Wtime();
		printf("Iteration number: %d\n", i);
		init_task_set(tasks, TASK_COUNT, i);
		executed_tasks = 0;
		executed_tasks_current = 0;
		additional_tasks_sum = 0;
		remaining_tasks = TASK_COUNT;
		additional_tasks = 0;

		exec_task_set(tasks);
		executed_tasks += executed_tasks_current;
		printf("process %d executed all in %lf - request new", rank, MPI_Wtime() - start_time);
		int flag = 1;
		while (flag) {
			flag = 0;
			for (int proc_numb = 0; proc_numb < size; proc_numb++) {
				if (proc_numb != rank) {
					MPI_Send(&rank, 1, MPI_INT, proc_numb, 123, MPI_COMM_WORLD);
					MPI_Recv(&thread_ans, 1, MPI_INT, proc_numb, SENDING_TASK_COUNT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					
					if (thread_ans != NO_TASKS_TO_SHARE) {
						flag = 1;
						printf("process %d received %d tasks from %d\n", rank, thread_ans, proc_numb);
						memset(tasks, 0, TASK_COUNT);
						additional_tasks = thread_ans;
						additional_tasks_sum += additional_tasks;

						MPI_Recv(tasks, additional_tasks, MPI_INT, proc_numb, SENDING_TASKS, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
						pthread_mutex_lock(&mutex);
						remaining_tasks = additional_tasks;
						executed_tasks_current = 0;
						pthread_mutex_unlock(&mutex);
						exec_task_set(tasks);
						executed_tasks += executed_tasks_current;
					}
				}
			}
		}
		end_time = MPI_Wtime();
		iter_duration = end_time - start_time;
		MPI_Allreduce(&iter_duration, &longest_iter, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
		MPI_Allreduce(&iter_duration, &shortest_iter, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
		printf("process %d executed %d main and %d additional tasks in %lf with res: %lf\n",
			rank, executed_tasks, additional_tasks_sum, iter_duration, global_res);
	}
	maximum_difference = longest_iter - shortest_iter;
	pthread_mutex_lock(&mutex);
	finished_execution = 1;
	pthread_mutex_unlock(&mutex);
	int signal = EXECUTOR_FINISHED_WORK;
	MPI_Send(&signal, 1, MPI_INT, rank, 123, MPI_COMM_WORLD);
	free(tasks);
	pthread_exit(NULL);
}