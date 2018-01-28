## Hello, MPI

Здесь находится минмальный пример для запуска на кластере.

Адрес кластера calc.dc.phystech.edu

```
$ ssh calc.dc.phystech.ede -l <login>

$ git clone https://gitlab.com/n-canter/openmp_dot_product.git

$ cd openmp_dot_product/example
$ make
$ module add openmpi
$ sbatch -n 2 ./wrapper ./run
$ cat slurm-*
```

Напоминаю, что для получения пароля к кластеру нужно написать письмо мне на почту.
В теме **должно быть** слово МВС, в теле указать фамилию, имя и логин.
