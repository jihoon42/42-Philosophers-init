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
- Each child replaces the inherited data semaphore with its own unlinked named
  data semaphore for its main thread and monitor thread.
- A named admission semaphore prevents all philosophers from holding one fork.
- A named fork-pair semaphore serializes only the acquisition of two fork
  tokens.
- Multi-philosopher children create a detached monitor thread to detect death.
- The one-philosopher bonus path prints its own deterministic death without a
  monitor thread.
- Children exit with explicit statuses for full completion, death, and setup
  errors.

## Differences Between Mandatory and Bonus

The mandatory program shares memory between threads and protects it with
pthread mutexes. The bonus program separates philosophers into processes and
uses named semaphores for synchronization. The parent process in the bonus part
inspects `waitpid` statuses: death stops remaining children, full children are
counted until every child exits cleanly, and setup or abnormal exits return
failure after cleanup.

## Synchronization Strategy

Mandatory fork deadlock is avoided by reserving both adjacent fork states
together or reserving neither. Philosophers also follow deterministic eating
phases so non-adjacent groups get regular opportunities instead of relying on
thread scheduling fairness.

Bonus fork deadlock and high-count starvation are avoided with an admission
semaphore initialized to half of the philosopher count, plus a fork-pair
semaphore initialized to one. A child reserves an admission slot, briefly locks
the fork-pair semaphore, takes two fork tokens, unlocks the fork-pair
semaphore, and releases the admission slot only after putting both fork tokens
back. This avoids one-fork hoarding while still allowing many children to eat
concurrently.

The bonus child updates `last_meal` immediately after taking both fork tokens
and before queued fork/eat logs. This prevents a process that already owns its
fork pair from dying behind serialized output under high philosopher counts.

Bonus log output is formatted into a stack buffer and emitted with one `write`
call per line to reduce print semaphore hold time.

Death messages hold the print synchronization primitive so exactly one death is
printed and no later state message can be printed.

Normal fork, eat, sleep, and think logs are guarded by a local death-deadline
check immediately before printing.

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

- `./implementation/philo/philo 1 800 200 200` printed one fork and one death
  at about 800 ms.
- `./implementation/philo/philo 2 60 60 60` printed exactly one death at about
  60 ms.
- `./implementation/philo/philo 3 800 200 200 1` completed after each
  philosopher ate once.
- `./implementation/philo/philo 5 800 200 200 7` completed without death.
- `./implementation/philo/philo 4 410 200 200` was observed running without
  death before manual interruption.
- `./implementation/philo/philo 4 310 200 100` printed exactly one death at
  about 310 ms.

Bonus:

- `./implementation/philo_bonus/philo_bonus 1 800 200 200` printed one fork
  and one death at 800 ms.
- `./implementation/philo_bonus/philo_bonus 2 60 60 60` printed exactly one
  death at about 60 ms.
- `./implementation/philo_bonus/philo_bonus 3 200 20 20 1` completed cleanly.
- `./implementation/philo_bonus/philo_bonus 3 800 200 200 1` completed
  cleanly.
- `./implementation/philo_bonus/philo_bonus 5 800 200 200 7` completed
  cleanly.
- `./implementation/philo_bonus/philo_bonus 4 410 200 200` was observed
  running without death before manual interruption.
- `./implementation/philo_bonus/philo_bonus 4 310 200 100` printed exactly one
  death at about 310 ms.
- `./implementation/philo_bonus/philo_bonus 4 200 205 200` printed exactly one
  death at about 200 ms.
- `./implementation/philo_bonus/philo_bonus 4 410 200 200` survived a 10-second
  observation without death.
- `./implementation/philo_bonus/philo_bonus 100 800 200 200` survived a
  10-second observation without death.
- `./implementation/philo_bonus/philo_bonus 105 800 200 200` survived a
  10-second observation without death.
- `./implementation/philo_bonus/philo_bonus 200 800 200 200` survived a
  10-second observation without death.
- `LazyPhilosophersTester/PhilosophersChecker.py` run directly against
  `philo_bonus 200 800 200 200` for 10 seconds exited successfully and printed
  `Worked! :D`. The checker also printed a `killall` permission warning from
  the local sandbox, but returned status 0.

Bonus stress-fix diagnosis:

- The `200 800 200 200` failure was a real philosopher death, not a parent
  status-handling or child setup error.
- The old `number_of_philosophers - 1` admission limit still allowed too many
  processes to contend around fork acquisition and serialized logging. Combined
  with the inherited global data semaphore, some children missed their death
  deadline under high process counts.
- The fix keeps the process/semaphore architecture and explicit child exit
  statuses, but narrows fork admission, serializes only fork-pair acquisition,
  removes cross-child data-lock contention, and shortens print critical
  sections.

Build and Norm:

- `make -C implementation/philo re` and `make -C implementation/philo` passed.
- `make -C implementation/philo_bonus re` and `make -C implementation/philo_bonus`
  passed.
- `norminette implementation/philo implementation/philo_bonus` reported OK.
- Invalid-input sweeps for both programs printed `Error` for no arguments, too
  few arguments, too many arguments, non-numeric values, negative values, zero
  philosophers, and zero timing values.

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
