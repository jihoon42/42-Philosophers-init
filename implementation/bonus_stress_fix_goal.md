# Bonus Stress Fix Goal: philo_bonus high-count premature termination

You are working on the existing 42 Philosophers implementation.

## Absolute sandbox boundary

Treat this directory as the absolute project root:

/Users/jihoonkim/42/Philosophers

Never read, list, search, modify, create, delete, or reference files outside this
directory. Do not use `..` to access any parent directory. Do not inspect the
home directory, shell history, global git config, SSH keys, credentials, package
caches, or any external path. Do not use the network.

## Scope

This is a targeted bonus fix session.

The current local code has not necessarily been pushed to GitHub. Inspect and
modify the local files under the project root only.

You may modify only:
- implementation/philo_bonus
- implementation/AI_IMPLEMENTATION_NOTES.md
- README.md, only if the current documentation becomes inaccurate

Do not modify unless absolutely necessary:
- implementation/philo

Do not modify:
- SUBJECT_Philosophers.md
- norm.md
- implementation/goal.md
- implementation/review_goal.md
- implementation/fix_goal.md
- implementation/bonus_stress_fix_goal.md
- implementation/LazyPhilosophersTester

Do not rewrite the whole project. Preserve the current structure and apply the
smallest robust fix that resolves the bonus high-count failure.

## Known current result

Mandatory currently passes LazyPhilosophersTester no-die tests including:

./philo 200 800 200 200

Bonus currently fails this no-die test:

./philo_bonus 200 800 200 200

Observed result:
- LazyPhilosophersTester reports:
  KO - program terminated prematurely
- It fails around 2 seconds out of the expected 10-second no-die observation.
- Lower-count bonus no-die tests such as 5, 100, and 105 philosophers pass.

Your goal is to fix philo_bonus so that 200 philosophers with:

200 800 200 200

does not die and does not terminate prematurely during a 10-second bounded
observation.

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
- bonus process/semaphore model

Bonus allowed functions only:
memset, printf, malloc, free, write, fork, kill, exit,
pthread_create, pthread_detach, pthread_join,
usleep, gettimeofday, waitpid,
sem_open, sem_close, sem_post, sem_wait, sem_unlink

Do not introduce:
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

## Diagnosis requirements

Before patching, inspect the current local implementation of philo_bonus.

Determine whether the premature termination is caused by:
1. a philosopher actually dying;
2. a child process exiting with an error status;
3. the parent incorrectly treating a child exit as successful completion;
4. fork acquisition throughput collapse at high philosopher counts;
5. semaphore initialization, stale semaphore, or cleanup issues;
6. pthread_create failure inside children at high process count;
7. another concrete cause.

Do not guess silently. Document the diagnosed cause in
implementation/AI_IMPLEMENTATION_NOTES.md.

## Main suspected issue: high-count fork acquisition

Investigate the current fork-admission strategy.

If the implementation uses a seats/admission semaphore initialized to
number_of_philosophers - 1, do not assume that this is sufficient for the bonus
central-fork semaphore model.

Problem to avoid:
- many processes may enter fork acquisition;
- many may each take one fork;
- too few philosophers can obtain two forks quickly;
- high-count tests can starve or terminate even though they should survive.

Required behavior:
- a philosopher should not be allowed to contribute to a large system-wide state
  where many processes each hold exactly one fork and wait for a second fork;
- high-count cases must maintain enough eating throughput;
- 200 800 200 200 should survive a 10-second bounded observation.

Preferred fix:
- Use an admission/eating-slot semaphore that allows at most
  number_of_philosophers / 2 philosophers to try to acquire forks at the same
  time when count > 1.
- For count == 1, keep the explicit one-philosopher death path.
- For count >= 2, the eating-slot value must be at least 1.
- A philosopher must acquire an eating slot before taking any fork.
- The eating slot must be released only after both forks are released.
- Keep the fork semaphore initialized to number_of_philosophers.
- Still print "has taken a fork" exactly once after each fork is actually
  acquired.

Alternative acceptable fix:
- Use a separate semaphore that serializes acquisition of a pair of forks so a
  process does not hold one fork while many others also hold one.
- This alternative must still allow adequate throughput for 200 800 200 200.
- Do not choose a solution that makes only one philosopher eat at a time for
  large N.

Do not fix this by:
- adding arbitrary long startup sleeps;
- reducing accepted philosopher count;
- special-casing 200;
- suppressing death output;
- forcing the parent to keep running after all children are dead;
- changing the tester;
- weakening input validation.

## Parent/child termination requirements

Re-check the current parent/child exit logic after the stress fix.

For runs without number_of_times_each_philosopher_must_eat:
- child processes should not exit normally by themselves;
- the parent should continue until a real death or error occurs;
- if any child exits unexpectedly with a non-death status, the parent must treat
  it as an error, clean up all children, and return failure;
- if a child exits with the explicit death status, the parent must kill and reap
  remaining children and return normally from the simulation.

For runs with number_of_times_each_philosopher_must_eat:
- a child that reaches its quota should release forks and exit with the explicit
  full/completion status;
- the parent should count full/completion exits;
- when all children completed, the parent should finish cleanly;
- the parent should avoid killing children mid-log during normal completion.

Preserve exactly one death message.

Do not print normal fork/eat/sleep/think messages after a local death deadline.

## Semaphore requirements

- Unlink named semaphores before opening them.
- Close and unlink semaphores during cleanup.
- Do not leave stale named semaphores that affect repeated runs.
- Do not introduce semaphore leaks.
- If sem_open fails after some semaphores were opened, clean up what was opened.

If you add or repurpose a semaphore, update:
- header definitions
- init/open logic
- close/unlink cleanup logic
- all relevant error paths
- AI_IMPLEMENTATION_NOTES.md

## Validation

Run from /Users/jihoonkim/42/Philosophers only.

Build:
- make -C implementation/philo_bonus re
- make -C implementation/philo_bonus

Mandatory regression, only if you changed implementation/philo:
- make -C implementation/philo re
- make -C implementation/philo
- ./implementation/philo/philo 200 800 200 200

Norm:
- If norminette is available, run:
  norminette implementation/philo_bonus
- If implementation/philo was modified, also run:
  norminette implementation/philo
- Fix all Norm errors.
- If norminette is unavailable, manually review norm.md constraints and document
  that norminette was unavailable.

Focused bonus tests:
- ./implementation/philo_bonus/philo_bonus 1 800 200 200
- ./implementation/philo_bonus/philo_bonus 2 60 60 60
- ./implementation/philo_bonus/philo_bonus 4 310 200 100
- ./implementation/philo_bonus/philo_bonus 4 200 205 200
- ./implementation/philo_bonus/philo_bonus 3 200 20 20 1
- ./implementation/philo_bonus/philo_bonus 5 800 200 200 7
- ./implementation/philo_bonus/philo_bonus 4 410 200 200
- ./implementation/philo_bonus/philo_bonus 100 800 200 200
- ./implementation/philo_bonus/philo_bonus 105 800 200 200
- ./implementation/philo_bonus/philo_bonus 200 800 200 200

For no-die tests without a meal limit, use a bounded observation of about
10 seconds. The program should still be running without printing "died"; then
terminate it cleanly for the test.

If available, run:
- cd implementation/LazyPhilosophersTester
- ./test.sh ../philo_bonus/philo_bonus

Do not modify the tester.

## Expected result

The following must no longer happen:

./implementation/philo_bonus/philo_bonus 200 800 200 200

must not terminate prematurely during a 10-second no-die observation.

LazyPhilosophersTester should pass all no-die tests for philo_bonus, including
the 200 philosopher case.

## Documentation update

Update implementation/AI_IMPLEMENTATION_NOTES.md with:
- root cause of the 200 philosopher bonus premature termination;
- exact bonus fork-admission strategy after the fix;
- parent/child exit-status behavior after the fix;
- semaphore changes, if any;
- build results;
- Norm result;
- focused tests and observed results;
- remaining limitations, if any.

Do not overstate guarantees that were not actually tested.

## Final response

At the end, report:
- root cause found;
- files modified;
- code changes made;
- build result;
- Norm result;
- focused test results;
- LazyPhilosophersTester result, if run;
- whether philo_bonus 200 800 200 200 now survives a 10-second no-die test;
- remaining concerns.
