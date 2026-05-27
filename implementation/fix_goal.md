# Fix Goal: 42 Philosophers Review Findings

You are working on the existing 42 Philosophers implementation.

## Absolute sandbox boundary

Treat this directory as the absolute project root:

/Users/jihoonkim/42/Philosophers

Never read, list, search, modify, create, delete, or reference files outside this
directory. Do not use `..` to access any parent directory. Do not inspect the
home directory, shell history, global git config, SSH keys, credentials, package
caches, or any external path. Do not use the network.

## Scope

This is a targeted fix session.

You may modify only:
- implementation/philo
- implementation/philo_bonus
- implementation/AI_IMPLEMENTATION_NOTES.md
- README.md, only if the existing text becomes inaccurate

Do not modify:
- SUBJECT_Philosophers.md
- norm.md
- implementation/goal.md
- implementation/review_goal.md
- implementation/fix_goal.md

Do not rewrite the entire project. Preserve the existing structure as much as
possible. Apply focused fixes for the review findings.

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
- for
- do while
- switch
- case
- goto
- ternary operator
- variable length arrays
- global variables
- unauthorized parsing/library functions
- pthread mutexes in bonus
- semaphores in mandatory

Keep all functions within Norm limits:
- max 25 lines per function
- max 4 parameters
- max 5 local variables
- max 5 function definitions per .c file
- max 80 columns per line
- declarations at the beginning of functions
- no comments inside function bodies

## Fix order

Fix mandatory first. Bonus should only be considered after mandatory behavior is
stable.

## Mandatory fixes

### 1. Prevent stale non-death logs at the death boundary

Problem:
The mandatory program can print normal state messages at the exact death
deadline before the monitor prints death.

Required behavior:
- Before printing any non-death state, verify that the philosopher has not
  already reached its own death deadline.
- If current_time_ms() - last_meal >= time_to_die, do not print fork/eat/sleep/
  think for that philosopher.
- Death printing should remain centralized and must print exactly once.
- Do not create duplicate death paths.

Implementation direction:
- Add a small helper that checks whether a philosopher is still alive based on
  last_meal and time_to_die.
- Protect last_meal with the existing meal mutex.
- Use this helper inside or immediately before non-death logging.
- Keep lock ordering simple and avoid deadlock with print/state/meal locks.

### 2. Stop meal-count simulations immediately and deterministically

Problem:
The mandatory program can allow an extra eating cycle after every philosopher has
already reached number_of_times_each_philosopher_must_eat.

Required behavior:
- When a philosopher's meal count reaches the required limit, mark that
  philosopher as full exactly once.
- Track how many philosophers are full under synchronization.
- When the last philosopher becomes full, set the global finished state
  immediately.
- Avoid extra fork/eat logs after the quota is already satisfied.
- Keep the optional meal-count 0 policy as immediate successful completion.

Implementation direction:
- Add per-philosopher completion state if needed.
- Add a synchronized full_count or equivalent table-level state if needed.
- Update completion in the eating path after the meal is completed and forks can
  be released safely.
- Remove or simplify polling-only completion logic if appropriate.

### 3. Preserve death comparison policy

Do not change the death condition to `>`.
Keep the policy:
current_time_ms() - last_meal >= time_to_die means dead.

## Bonus fixes

### 1. Fix one-philosopher bonus behavior

Problem:
philo_bonus with one philosopher can print only one fork message and exit without
a death message.

Required behavior:
- The one-philosopher bonus case must deterministically:
  1. take one fork;
  2. print exactly one fork message;
  3. wait until time_to_die;
  4. print exactly one death message;
  5. exit with an explicit death status.
- Do not rely on a detached monitor thread for this one-philosopher case.
- Do not print further state messages after death.

### 2. Add explicit child exit semantics

Problem:
The parent currently treats any child exit the same way.

Required behavior:
- Define explicit child exit codes for:
  - normal/full meal completion
  - death
  - setup/error
- The parent must inspect waitpid status with WIFEXITED and WEXITSTATUS.
- If a child exits because of death, kill and reap all remaining children.
- If all children exit because they completed the required meals, finish cleanly.
- If a child exits because of setup/error or abnormal status, kill and reap all
  remaining children and return failure.
- Avoid zombie processes.

### 3. Clean bonus meal-quota shutdown

Problem:
Children can continue looping after reporting meal completion, and the parent
can kill them while they are printing.

Required behavior:
- When number_of_times_each_philosopher_must_eat is provided, a child that
  reaches the quota should stop cleanly after releasing forks.
- Avoid parent-side SIGTERM during normal successful meal-count completion.
- Do not kill children mid-log during normal completion.
- Avoid truncated or malformed log lines.

Implementation direction:
- Make eating or child loop return a status when quota is reached.
- Release forks before exiting.
- Exit with the normal/full completion code.
- Let parent reap children and count normal completions.

### 4. Bonus normal-log guard

Add a child-local guard so a child does not print normal fork/eat/sleep/think
logs after it has reached its own death deadline.

Do not create duplicate death messages.
The death monitor or deterministic one-philosopher path should remain
responsible for death printing.

### 5. Bonus parent failure cleanup

If parent watcher/thread setup remains in the design and fails, kill and reap
already-created children before returning failure.

If you redesign the parent to avoid a watcher thread, make sure the resulting
waitpid loop handles death, completion, and error statuses correctly.

## Keep these policies unchanged unless absolutely necessary

- Keep optional meal count 0 as immediate successful completion.
- Keep the implementation under implementation/philo and
  implementation/philo_bonus.
- Keep README first line with login jkim2.
- Do not move files to root-level philo or philo_bonus in this session.

## Required validation

Run from /Users/jihoonkim/42/Philosophers only.

Build:
- make -C implementation/philo re
- make -C implementation/philo
- make -C implementation/philo_bonus re
- make -C implementation/philo_bonus

Norm:
- If norminette is available, run:
  norminette implementation/philo implementation/philo_bonus
- Fix all Norm errors.
- If norminette is not available, manually review norm.md constraints and say so.

Mandatory tests:
- ./implementation/philo/philo 1 800 200 200
- ./implementation/philo/philo 2 60 60 60
- ./implementation/philo/philo 3 800 200 200 1
- ./implementation/philo/philo 5 800 200 200 7
- ./implementation/philo/philo 4 410 200 200
- ./implementation/philo/philo 4 310 200 100

Bonus tests:
- ./implementation/philo_bonus/philo_bonus 1 800 200 200
- ./implementation/philo_bonus/philo_bonus 2 60 60 60
- ./implementation/philo_bonus/philo_bonus 3 200 20 20 1
- ./implementation/philo_bonus/philo_bonus 3 800 200 200 1
- ./implementation/philo_bonus/philo_bonus 5 800 200 200 7
- ./implementation/philo_bonus/philo_bonus 4 410 200 200
- ./implementation/philo_bonus/philo_bonus 4 310 200 100

Invalid input:
Run a small invalid-input sweep for both programs:
- no arguments
- too few arguments
- too many arguments
- non-numeric values
- negative values
- zero philosophers
- zero time values
- overflowing values
- optional meal target 0

Review-specific regressions:
- For ./implementation/philo/philo 2 60 60 60, verify there is no normal state
  message after the philosopher is already dead.
- For ./implementation/philo/philo 3 800 200 200 1, verify the program stops
  after all philosophers reach the quota without starting unnecessary extra
  meals.
- For ./implementation/philo_bonus/philo_bonus 1 800 200 200, verify exactly one
  death message is printed.
- For ./implementation/philo_bonus/philo_bonus 3 200 20 20 1, verify there are
  no truncated or malformed log lines.

## Documentation updates

Update implementation/AI_IMPLEMENTATION_NOTES.md after the fixes.

The notes must accurately describe:
- mandatory death-boundary handling
- mandatory meal-count completion handling
- bonus one-philosopher handling
- bonus parent/child exit-status strategy
- bonus meal-count shutdown strategy
- tests run and observed results
- remaining limitations, if any

Do not overstate guarantees that were not verified.

## Final response

At the end, provide:
- concise summary of code changes
- files modified
- mandatory build result
- bonus build result
- Norm result
- tests run and observed results
- whether the original review findings were fixed
- any remaining concerns
