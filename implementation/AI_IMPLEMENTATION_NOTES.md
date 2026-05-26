# AI Implementation Notes

## Implementation Summary

Both parts were implemented in C under `implementation/`. The mandatory
program is in `implementation/philo`; the bonus program is in
`implementation/philo_bonus`. Both parse the same arguments, reject invalid
input, use millisecond timestamps from `gettimeofday`, and serialize log
output.

## Mandatory Architecture

- One pthread is created per philosopher.
- One mutex is created per fork.
- Fork availability is stored explicitly and protected by fork mutexes.
- A print mutex prevents overlapping log messages.
- A state mutex protects the shared stop flag.
- Each philosopher has a meal mutex protecting `last_meal` and `meals`.
- The main thread runs the monitor loop and joins all philosopher threads.

## Bonus Architecture

- One child process is created per philosopher.
- The parent process coordinates termination and never acts as a philosopher.
- A named counting semaphore represents the forks.
- A named print semaphore serializes all logs.
- A named data semaphore protects each child monitor's shared meal fields.
- A named admission semaphore prevents all philosophers from holding one fork.
- Each child creates a detached monitor thread to detect its own death.

## Differences Between Mandatory and Bonus

The mandatory program shares memory between threads and protects it with
pthread mutexes. The bonus program separates philosophers into processes and
uses named semaphores for synchronization. The parent process in the bonus part
waits for either a child death or all meal-completion notifications, then stops
the remaining children and reaps them.

## Synchronization Strategy

Mandatory fork deadlock is avoided by reserving both adjacent fork states
together or reserving neither. Philosophers also follow deterministic eating
phases so non-adjacent groups get regular opportunities instead of relying on
thread scheduling fairness.

Bonus fork deadlock is avoided with an admission semaphore initialized to
`number_of_philosophers - 1` for multi-philosopher runs. This prevents every
child from holding one fork at the same time.

Death messages hold the print synchronization primitive so exactly one death is
printed and no later state message can be printed.

## Edge Cases

- Wrong argument count prints `Error`.
- Non-numeric, negative, and overflowing values are rejected.
- Zero philosophers and zero timing values are rejected.
- A meal target of zero exits successfully without starting a simulation.
- One philosopher takes one fork and eventually dies.
- Allocation, mutex, semaphore, thread, or fork setup failures return an error.

## Build Commands

```sh
cd implementation/philo
make
```

```sh
cd implementation/philo_bonus
make
```

## Manual Tests and Observed Results

Mandatory:

- `./philo 1 200 100 100` printed one fork message and one death at about
  200 ms.
- `./philo -1 200 100 100` printed `Error` and exited non-zero.
- `./philo 5 800 200 200 3` completed without a death.
- `./philo 4 310 200 200` printed exactly one death at about 310 ms.
- A second `make` reported that nothing needed to be rebuilt.

Bonus:

- `./philo_bonus 1 200 100 100` printed one fork message and one death at
  about 200 ms.
- `./philo_bonus -1 200 100 100` printed `Error` and exited non-zero.
- `./philo_bonus 5 800 200 200 3` completed without a death.
- `./philo_bonus 4 310 200 200` printed exactly one death at about 310 ms.
- A second `make` reported that nothing needed to be rebuilt.

## Known Limitations

The bonus program uses fixed named semaphore names and unlinks stale semaphores
at startup and cleanup. Running multiple `philo_bonus` instances at the same
time would make those instances interfere with each other.

No external network resources or dependencies were used.

## Assumptions

- The optional meal target `0` means every philosopher has already eaten enough
  times, so the program exits successfully without output.
- Large numeric inputs beyond signed 32-bit range are treated as invalid input.
- The project is evaluated from the repository root with the expected
  `implementation/philo` and `implementation/philo_bonus` directories.
