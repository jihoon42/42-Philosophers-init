*Non-essential content has been hidden using `[...]`.*

# Philosophers

**I never thought philosophy would be so deadly**

**Summary:** In this project, you will learn the basics of threading a process. You will learn how to create threads and explore the use of mutexes.

**Version:** 13.0

---

## Chapter I — Introduction
[...]

---

## Chapter II — Common Instructions

- Your project must be written in C.
- Your project must be written in accordance with the Norm. If you have bonus files/functions, they are included in the norm check, and you will receive a 0 if there is a norm error.
- Your functions should not quit unexpectedly (segmentation fault, bus error, double free, etc.) except for undefined behavior. If this occurs, your project will be considered non-functional and will receive a 0 during the evaluation.
- All heap-allocated memory must be properly freed when necessary. Memory leaks will not be tolerated.
- If the subject requires it, you must submit a Makefile that compiles your source files to the required output with the flags `-Wall`, `-Wextra`, and `-Werror`, using `cc`. Additionally, your Makefile must not perform unnecessary relinking.
- Your Makefile must contain at least the rules `$(NAME)`, `all`, `clean`, `fclean` and `re`.
- To submit bonuses for your project, you must include a `bonus` rule in your Makefile, which will add all the various headers, libraries, or functions that are not allowed in the main part of the project. Bonuses must be placed in `_bonus.{c/h}` files, unless the subject specifies otherwise. The evaluation of mandatory and bonus parts is conducted separately.
- If your project allows you to use your libft, you must copy its sources and its associated Makefile into a `libft` folder. Your project's Makefile must compile the library by using its Makefile, then compile the project.
- We encourage you to create test programs for your project, even though this work **does not need to be submitted and will not be graded**. It will give you an opportunity to easily test your work and your peers' work. You will find these tests especially useful during your defence. Indeed, during defence, you are free to use your tests and/or the tests of the peer you are evaluating.
- Submit your work to the assigned Git repository. Only the work in the Git repository will be graded. If Deepthought is assigned to grade your work, it will occur after your peer-evaluations. If an error happens in any section of your work during Deepthought's grading, the evaluation will stop.

---

## Chapter III — AI Instructions
[...]

---
## Chapter V — Global rules

You have to write a program for the mandatory part and another one for the bonus part (if you decide to do the bonus part). They both have to comply with the following rules:

- Global variables are forbidden!
- Your program(s) must take the following arguments:
  `number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]`
  - `number_of_philosophers`: The number of philosophers and also the number of forks.
  - `time_to_die` (in milliseconds): If a philosopher has not started eating within `time_to_die` milliseconds since the start of their last meal or the start of the simulation, they die.
  - `time_to_eat` (in milliseconds): The time it takes for a philosopher to eat. During that time, they will need to hold two forks.
  - `time_to_sleep` (in milliseconds): The time a philosopher will spend sleeping.
  - `number_of_times_each_philosopher_must_eat` (optional argument): If all philosophers have eaten at least `number_of_times_each_philosopher_must_eat` times, the simulation stops. If not specified, the simulation stops when a philosopher dies.
- Each philosopher has a number ranging from 1 to `number_of_philosophers`.
- Philosopher number 1 sits next to philosopher number `number_of_philosophers`. Any other philosopher, numbered N, sits between philosopher N - 1 and philosopher N + 1.

### About the logs of your program

- Any state change of a philosopher must be formatted as follows:
  - `timestamp_in_ms X has taken a fork`
  - `timestamp_in_ms X is eating`
  - `timestamp_in_ms X is sleeping`
  - `timestamp_in_ms X is thinking`
  - `timestamp_in_ms X died`

  Replace `timestamp_in_ms` with the current timestamp in milliseconds and `X` with the philosopher number.

- A displayed state message should not overlap with another message.
- A message announcing a philosopher's death must be displayed within 10 ms of their actual death.
- Again, philosophers should avoid dying!

> **Your program must not have any data races.**

---

## Chapter VI — Mandatory part

| Field | Value |
|---|---|
| **Program Name** | `philo` |
| **Files to Submit** | Makefile, *.h, *.c, in directory `philo/` |
| **Makefile** | NAME, all, clean, fclean, re |
| **Arguments** | `number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]` |
| **External Function** | `memset`, `printf`, `malloc`, `free`, `write`, `usleep`, `gettimeofday`, `pthread_create`, `pthread_detach`, `pthread_join`, `pthread_mutex_init`, `pthread_mutex_destroy`, `pthread_mutex_lock`, `pthread_mutex_unlock` |
| **Libft authorized** | No |
| **Description** | Philosophers with threads and mutexes |

The specific rules for the mandatory part are:

- Each philosopher must be represented as a separate thread.
- There is one fork between each pair of philosophers. Therefore, if there are several philosophers, each philosopher has a fork on their left side and a fork on their right side. If there is only one philosopher, they will have access to just one fork.
- To prevent philosophers from duplicating forks, you should protect each fork's state with a mutex.

---

## Chapter VII — Readme Requirements

A `README.md` file must be provided at the root of your Git repository. Its purpose is to allow anyone unfamiliar with the project (peers, staff, recruiters, etc.) to quickly understand what the project is about, how to run it, and where to find more information on the topic.

The `README.md` must include at least:

- The very first line must be italicized and read: *This project has been created as part of the 42 curriculum by \<login1\>[, \<login2\>[, \<login3\>[...]]]*
- A **"Description"** section that clearly presents the project, including its goal and a brief overview.
- An **"Instructions"** section containing any relevant information about compilation, installation, and/or execution.
- A **"Resources"** section listing classic references related to the topic (documentation, articles, tutorials, etc.), as well as a description of how AI was used — specifying for which tasks and which parts of the project.
- Additional sections may be required depending on the project (e.g., usage examples, feature list, technical choices, etc.).

Any required additions will be explicitly listed below.

> **Your README must be written in English.**

---

## Chapter VIII — Bonus part

| Field | Value |
|---|---|
| **Program Name** | `philo_bonus` |
| **Files to Submit** | Makefile, *.h, *.c, in directory `philo_bonus/` |
| **Makefile** | NAME, all, clean, fclean, re |
| **Arguments** | `number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]` |
| **External Function** | `memset`, `printf`, `malloc`, `free`, `write`, `fork`, `kill`, `exit`, `pthread_create`, `pthread_detach`, `pthread_join`, `usleep`, `gettimeofday`, `waitpid`, `sem_open`, `sem_close`, `sem_post`, `sem_wait`, `sem_unlink` |
| **Libft authorized** | No |
| **Description** | Philosophers with processes and semaphores |

The program of the bonus part takes the same arguments as the mandatory program. It has to comply with the requirements of the Global rules chapter.

The specific rules for the bonus part are:

- All the forks are put in the middle of the table.
- They have no states in memory, but the number of available forks is represented by a semaphore.
- Each philosopher must be represented as a separate process. However, the main process should not act as a philosopher.

> **The bonus part will only be assessed if the mandatory part is PERFECT. Perfect means the mandatory part has been integrally done and works without malfunctioning. If you have not passed ALL the mandatory requirements, your bonus part will not be evaluated at all.**

---

## Chapter IX — Submission and peer-evaluation

Submit your assignment in your Git repository as usual. Only the work inside your repository will be evaluated during the defense. Don't hesitate to double-check the names of your files to ensure they are correct.

- Mandatory part directory: `philo/`
- Bonus part directory: `philo_bonus/`

During the evaluation, a brief **modification of the project** may occasionally be requested. This could involve a minor behavior change, a few lines of code to write or rewrite, or an easy-to-add feature.

While this step may **not be applicable to every project**, you must be prepared for it if it is mentioned in the evaluation guidelines.

This step is meant to verify your actual understanding of a specific part of the project. The modification can be performed in any development environment you choose (e.g., your usual setup), and it should be feasible within a few minutes — unless a specific timeframe is defined as part of the evaluation.

You can, for example, be asked to make a small update to a function or script, modify a display, or adjust a data structure to store new information, etc.

The details (scope, target, etc.) will be specified in the **evaluation guidelines** and may vary from one evaluation to another for the same project.