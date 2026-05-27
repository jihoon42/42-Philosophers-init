# Reproduce-Then-Fix Goal: Philosophers Timing Stability

You are working on the existing 42 Philosophers implementation.

This is a targeted reproduce-then-fix session, not a rewrite.

## Absolute sandbox boundary

Treat this directory as the absolute project root:

/Users/jihoonkim/42/Philosophers

Never read, list, search, modify, create, delete, or reference files outside this
directory.

Do not use `..` to escape the project root. Relative tester commands such as
`../philo/philo` and `../philo_bonus/philo_bonus` are allowed only when run from
tester directories and only if they resolve inside this project root.

Do not inspect:
- /Users/jihoonkim/42/Philo-copy
- my home directory outside this project
- shell history
- global git config
- SSH keys
- credentials
- package caches
- any external path

Do not use the network.
Do not install packages.
Do not run docker scripts.
Do not push, pull, fetch, commit, or modify git remotes.

## Scope

You may modify only:
- implementation/philo
- implementation/philo_bonus
- implementation/AI_IMPLEMENTATION_NOTES.md
- README.md only if it becomes inaccurate

Do not modify:
- SUBJECT_Philosophers.md
- norm.md
- implementation/LazyPhilosophersTester source files
- implementation/42-philosophers-tester source files
- implementation/INSPECTION.md
- previous goal/review/fix md files
- implementation/reproduce_then_fix_goal.md

You may remove or overwrite generated test artifacts only if needed for a clean
run:
- implementation/42-philosophers-tester/fails/*
- implementation/42-philosophers-tester/tempfile
- implementation/inspection_logs/*

Do not manually edit tester scripts or tester source files.

## Key context

Two inspections produced conflicting evidence.

Original /Users/jihoonkim/42/Philosophers inspection reported:
- mandatory 42-philosophers-tester option 0 failed:
  - 37 passed / 3 failed
  - failing case: 4 410 200 200 10
- bonus 42-philosophers-tester option 0 failed:
  - 38 passed / 2 failed
  - failing case: 4 410 200 200 10
- manual mandatory high-count no-die failed:
  - ./implementation/philo/philo 200 800 200 200
  - observed death: 11789 33 died

Copy inspection was interrupted/incomplete, but found:
- eval-sheet 42 tester option 0 passed 40/40 for mandatory
- eval-sheet 42 tester option 0 passed 40/40 for bonus
- many stale orphan philo_bonus processes from previous tests were present and
  had to be cleaned
- after cleanup, mandatory high-count 100/200 no-die observations survived

Because the code is currently identical between the two folders, do not assume
the original failure is definitely caused by code until it is reproduced in a
clean environment.

## Required strategy

Do this in three phases:

1. Clean environment and reproduce.
2. If failures are reproduced, apply the smallest robust fix.
3. Re-run the exact failing tests plus regression suites.

If the original failures cannot be reproduced after a clean run, do not make
source-code changes. Instead, update only AI_IMPLEMENTATION_NOTES.md if needed
with a clear note about non-reproducibility and process-contamination risk.

## Required constraints

Follow SUBJECT_Philosophers.md and norm.md.

Preserve:
- C only
- no libft
- no global variables
- no unauthorized functions
- no data races
- no unnecessary relinking
- cc with -Wall -Wextra -Werror
- Norm compliance
- mandatory thread/mutex model
- bonus process/semaphore model

Mandatory allowed functions only:
memset, printf, malloc, free, write, usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy,
pthread_mutex_lock, pthread_mutex_unlock

Bonus allowed functions only:
memset, printf, malloc, free, write, fork, kill, exit,
pthread_create, pthread_detach, pthread_join,
usleep, gettimeofday, waitpid,
sem_open, sem_close, sem_post, sem_wait, sem_unlink

Do not introduce:
- pthread condition variables
- semaphores in mandatory
- pthread mutexes in bonus
- sem_trywait
- sem_getvalue
- signal handlers
- sleep
- atoi, strtol, strlen, strcmp, perror, strerror, or other unauthorized calls
- for
- do while
- switch
- case
- goto
- ternary operator
- variable length arrays
- global variables

Keep all functions within Norm limits:
- max 25 lines per function
- max 4 parameters
- max 5 local variables
- max 5 function definitions per .c file
- max 80 columns per line
- declarations at the beginning of functions
- no comments inside function bodies

Do not change these policies unless a reproduced failure proves it necessary:
- death is current_time_ms() - last_meal >= time_to_die
- optional meal count 0 means immediate successful completion
- log strings and public log format stay exactly unchanged
- keep implementation under implementation/philo and implementation/philo_bonus

## Phase 1: Clean environment and reproduce

Run from /Users/jihoonkim/42/Philosophers only.

### 1.1 Clean stale processes

Before any timing test, check for stale processes from previous runs.

Use safe process inspection. Kill only processes that are clearly philosopher
test processes owned by this user and related to this project or to the binary
names philo/philo_bonus.

At minimum:
- list matching philo/philo_bonus processes with pid, ppid, elapsed time, and
  command when possible
- clean stale philo/philo_bonus processes before timed tests
- confirm no stale philo/philo_bonus processes remain

Be careful not to kill the current shell or unrelated editor processes.

### 1.2 Clean build and tester artifacts

Run:
- make -C implementation/philo fclean
- make -C implementation/philo
- make -C implementation/philo
- make -C implementation/philo_bonus fclean
- make -C implementation/philo_bonus
- make -C implementation/philo_bonus

Record whether the second make relinks.

If useful, remove only generated tester artifacts:
- implementation/42-philosophers-tester/fails/*
- implementation/42-philosophers-tester/tempfile

Do not modify tester scripts.

### 1.3 Norm

Run:
- norminette implementation/philo implementation/philo_bonus

If norminette is unavailable, manually review norm.md and say so.

### 1.4 Reproduce original failures first

Run these before making any code changes:

Mandatory:
- repeat ./implementation/philo/philo 4 410 200 200 10 at least 30 times
- observe ./implementation/philo/philo 200 800 200 200 for about 10 seconds
- run 42-philosophers-tester eval sheet:
  cd implementation/42-philosophers-tester
  printf '0\n' | ./test.sh ../philo/philo

Bonus:
- repeat ./implementation/philo_bonus/philo_bonus 4 410 200 200 10 at least 30 times
- observe ./implementation/philo_bonus/philo_bonus 200 800 200 200 for about 10 seconds
- run 42-philosophers-tester eval sheet:
  cd implementation/42-philosophers-tester
  printf '0\n' | ./test.sh ../philo_bonus/philo_bonus

After every bonus no-die observation, check and clean leftover philo_bonus
processes if the tester or watchdog leaves children behind.

### 1.5 Run broader 42 tester modes

If option numbers are supported by the tester script, run:

Mandatory:
- cd implementation/42-philosophers-tester
- printf '10\n' | ./test.sh ../philo/philo
- printf '20\n' | ./test.sh ../philo/philo

Bonus:
- cd implementation/42-philosophers-tester
- printf '10\n' | ./test.sh ../philo_bonus/philo_bonus
- printf '20\n' | ./test.sh ../philo_bonus/philo_bonus

If these menu inputs are not correct, inspect test.sh just enough to determine
the correct noninteractive inputs. Do not edit the tester.

### 1.6 LazyPhilosophersTester

Run:
- cd implementation/LazyPhilosophersTester
- ./test.sh ../philo/philo
- ./test.sh ../philo_bonus/philo_bonus

If the tester is interactive, drive it in the normal all-tests mode.
Record whether the no-die counter is reliable in this environment.
Do not rely only on LazyPhilosophersTester if it disagrees with
42-philosophers-tester.

## Phase 2: Decision gate

After Phase 1, make a decision:

### If no failures reproduce

Do not change source code.

Update implementation/AI_IMPLEMENTATION_NOTES.md only if appropriate, adding:
- clean-run results;
- note about previous stale philo_bonus orphan processes;
- note that previous failures were not reproduced after cleanup;
- remaining caution about community tester nondeterminism.

Then run final validation and report.

### If mandatory failures reproduce

Fix mandatory first. Do not work on bonus until mandatory is stable.

Mandatory failures that must be fixed if reproduced:
- 4 410 200 200 10 dies before all philosophers complete 10 meals
- 200 800 200 200 dies during a 10-second no-die observation
- 42-philosophers-tester option 0/10/20 reports failures aligned with those
  cases

### If bonus failures reproduce but mandatory is stable

Fix bonus only after mandatory is stable.

Bonus failures that must be fixed if reproduced:
- 4 410 200 200 10 dies before all children complete 10 meals
- 200 800 200 200 terminates prematurely or dies during no-die observation
- 42-philosophers-tester option 0/10/20 reports failures aligned with those
  cases
- leftover child processes remain after normal program completion

## Phase 3: Fix directions if failures reproduce

Do not rewrite the project.
Apply the smallest robust changes.

### Mandatory possible fixes

1. Robust start synchronization

Use this only if reproduction suggests start timing unfairness.

Required behavior:
- all philosopher threads should be created before the simulation clock starts;
- all last_meal values should align with the real release time;
- monitor should not report death before all workers are released.

Implementation direction:
- add a shared start flag under a mutex;
- workers wait with short usleep polling;
- after all threads are created, set start_time, reset last_meal, set start flag,
  then monitor;
- preserve Norm and authorized functions.

2. Tight scheduling stability

Use this only if 4 410 200 200 10 or 200 800 200 200 reproducibly fails.

Required behavior:
- 4 410 200 200 10 should complete repeatedly without death;
- 4 410 200 200 should survive a bounded no-die observation;
- 200 800 200 200 should survive a bounded no-die observation.

Implementation direction:
- audit schedule.c and initial/even/odd delays;
- avoid accumulating artificial delay per cycle;
- avoid over-aggressive deterministic phasing that starves high-count cases;
- improve precise_sleep with absolute target timing and shorter sleeps near the
  deadline;
- do not suppress death;
- do not change the death comparison;
- do not special-case only 4 or 200.

3. Meal-count completion

Use this if meal-limit failures reproduce.

Required behavior:
- when a philosopher reaches must_eat, mark it full exactly once;
- track full_count under synchronization;
- set global finished immediately when the last philosopher becomes full;
- avoid extra cycles after all philosophers meet the quota;
- choose and document whether full philosophers keep participating or are skipped
  by the monitor;
- if full philosophers stop participating, monitor must not later kill them.

### Bonus possible fixes

1. Start synchronization

Use this if process start jitter reproduces tight-boundary failures.

Required behavior:
- child processes should not start death-sensitive work before the intended
  common simulation start;
- last_meal in each child must match the true start policy;
- logs remain relative to common start time.

Implementation direction:
- consider setting a near-future start_time before forking so all children
  inherit it;
- or use a start semaphore if already compatible with the architecture;
- remember that memory changes after fork are not shared.

2. last_meal timing

Audit and fix only if currently wrong or failures point to it.

Required behavior:
- last_meal should be updated when the philosopher starts eating;
- this means after both forks are acquired and immediately before or together
  with printing "is eating";
- do not reset last_meal before the philosopher has two forks.

3. Parent/child cleanup and stress behavior

Preserve previous fixes:
- one philosopher prints one fork then one death;
- child exit statuses distinguish full, death, error;
- parent uses waitpid status correctly;
- death kills and reaps remaining children;
- meal-count completion exits cleanly;
- normal completion does not kill children mid-log;
- high-count 200 800 200 200 survives bounded observation.

If tester or manual runs leave orphaned child processes after normal program
completion, inspect and fix parent cleanup.

Do not try to solve tester-caused orphaning from external SIGKILL unless it can
be done within authorized functions and without hurting normal behavior. If the
orphaning is caused only by an external tester killing the launcher process,
document it clearly.

## Final validation after any code changes

Run:
- make -C implementation/philo fclean
- make -C implementation/philo
- make -C implementation/philo
- make -C implementation/philo_bonus fclean
- make -C implementation/philo_bonus
- make -C implementation/philo_bonus
- norminette implementation/philo implementation/philo_bonus

Mandatory focused:
- ./implementation/philo/philo 1 800 200 200
- ./implementation/philo/philo 2 60 60 60
- ./implementation/philo/philo 4 310 200 100
- ./implementation/philo/philo 4 200 205 200
- ./implementation/philo/philo 3 800 200 200 1
- ./implementation/philo/philo 3 200 20 20 1
- ./implementation/philo/philo 5 800 200 200 7
- repeat ./implementation/philo/philo 4 410 200 200 10 at least 30 times
- observe ./implementation/philo/philo 4 410 200 200 for about 10 seconds
- observe ./implementation/philo/philo 100 800 200 200 for about 10 seconds
- observe ./implementation/philo/philo 200 800 200 200 for about 10 seconds

Bonus focused:
- ./implementation/philo_bonus/philo_bonus 1 800 200 200
- ./implementation/philo_bonus/philo_bonus 2 60 60 60
- ./implementation/philo_bonus/philo_bonus 4 310 200 100
- ./implementation/philo_bonus/philo_bonus 4 200 205 200
- ./implementation/philo_bonus/philo_bonus 3 800 200 200 1
- ./implementation/philo_bonus/philo_bonus 3 200 20 20 1
- ./implementation/philo_bonus/philo_bonus 5 800 200 200 7
- repeat ./implementation/philo_bonus/philo_bonus 4 410 200 200 10 at least 30 times
- observe ./implementation/philo_bonus/philo_bonus 4 410 200 200 for about 10 seconds
- observe ./implementation/philo_bonus/philo_bonus 100 800 200 200 for about 10 seconds
- observe ./implementation/philo_bonus/philo_bonus 200 800 200 200 for about 10 seconds

Community testers:
- cd implementation/42-philosophers-tester
- printf '0\n' | ./test.sh ../philo/philo
- printf '0\n' | ./test.sh ../philo_bonus/philo_bonus
- if available:
  printf '10\n' | ./test.sh ../philo/philo
  printf '10\n' | ./test.sh ../philo_bonus/philo_bonus
  printf '20\n' | ./test.sh ../philo/philo
  printf '20\n' | ./test.sh ../philo_bonus/philo_bonus
- cd ../LazyPhilosophersTester
- ./test.sh ../philo/philo
- ./test.sh ../philo_bonus/philo_bonus

Invalid input sweep for both programs:
- no arguments
- too few arguments
- too many arguments
- a b c d
- 4 800 200 x
- -1 800 200 200
- 0 800 200 200
- 4 0 200 200
- 4 800 0 200
- 4 800 200 0
- 4 800 200 200 0
- 4 800 200 200 -1
- 4 800 200 200 999999999999999999999999

## Documentation updates

Update implementation/AI_IMPLEMENTATION_NOTES.md with:
- whether the original failures reproduced after cleanup;
- process-contamination findings, if any;
- root cause if a real code bug was fixed;
- mandatory changes, if any;
- bonus changes, if any;
- exact tester commands and results;
- remaining limitations.

Update README.md only if behavior, layout, or instructions become inaccurate.

Do not overstate guarantees.

## Final response

At the end, report:
- stale process cleanup result;
- whether original failures reproduced before code changes;
- root causes found;
- files modified;
- mandatory changes, if any;
- bonus changes, if any;
- build results;
- norminette result;
- focused manual test results;
- 42-philosophers-tester results;
- LazyPhilosophersTester results;
- whether 4 410 200 200 10 is stable for both programs;
- whether mandatory and bonus 200 800 200 200 survive bounded observation;
- whether any stale philo/philo_bonus processes remain;
- remaining concerns.
