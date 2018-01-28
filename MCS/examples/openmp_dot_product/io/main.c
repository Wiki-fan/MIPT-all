#include <mpi.h>
#include <stdio.h>

static int n = 4;

void simple_write(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_seek(fh, rank * n * intex, MPI_SEEK_SET);
	MPI_File_write(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void simple_read(int rank, int size)
{
	int buf[n];
	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

	MPI_File_seek(fh, n * rank * intex, MPI_SEEK_SET);
	MPI_File_read(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);

	for (int i = 0; i < n; i++) {
		printf("%d: %d\n", rank, buf[i]);
	}
}

void simple_write_at(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_write_at(fh, rank * n * intex, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void simple_read_at(int rank, int size)
{
	int buf[n];
	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

	MPI_File_read_at(fh, n * rank * intex, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);

	for (int i = 0; i < n; i++) {
		printf("%d: %d\n", rank, buf[i]);
	}
}

void simple_write_all(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_seek(fh, rank * n * intex, MPI_SEEK_SET);
	MPI_File_write_all(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void simple_read_all(int rank, int size)
{
	int buf[n];
	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

	MPI_File_seek(fh, n * rank * intex, MPI_SEEK_SET);
	MPI_File_read_all(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);

	for (int i = 0; i < n; i++) {
		printf("%d: %d\n", rank, buf[i]);
	}
}

void simple_write_at_all(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_write_at_all(fh, rank * n * intex, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void simple_read_at_all(int rank, int size)
{
	int buf[n];
	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

	MPI_File_read_at_all(fh, n * rank * intex, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);

	for (int i = 0; i < n; i++) {
		printf("%d: %d\n", rank, buf[i]);
	}
}

void simple_write_shared(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_write_shared(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void simple_write_ordered(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_File_write_ordered(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);

	MPI_File_close(&fh);
}

void matrix_write(int rank, int size)
{
	int buf[n];
	for (int i = 0; i < n; i++) {
		buf[i] = n * rank + i;
	}

	MPI_Aint intex;
	MPI_Type_extent(MPI_INT, &intex);

	MPI_File fh;
	MPI_File_open(MPI_COMM_WORLD, "data", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

	MPI_Datatype view;
	MPI_Type_vector(2, 2, 4, MPI_INT, &view);
	MPI_Type_commit(&view);
	int offset;
	switch (rank) {
	case 0:
		offset = 0;
		break;
	case 1:
		offset = 2;
		break;
	case 2:
		offset = 8;
		break;
	case 3:
		offset = 10;
		break;
	}

	offset = offset * intex;

	MPI_File_set_view(fh, offset, MPI_INT, view, "native", MPI_INFO_NULL);

	MPI_File_write_all(fh, buf, n, MPI_INT, MPI_STATUS_IGNORE);
	MPI_Type_free(&view);
	MPI_File_close(&fh);
	// MPI_Type_create_darray
}

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* simple_write(rank, size); */
	/* simple_read(rank, size); */
	/* simple_write_at(rank, size); */
	/* simple_read_at(rank, size); */
	/* simple_write_all(rank, size); */
	/* simple_read_all(rank, size); */
	/* simple_write_at_all(rank, size); */
	/* simple_read_at_all(rank, size); */
	/* simple_write_shared(rank, size); */
	/* simple_write_ordered(rank, size); */
	matrix_write(rank, size);

	MPI_Finalize();
	return 0;
}
