*This project has been created as part of the 42 curriculum by jkim2*

# Philosophers

## Description

This repository contains the 42 Philosophers project. The goal is to simulate
the dining philosophers problem while respecting strict synchronization rules:
state messages must not overlap, shared data must be protected, and the
simulation must stop on death or when the optional meal target is reached.

The mandatory program uses threads and mutexes. The bonus program uses child
processes and named semaphores.

## Instructions

Build the mandatory program:

```sh
cd implementation/philo
make
```

Build the bonus program:

```sh
cd implementation/philo_bonus
make
```

Both programs accept:

```text
number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

Invalid input prints `Error` and exits with a non-zero status.

## Usage Examples

Mandatory:

```sh
./philo 5 800 200 200
./philo 5 800 200 200 3
./philo 1 200 100 100
```

Bonus:

```sh
./philo_bonus 5 800 200 200
./philo_bonus 5 800 200 200 3
./philo_bonus 1 200 100 100
```

## Project Structure

```text
implementation/
  philo/          mandatory threads and mutexes implementation
  philo_bonus/    bonus processes and semaphores implementation
  AI_IMPLEMENTATION_NOTES.md
README.md
```

## Technical Overview

The mandatory program creates one pthread per philosopher and one mutex per
fork. Fork availability is reserved under the fork mutexes so a philosopher
does not hold one fork while waiting for another. Additional mutexes protect
printing, termination state, and meal data. A monitor loop checks death timing
and optional meal completion.

The bonus program creates one child process per philosopher. A counting
semaphore represents the forks, another semaphore serializes printing, and an
admission semaphore prevents every process from holding one fork forever. Each
child has a monitor thread for its own death detection.

## Resources

- 42 Philosophers subject
- POSIX pthread documentation
- POSIX semaphore documentation
- `gettimeofday(2)`, `usleep(3)`, `fork(2)`, `waitpid(2)`

AI assistance was used to design, implement, and test this project. The
implementation was constrained to this project root and generated specifically
for the mandatory and bonus requirements described in the provided subject and
goal files.
