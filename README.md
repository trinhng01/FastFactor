# FastFactor

Fastfactor is a program in C that find all the inegral factors of a number (integer) using multiple processes with fork() and pipe().

Fastfactor works like a "standard" UNIX command so the user can invoke the command like

```
fastfactor 12 13
```
which will deliver an output
```
12: 1 2 3 4 6 12
13: 1 13
```
or they can pass a file with a list of integers. for example, assume we have a file nums

```
cat nums
12
13
4
```
when we run
```
fastfactor < nums
```
we will get this result
```
12: 1 2 3 4 6 12
13: 1 13
4: 1 2 4
```


