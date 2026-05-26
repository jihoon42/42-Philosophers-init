# Review Goal: 42 Philosophers Code Review

You are reviewing a 42 Common Core “Philosophers” implementation.

## Absolute sandbox boundary

Treat this directory as the absolute project root:

/Users/jihoonkim/42/Philosophers

Never read, list, search, modify, create, delete, or reference files outside this
directory. Do not use `..` to access any parent directory. Do not inspect the
home directory, shell history, global git config, SSH keys, credentials, package
caches, or any external path. Do not use the network.

This is a code review session, not an implementation session.

Default mode:
- Read and analyze.
- Run safe local build/test commands if useful.
- Do not edit, patch, reformat, delete, or generate source files.
- Do not modify SUBJECT_Philosophers.md, norm.md, goal.md, or this file.
- Do not install packages.
- Do not leave long-running processes alive.

You may inspect only files under the project root. Relevant possible project
layouts are:
- implementation/philo
- implementation/philo_bonus
- philo
- philo_bonus
- README.md
- SUBJECT_Philosophers.md
- norm.md

If both `implementation/...` and root-level `philo` / `philo_bonus` exist,
review all present implementations and clearly state which one each finding
refers to.

## Main review objective

Perform a strict, evidence-based review of both mandatory and bonus parts.

Do not only check whether the code “works once”. Evaluate whether it is likely
to pass 42 peer evaluation and hidden edge cases.

For every issue, include:
- severity: BLOCKER, MAJOR, MINOR, or QUESTION
- file and line reference
- what is wrong
- why it matters for the subject, Norm, or runtime behavior
- how to reproduce or verify it, if applicable
- a concise suggested fix direction, without patching the code

Severity guide:
- BLOCKER: build failure, Norm failure, unauthorized function, global variable,
  data race, deadlock, missing mandatory behavior, wrong binary/layout, broken
  death handling, zombie/leaking processes, or anything likely to cause 0.
- MAJOR: intermittent timing failure, fragile synchronization, incomplete cleanup,
  weak input parsing, likely edge-case failure, or non-deterministic output.
- MINOR: maintainability, README clarity, small Makefile polish, weak notes.
- QUESTION: ambiguous behavior that needs the author’s decision.

## Use subagents

If the environment supports subagents, create and run these review agents.
If subagents are not available, simulate them sequentially using the same roles.

### Agent 1 — Layout, Build, Makefile, Submission Compliance

Check:
- mandatory program name `philo`
- bonus program name `philo_bonus`
- correct directories according to the implementation layout
- required files: Makefile, .h, .c
- Makefile rules: NAME, all, clean, fclean, re
- bonus rule if the chosen layout uses one Makefile for both parts
- compiler is `cc`
- flags include `-Wall -Wextra -Werror`
- no unnecessary relinking
- source files explicitly listed; no wildcard source expansion
- no libft
- README.md exists at repository root
- README first line uses login `jkim2`
- README is in English and includes AI usage note if present in requirements

Run when possible:
- make -C implementation/philo
- make -C implementation/philo re
- make -C implementation/philo
- make -C implementation/philo_bonus
- make -C implementation/philo_bonus re
- make -C implementation/philo_bonus
- or equivalent commands for root-level philo/philo_bonus

### Agent 2 — Norm and Authorized Function Audit

Check Norm constraints:
- 42 header in .c and .h files if applicable
- lowercase snake_case identifiers
- struct names start with s_
- typedef names start with t_
- no global variables
- max 25 lines per function
- max 80 columns per line
- real tabs for indentation
- declarations at beginning of functions
- max 4 named parameters per function
- max 5 local variables per function
- one declaration per line
- no declaration and initialization on same line inside functions
- return values in parentheses unless void
- no comments inside function bodies
- no struct declarations in .c files
- include guards in headers
- no unused includes
- no .c file includes another .c file
- max 5 function definitions per .c file
- forbidden: for, do while, switch, case, goto, ternary operator, VLA,
  implicit types, multiline macros

Audit unauthorized functions.

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

Flag calls such as atoi, strtol, strlen, strcmp, perror, strerror, sleep, time,
getpid, signal, pthread_mutex_* in bonus, sem_* in mandatory, or any other
unlisted external function.

Run `norminette` if available, only inside project root and only on the project
files. If unavailable, say so and perform manual Norm review.

### Agent 3 — Mandatory Threads/Mutexes/Timing Review

Review mandatory `philo`.

Check:
- one pthread per philosopher
- one mutex per fork
- fork ownership cannot be duplicated
- every shared field is protected:
  - stop/death flag
  - last_meal timestamp
  - meal count
  - finished count
  - print state
- no data races
- no lock-order deadlock
- no missed unlock on error/early return
- no detached thread that is later joined
- all created threads are joined or safely detached
- mutexes are destroyed after use
- malloc failure and pthread_create failure are handled with cleanup
- one philosopher case is explicit and correct
- death can be detected while a philosopher is eating, sleeping, thinking, or
  waiting for forks
- precise sleep uses gettimeofday/usleep in small increments
- death is printed within 10 ms
- after death, no further state messages are printed
- optional meal-count completion stops cleanly without false death
- timestamps are relative to simulation start
- last_meal is initialized before a philosopher can be declared dead
- philosopher numbering starts at 1
- philosopher 1 is adjacent to philosopher N

Pay special attention to:
- time_to_die <= time_to_eat
- time_to_die <= time_to_sleep
- all philosophers start at slightly different times
- monitor checks not being blocked by fork mutexes
- logging not racing with death detection
- completion race between “all ate enough” and “someone died”

### Agent 4 — Bonus Processes/Semaphores Review

Review bonus `philo_bonus`.

Check:
- one child process per philosopher
- main process does not act as a philosopher
- fork count represented by a counting semaphore
- printing serialized by semaphore
- no pthread mutexes in bonus
- named semaphores are unlinked before sem_open to avoid stale state
- semaphores are closed and unlinked during cleanup
- parent waits for children
- no zombie processes
- parent kills remaining children on death
- exactly one death message is printed
- no further state messages after death
- optional meal-count completion stops all children cleanly
- child process exit statuses are not confused between death and completion
- child-local monitor thread correctly detects that child’s death
- monitor thread does not race child routine over last_meal/eat_count
- one philosopher case works
- fork acquisition cannot deadlock

Pay special attention to the classic bonus deadlock:
if every process can take exactly one fork and then wait for a second fork
forever, the implementation is wrong. Look for an admission/waiter semaphore,
N - 1 strategy, paired fork acquisition strategy, or another defensible method.

Also check:
- child process cleanup path
- parent cleanup path
- behavior if fork fails after some children were already created
- behavior if sem_open fails
- repeated program runs do not break due to stale named semaphores

### Agent 5 — Input Parsing, Logs, and Edge Cases

Check both mandatory and bonus.

Arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]

Validate:
- exactly 4 or 5 user arguments
- non-numeric input rejected
- negative input rejected
- zero philosophers rejected
- zero time values handled according to project policy
- overflow rejected
- whitespace/sign handling is deliberate
- optional meal count zero is handled deliberately and documented or justified
- no unauthorized library parsing functions

Log format must be exactly:
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died

Check:
- exact spelling
- no extra prefixes/suffixes
- no overlapping messages
- timestamps non-negative and relative to start
- death printed once
- no logs after death
- no “sleeping/thinking” printed after simulation stopped
- meal-count completion produces no death message unless a real death happens

Suggested invalid inputs:
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
- 999999999999999999999999 800 200 200

### Agent 6 — Runtime Test and Stress Review

Run safe representative tests if binaries build.

Use a timeout mechanism when available. If `timeout` is unavailable, try
`gtimeout`. If neither is available, run short observations only and make sure
to stop any long-running process safely.

Do not leave background processes running.

Representative tests for each binary:
- 1 800 200 200
  Expected: takes one fork, dies around 800 ms, exactly one death.
- 2 60 60 60
  Expected: someone dies; no deadlock; death prompt enough.
- 3 800 200 200 1
  Expected: all eat once, program terminates, no death.
- 5 800 200 200 7
  Expected: all eat 7 times, program terminates, no death.
- 4 410 200 200
  Expected: should generally survive; observe for timing fragility.
- 4 310 200 100
  Expected: tight timing stress; look for premature death or stale logs.
- 5 150 80 80
  Expected: stressful; verify prompt death behavior.
- 199 800 200 200
  Expected: high concurrency stress if system allows; no immediate crash.

For long-running “should survive” tests, do not wait forever. Observe a bounded
period and report that the test was manually bounded.

If possible without editing files:
- run valgrind for mandatory if available
- run leaks on macOS if available
- run ThreadSanitizer via temporary command only if feasible
- never change the submitted Makefile flags permanently

### Agent 7 — Maintainability and Defense Readiness

Evaluate whether the author can explain the implementation during peer defense.

Check:
- clear data structures
- clear lifecycle: parse, init, start, monitor, stop, cleanup
- clear lock ownership
- small functions with real responsibilities, not Norm-avoidance spaghetti
- no magical sleeps that hide races
- no undocumented assumptions
- README and implementation notes match the code
- AI usage note is honest if AI was used
- reviewer can trace how death, completion, and cleanup happen

Also identify 3 to 5 likely peer-evaluation questions the author should be ready
to answer.

## Final consolidated report format

The lead reviewer must merge all subagent findings and remove duplicates.

Use this exact structure:

# 42 Philosophers Code Review

## 1. Review Scope
- directories reviewed
- commit/status if available
- files not reviewed and why
- commands run

## 2. Executive Verdict
Choose one:
- PASS-LIKELY
- PASS-MANDATORY-ONLY
- NEEDS-FIXES
- BLOCKED

Explain in 3 to 6 sentences.

## 3. Mandatory Verdict
- build result
- Norm result
- runtime result
- data-race/deadlock assessment
- top risks

## 4. Bonus Verdict
- build result
- Norm result
- runtime result
- process/semaphore assessment
- top risks
- explicitly state whether bonus should be evaluated only after mandatory fixes

## 5. Findings by Severity
Group by:
- BLOCKER
- MAJOR
- MINOR
- QUESTION

Each finding must include file:line evidence.

## 6. Edge Cases Tested
For each test:
- command
- observed result
- pass/fail/unknown
- notes

## 7. Subject Compliance Matrix
Include rows for:
- arguments
- logs
- death timing
- no data races
- mandatory thread/mutex model
- bonus process/semaphore model
- one philosopher
- meal-count completion
- cleanup
- README
- Makefile
- authorized functions

Use: PASS / FAIL / PARTIAL / NOT CHECKED.

## 8. Norm and API Audit
Summarize:
- Norm status
- unauthorized functions, if any
- suspicious includes/macros
- Makefile relinking status

## 9. Most Important Fixes
List the top fixes in priority order. Do not patch the code.

## 10. Defense Questions
List questions the author should be able to answer.

## 11. Review Limitations
State what could not be verified and why.
