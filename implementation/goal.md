# Goal: 42 Philosophers with AI-only implementation

You are working on the 42 Common Core project “Philosophers”.

## Sandbox boundary

Treat this directory as the absolute project root:

/Users/jihoonkim/42/Philosophers

Never read, list, search, modify, create, delete, or reference files outside this
directory. Do not use `..` to access any parent directory. Do not inspect my home
directory, shell history, git config, SSH keys, credentials, caches, or any
external path. Do not use the network.

You may read only these existing documents:
- /Users/jihoonkim/42/Philosophers/SUBJECT_Philosophers.md
- /Users/jihoonkim/42/Philosophers/norm.md
- /Users/jihoonkim/42/Philosophers/implementation/goal.md

Do not modify SUBJECT_Philosophers.md, norm.md, or this goal file.

## Workspace

Put all implementation code inside:

/Users/jihoonkim/42/Philosophers/implementation

Create:
- implementation/philo
- implementation/philo_bonus
- implementation/AI_IMPLEMENTATION_NOTES.md

The only root-level file you may create is README.md, because the subject
requires a README at the root of the Git repository.

My 42 login is `jkim2`.

The first line of README.md must be exactly:

*This project has been created as part of the 42 curriculum by jkim2*

## Primary objective

Implement both mandatory and bonus parts from scratch in C.

Implementation order:
1. Read SUBJECT_Philosophers.md and norm.md.
2. Implement mandatory `philo`.
3. Build and test mandatory.
4. Only then implement bonus `philo_bonus`.
5. Build and test bonus.
6. Write README.md and AI_IMPLEMENTATION_NOTES.md.
7. Summarize files, tests, Norm status, and remaining concerns.

Do not redesign mandatory to fit bonus. They are separate programs with
different concurrency models.

## Common rules

Follow norm.md strictly.

Requirements:
- C only.
- No libft.
- No global variables.
- No data races.
- No crashes, segfaults, bus errors, double frees, leaks, or undefined behavior.
- Free heap allocations when necessary.
- Use only functions authorized by the subject for the relevant part.
- Handle invalid input cleanly.
- On input error, print a simple error message and exit non-zero.
- Do not busy-wait aggressively.

Arguments for both programs:

number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

Parsing:
- Accept exactly 4 or 5 user arguments.
- Reject non-numeric input.
- Reject negative values.
- Reject zero where invalid.
- Reject overflowing values.
- Use long or long long internally for milliseconds if needed.

Simulation:
- Philosophers are numbered from 1.
- Philosopher 1 sits next to philosopher N.
- A philosopher dies if they have not started eating within time_to_die ms since
  the start of their last meal or simulation start.
- If the optional meal-count argument is given, stop when all philosophers have
  eaten enough times.
- Otherwise, stop only when a philosopher dies.
- One philosopher can take only one fork and must eventually die.

Log format must be exactly one of:

timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died

Log rules:
- timestamp_in_ms is milliseconds since simulation start.
- Messages must not overlap.
- Print death within 10 ms of actual death.
- Print exactly one death message.
- After death, print no further philosopher state messages.

## Mandatory: implementation/philo

Program name: philo.

Files:
- Makefile
- .h files
- .c files

Makefile:
- NAME, all, clean, fclean, re
- cc
- -Wall -Wextra -Werror
- no unnecessary relinking
- explicitly list sources; no wildcard source expansion

Allowed mandatory functions only:
memset, printf, malloc, free, write, usleep, gettimeofday,
pthread_create, pthread_detach, pthread_join,
pthread_mutex_init, pthread_mutex_destroy,
pthread_mutex_lock, pthread_mutex_unlock

Architecture:
- One pthread per philosopher.
- One mutex per fork.
- Additional mutexes for printing and shared state.
- Use a monitor loop or monitor thread for death and meal completion.
- Protect every shared field.
- Use precise sleep with gettimeofday and usleep.
- Avoid deadlock, for example by alternating fork acquisition order or another
  clearly documented safe strategy.
- Handle the one-philosopher case explicitly.

## Bonus: implementation/philo_bonus

Program name: philo_bonus.

Files:
- Makefile
- .h files
- .c files

Makefile:
- NAME, all, clean, fclean, re
- cc
- -Wall -Wextra -Werror
- no unnecessary relinking
- explicitly list sources; no wildcard source expansion

Allowed bonus functions only:
memset, printf, malloc, free, write, fork, kill, exit,
pthread_create, pthread_detach, pthread_join,
usleep, gettimeofday, waitpid,
sem_open, sem_close, sem_post, sem_wait, sem_unlink

Architecture:
- One child process per philosopher.
- Main process must not act as a philosopher.
- Forks are in the middle of the table.
- Fork count is represented by a semaphore.
- Use a semaphore for printing.
- Use semaphores for inter-process coordination.
- Do not use pthread mutexes in bonus.
- Each child may use an internal monitor thread to detect its own death.
- If one philosopher dies, print exactly one death message and stop all children.
- If meal count is provided, stop when all philosophers have eaten enough.
- Parent must wait for and clean up children.
- No zombie processes.
- Unlink named semaphores before opening to avoid stale semaphores.
- Close and unlink semaphores during cleanup.
- Avoid deadlock; do not let every process hold one fork forever. Use an
  admission semaphore or another safe strategy.
- Handle the one-philosopher case explicitly.

## Norm constraints to respect

- 42 header in .c and .h files if possible.
- lowercase snake_case identifiers only.
- struct names start with s_.
- typedef names start with t_.
- no global variables.
- max 25 lines per function, excluding the function braces.
- max 80 columns per line.
- real tab indentation.
- declarations at the beginning of functions.
- max 4 named parameters per function.
- max 5 local variables per function.
- one variable declaration per line.
- no declaration and initialization on the same line inside functions.
- return values in parentheses unless void.
- no comments inside function bodies.
- no struct declarations in .c files.
- headers must have include guards.
- no unused includes.
- no .c file may include another .c file.
- max 5 function definitions per .c file.
- forbidden: for, do while, switch, case, goto, ternary operator, VLA, implicit
  types, multiline macros.

## README.md

Create /Users/jihoonkim/42/Philosophers/README.md in English.

Include:
- required first line with jkim2
- Description
- Instructions
- Usage examples for mandatory and bonus
- Project structure
- Technical overview
- Resources
- AI usage note explaining that AI assistance was used, what it was used for,
  and that the implementation was constrained to this project root.

## AI_IMPLEMENTATION_NOTES.md

Create implementation/AI_IMPLEMENTATION_NOTES.md.

Include:
- implementation summary
- mandatory architecture
- bonus architecture
- differences between mandatory and bonus
- synchronization strategy
- edge cases
- build commands
- manual tests and observed results
- known limitations
- assumptions
- note that all code was written under the filesystem restriction above
- note that actual 42 submission may require copying implementation/philo to
  philo and implementation/philo_bonus to philo_bonus

## Validation

Run from /Users/jihoonkim/42/Philosophers only.

Mandatory:
- make -C implementation/philo
- make -C implementation/philo re
- make -C implementation/philo again to check no unnecessary relinking

Bonus:
- make -C implementation/philo_bonus
- make -C implementation/philo_bonus re
- make -C implementation/philo_bonus again to check no unnecessary relinking

Test both programs with:
- 1 800 200 200
- 5 800 200 200
- 5 800 200 200 7
- 4 410 200 200
- 4 310 200 100
- 2 60 60 60
- 3 800 200 200 1

Also test invalid inputs:
- no arguments
- too few arguments
- too many arguments
- non-numeric values
- negative values
- zero philosophers
- zero time values
- overflowing values

If norminette is available, run it only on:
- implementation/philo
- implementation/philo_bonus

Fix all Norm errors. If unavailable, manually review norm.md and document that
norminette was unavailable.

Do not change final Makefile flags for debug builds. Remove temporary debug
artifacts before final summary.

## Final response

Implement the project, not just a plan.

At the end, summarize:
- files created
- mandatory build result
- mandatory tests and observed results
- bonus build result
- bonus tests and observed results
- Norm check status
- no-unnecessary-relinking status
- remaining concerns
