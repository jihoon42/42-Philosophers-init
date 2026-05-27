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
- Threads wait on a start barrier so `last_meal` is initialized after all
  philosophers have been created.
- The main thread runs the monitor loop and joins all philosopher threads.

## Bonus Architecture

- One child process is created per philosopher.
- The parent process coordinates termination and never acts as a philosopher.
- A named counting semaphore represents the forks.
- A named print semaphore serializes all logs.
- Each child replaces the inherited data semaphore with its own unlinked named
  data semaphore for its main thread and monitor thread.
- A named admission semaphore caps fork contenders at half the philosopher
  count, so every admitted child can take two fork tokens without deadlock.
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
together or reserving neither. The reservation locks the two fork mutexes in
canonical lower-index order, updates the availability flags, and releases the
mutexes before the philosopher eats.

The old elapsed-time bucket scheduler was removed. `schedule.c` now gives a
philosopher permission to compete for forks when neither neighbor has eaten
fewer meals, eaten earlier in the same meal round, or won the deterministic
local tie-break. This local policy avoids the old starvation pattern where even
rings were forced to wait exactly `2 * time_to_eat` and odd rings exactly
`3 * time_to_eat`; a philosopher can advance as soon as its adjacent forks and
adjacent meal history allow it.

Meal-start logging for the two fork messages and the eating message is emitted
under one print lock acquisition. Eating and sleeping waits are aligned to the
recorded meal start time, so delayed logging does not accidentally extend the
fork-holding or sleep cycle. Tight no-death cases are allowed to wake early only
when `time_to_die` is strictly larger than the minimum theoretical cycle; at or
below that boundary the sleep path stays strict so expected deaths are not
hidden.

Bonus fork deadlock is avoided with an admission semaphore initialized to half
of the philosopher count. Since no more than `count / 2` children can compete
for forks at once, the counting fork semaphore always has enough tokens for
every admitted child to take two forks. Removing fork-pair serialization keeps
the tight high-count handoff path short.

The bonus child updates `last_meal` immediately after taking both fork tokens
and before queued fork/eat logs. This prevents a process that already owns its
fork pair from dying behind serialized output under high philosopher counts.
`start_meal_clock()` still rejects a meal start if the child has already met
`current_time_ms() - last_meal >= time_to_die`.

Bonus `schedule.c` adds a phase gate only when `time_to_die` is at or below the
minimum theoretical cycle for the current philosopher count. That restores
expected death behavior for boundary cases such as `31 599 200 200 10` without
reintroducing the wide phase gate that caused tight no-death starvation.

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
  statuses, but narrows fork admission, removes cross-child data-lock
  contention, shortens print critical sections, and avoids serializing every
  fork-pair acquisition.

Build and Norm:

- `make -C implementation/philo re` and `make -C implementation/philo` passed.
- `make -C implementation/philo_bonus re` and `make -C implementation/philo_bonus`
  passed.
- `norminette implementation/philo implementation/philo_bonus` reported OK.
- Invalid-input sweeps for both programs printed `Error` for no arguments, too
  few arguments, too many arguments, non-numeric values, negative values, zero
  philosophers, and zero timing values.

## Tight Scheduler Follow-up, 2026-05-27

Clean-run setup:

- Filtered process checks before timing runs and after bonus/Lazy validation
  found no stale project `philo`, `philo_bonus`, or tester processes.
- Generated tester artifacts in `implementation/42-philosophers-tester/fails`,
  `implementation/42-philosophers-tester/tempfile`, and
  `implementation/inspection_logs` were cleaned before the main reproduction
  pass. New logs were generated during final validation.
- Earlier stale `philo_bonus` orphan processes remain a plausible explanation
  for conflicting pre-fix evidence, but no stale project processes remained at
  the end of this session.

The independent copy-inspection findings did reproduce in this tree. The
original time-bucketed mandatory scheduler was the relevant area: wide-margin
eval-sheet cases were not the main issue, while tight no-death cases with 1 to
10 ms of slack exposed missed handoffs. In this tree,
`./implementation/philo/philo 199 610 200 200` reproduced a death before the
mandatory scheduler was changed.

Changes made in `implementation/philo`:

- Removed the mandatory elapsed-time phase buckets from `schedule.c`.
- Kept fork reservation, with both fork availability flags updated together
  under canonical fork-pair locking.
- Replaced phase gating with local neighbor fairness: a philosopher waits for
  any non-full neighbor that has fewer meals, an older meal timestamp in the
  same meal round, or a lower deterministic tie rank.
- Added a start barrier so all `last_meal` values are set after thread
  creation.
- Added strict `start_meal_clock()` checks before meal logs; the death rule
  remains `current_time_ms() - last_meal >= time_to_die`.
- Batched the two fork logs and eating log for a meal start into one print-lock
  section and one write buffer.
- Aligned eating and sleeping waits to the recorded meal start time.
- Added conditional early wakeups for sleeping/eating waits only when the case
  is a valid no-death case with timing slack beyond the minimum cycle.
- Removed the fixed 1 ms startup delay for even philosophers.

Why the new mandatory scheduler avoids starvation:

- It no longer depends on elapsed-time buckets that drift under OS scheduling
  and printing latency.
- Neighbor priority follows actual meal progress, so adjacent philosophers that
  are behind or older in the current round get the next opportunity.
- Deterministic tie ranks create an initial independent eating set without
  permanently pinning philosophers to fixed wall-clock windows.
- Atomic fork reservation preserves deadlock-freedom without holding fork
  mutexes through the full meal.
- The start barrier prevents early thread-creation skew from counting against
  slow-starting workers.

Final mandatory verification commands/results from this session:

- `make -C implementation/philo fclean`: passed.
- `make -C implementation/philo`: passed.
- `make -C implementation/philo`: passed, nothing to do.
- `norminette implementation/philo`: all files OK.
- `./implementation/philo/philo 3 599 200 200 10`: death observed at about
  599 ms.
- `./implementation/philo/philo 31 599 200 200 10`: death observed at about
  599 to 600 ms.
- `./implementation/philo/philo 199 610 200 200`: 3/3 focused repetitions
  stayed alive for the bounded observation window.
- `./implementation/philo/philo 5 601 200 200`: 3/3 focused repetitions stayed
  alive for the bounded observation window.
- `./implementation/philo/philo 50 401 200 200`: 3/3 focused repetitions
  stayed alive for the bounded observation window.
- `./implementation/philo/philo 198 401 200 200`: 3/3 focused repetitions
  stayed alive for the bounded observation window.
- `./implementation/philo/philo 198 401 200 200 10`: 5/5 meal-count
  repetitions completed without death.
- `./implementation/philo/philo 200 800 200 200`: alive after 8 seconds,
  killed by the harness, no death log.
- `./implementation/philo/philo 4 410 200 200`: alive after 8 seconds, killed
  by the harness, no death log.
- `./implementation/philo/philo 4 410 200 200 10`: completed, no death log.
- `./implementation/philo/philo 5 800 200 200 7`: completed, no death log.
- `printf '0\n' | ./test.sh ../philo/philo` from
  `implementation/42-philosophers-tester`: passed 40/40 eval-sheet mandatory
  tests.
- `printf '10\n' | ./test.sh ../philo/philo`: Normal mode passed
  `RESULT: passed: 120 failed: 0`.
- `printf '20\n' | ./test.sh ../philo/philo`: Hardcore mode passed
  `RESULT: passed: 600 failed: 0`.

Bonus re-check after mandatory stabilized:

- `make -C implementation/philo_bonus fclean`: passed.
- `make -C implementation/philo_bonus`: passed.
- `make -C implementation/philo_bonus`: passed, nothing to do.
- `norminette implementation/philo implementation/philo_bonus`: all files OK.
- `./implementation/philo_bonus/philo_bonus 31 599 200 200 10`: 5/5 focused
  repetitions died after the bonus boundary scheduler and strict meal-start
  check were added.
- `./implementation/philo_bonus/philo_bonus 131 596 200 200 10`: 5/5 focused
  repetitions died.
- `./implementation/philo_bonus/philo_bonus 198 401 200 200 10`: 10/10 focused
  repetitions completed without death after fork-pair serialization was removed.
- `./implementation/philo_bonus/philo_bonus 4 410 200 200 10`: completed
  30/30 focused repetitions without death on the final binary.
- `./implementation/philo_bonus/philo_bonus 4 410 200 200`,
  `100 800 200 200`, and `200 800 200 200`: each survived a 10-second bounded
  observation without death on the final binary.
- `printf '0\n' | ./test.sh ../philo_bonus/philo_bonus`: passed 40/40
  eval-sheet bonus tests.
- `printf '10\n' | ./test.sh ../philo_bonus/philo_bonus`: Normal mode passed
  `RESULT: passed: 120 failed: 0`.
- `printf '20\n' | ./test.sh ../philo_bonus/philo_bonus`: Hardcore mode passed
  `RESULT: passed: 600 failed: 0`.
- `./test.sh ../philo/philo` and `./test.sh ../philo_bonus/philo_bonus` from
  `implementation/LazyPhilosophersTester`: both exited 0, but the no-die
  counter is unreliable in this sandbox because the checker prints `killall`
  permission warnings and reports `PASSED: 0/6 | FAILED: 0/6`.
- Invalid-input sweeps for both programs passed. All invalid values printed
  `Error`; `4 800 200 200 0` exited successfully without output.

Remaining concerns:

- Mandatory is now stable across the focused tight no-death cases and the
  42-philosophers-tester Normal/Hardcore modes run in this session, but these
  cases still depend on operating-system scheduling staying within the small
  1 to 10 ms slack provided by the tests.
- LazyPhilosophersTester's no-die summary cannot be used as primary evidence in
  this environment because its checker process cannot run its cleanup command.
  The focused manual runs and 42-philosophers-tester results are the primary
  timing evidence.
- The local workspace contains tester/build artifacts generated during
  reproduction and validation.

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
