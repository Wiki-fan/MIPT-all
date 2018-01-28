#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MASTER 0

void hello_world(int rank, int size)
{
	printf("Hello from %d out of %d\n", rank, size);
}

void simple_send_recv(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	int v;

	if (rank == MASTER) {
		v = rand();
		MPI_Send(&v, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		printf("%d: sent %d\n", rank, v);
		return;
	}

	MPI_Recv(&v, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	printf("%d: recv %d\n", rank, v);
}

void ring_send_recv(int rank, int size)
{
	int dst = (rank + 1) % size;
	int src = rank - 1;
	if (src < 0) {
		src = size - 1;
	}
	int v;
	if (rank == MASTER) {
		v = 42;
		MPI_Send(&v, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
		MPI_Recv(&v, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d: recv %d from %d\n", rank, v, src);
		return;
	}
	MPI_Recv(&v, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	MPI_Send(&v, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
	printf("%d: recv %d from %d\n", rank, v, src);
}

void send_recv_right(int rank, int size)
{
	int dst = (rank + 1) % size;
	int src = rank - 1;
	if (src < 0) {
		src = size - 1;
	}
	int v = rank * 10;
	if (rank % 2 == 0) {
		MPI_Ssend(&v, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);

		MPI_Recv(&v, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d: recv %d from %d\n", rank, v, src);
	} else {
		MPI_Recv(&v, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d: recv %d from %d\n", rank, v, src);
		v = rank * 10;
		MPI_Ssend(&v, 1, MPI_INT, dst, 0, MPI_COMM_WORLD);
	}
}

void send_recv_right_nonblocking(int rank, int size)
{
	int dst = (rank + 1) % size;
	int src = rank - 1;
	if (src < 0) {
		src = size - 1;
	}
	int v = rank * 10;
	MPI_Request requests[2];
	MPI_Issend(&v, 1, MPI_INT, dst, 0, MPI_COMM_WORLD, requests);
	MPI_Irecv(&v, 1, MPI_INT, src, MPI_ANY_TAG, MPI_COMM_WORLD, requests + 1);

	MPI_Waitall(2, requests, MPI_STATUS_IGNORE);
	printf("%d: recv %d from %d\n", rank, v, src);
}

void send_array(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	int *buf = NULL;
	if (rank == MASTER) {
		int n = 10;
		buf = malloc(n * sizeof(int));
		for (int i = 0; i < n; i++) {
			buf[i] = i;
		}
		MPI_Send(&n, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send(buf, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else {
		int n;
		MPI_Recv(&n, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		buf = malloc(n * sizeof(int));
		MPI_Recv(buf, n, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n; i++) {
			printf("buf[%d] = %d\n", i, buf[i]);
		}
	}
	if (buf) {
		free(buf);
	}
}

void send_array_probe(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	int *buf = NULL;
	if (rank == MASTER) {
		int n = 10;
		buf = malloc(n * sizeof(int));
		for (int i = 0; i < n; i++) {
			buf[i] = i;
		}
		MPI_Send(buf, n, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else {
		int n;
		MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &n);

		buf = malloc(n * sizeof(int));
		MPI_Recv(buf, n, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n; i++) {
			printf("buf[%d] = %d\n", i, buf[i]);
		}
	}
	if (buf) {
		free(buf);
	}
}

void scatter(int rank, int size)
{
	int *buf = calloc(20, sizeof(int));
	if (rank == MASTER) {
		for (int i = 0; i < 20; i++) {
			buf[i] = i;
		}
	}
	int *inbuf = malloc(5 * sizeof(int));
	MPI_Scatter(buf, 5, MPI_INT, inbuf, 5, MPI_INT, MASTER, MPI_COMM_WORLD);

	for (int i = 0; i < 5; i++) {
		printf("%d: inbuf[%d] = %d\n", rank, i, inbuf[i]);
	}
	free(buf);
	free(inbuf);

	// gather
	// scatterv, gatherv
	// allgather
}

void reduce(int rank, int size)
{
	int buf[3];
	int outbuf[3];
	for (int i = 0; i < 3; i++) {
		buf[i] = 1;
	}

	MPI_Reduce(buf, outbuf, 3, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);
	if (rank == MASTER) {
		for (int i = 0; i < 3; i++) {
			printf("outbuf[%d] = %d\n", i, outbuf[i]);
		}
	}
	// allreduce
}

void contiguous(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	int n = 4;
	int *matrix = calloc(n * n, sizeof(int));
	MPI_Datatype datatype;
	MPI_Type_contiguous(n * n, MPI_INT, &datatype);
	MPI_Type_commit(&datatype);
	if (rank == MASTER) {
		for (int i = 0; i < n * n; i++) {
			matrix[i] = i;
		}
		MPI_Send(matrix, 1, datatype, 1, 0, MPI_COMM_WORLD);
	} else {
		MPI_Recv(matrix, 1, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n * n; i++) {
			printf("m[%d] = %d\n", i, matrix[i]);
		}
	}
	MPI_Type_free(&datatype);
	free(matrix);
}

void vector(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	int n = 4;
	int *matrix = calloc(n * n, sizeof(int));
	MPI_Datatype datatype;
	MPI_Type_vector(2, 4, 8, MPI_INT, &datatype);
	MPI_Type_commit(&datatype);
	if (rank == MASTER) {
		for (int i = 0; i < n * n; i++) {
			matrix[i] = i;
		}
		MPI_Send(matrix, 1, datatype, 1, 0, MPI_COMM_WORLD);
	} else {
		MPI_Recv(matrix, 1, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < n * n; i++) {
			printf("m[%d] = %d\n", i, matrix[i]);
		}
	}
	MPI_Type_free(&datatype);
	free(matrix);
}

void custom(int rank, int size)
{
	if (rank >= 2) {
		return;
	}
	struct {
		int a;
		float b;
		int x; // we'll skip this element
		double c[2];
	} in, out;
	in.a = 1;
	in.b = 2;
	in.x = 42;
	in.c[0] = 3;
	in.c[1] = 4;

	MPI_Datatype types[3] = {MPI_INT, MPI_FLOAT, MPI_DOUBLE};
	int blocks[3] = {1, 1, 2};

	MPI_Aint disps[3];
	// it's better to use MPI_Address instead of MPI_Type_extent()
	// because of alingment and padding
	// Using MPI_Address instead of '&' allows you to build more portables soultions
	MPI_Address(&in, disps); 
	MPI_Address(&in.b, disps + 1);
	MPI_Address(&in.c, disps + 2);

	long base = disps[0];
	for (int i = 0; i < 3; i++) {
		disps[i] -= base;
	}

	MPI_Datatype datatype;
	MPI_Type_struct(3, blocks, disps, types, &datatype);
	MPI_Type_commit(&datatype);
	if (rank == MASTER) {
		MPI_Send(&in, 1, datatype, 1, 0, MPI_COMM_WORLD);
	} else {
		MPI_Recv(&out, 1, datatype, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("%d %f %lf %lf\n", out.a, out.b, out.c[0], out.c[1]);
	}
	MPI_Type_free(&datatype);
}


void barrier(int rank, int size)
{
	if (rank == 0) {
		sleep(3);
	}
	printf("%d: before barrier\n", rank);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("%d: after barrier\n", rank);
}

void broadcast(int rank, int size)
{
	int v;
	if (rank == MASTER) {
		v = 123;
		sleep(3);
	}
	MPI_Bcast(&v, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	printf("%d: v = %d\n", rank, v);
}


int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	custom(rank, size);

	MPI_Finalize();
	return 0;
}
