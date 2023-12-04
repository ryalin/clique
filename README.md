# Clique Decision Problem
15418 Final Project

## Summary: 

In this project, we will implement a parallelized algorithm using OpenMP and CUDA to determine the existence of a clique of size n given an undirected, unweighted graph. 

## Background:

A clique, or a complete subgraph, is a subset of vertices in a graph such that all the vertices are adjacent to each other. This problem has several real-life applications, such as determining mutual connection groups in a social network. 

There are many clique problems, for example, finding the max clique, listing all maximum cliques, finding all cliques greater than a size, etc. We will focus on the clique decision problem, which is NP-Complete. The clique decision problem is to find if a clique of size n exists in the graph or not. 

The goal of our project is to accelerate the solution to the problem using CUDA and OpenMP. This will benefit from parallelization because a singular thread or core will have to scan all the node combinations by itself. With multiple threads we can cut down on the search time significantly, either by assigning a thread to each node, breaking the graph into smaller subgraphs for computation, or something else.

## Initial Planned Schedule

| Time Period | Todo List | Assignment |
| :---------|:--------| :---------|
| 11/11 - 11/17 | - Research problem and solutions<br>- Work on project proposal | Ryan and Matt |
| 11/18 - 11/24 | - Start working on sequential algorithm and start on parallelization with OpenMP<br>- Write test cases | Matt (parallelization), Ryan (setup) |
| 11/25 - 12/1 | - Finish week 2 tasks and start parallelization with CUDA<br>- Start working on milestone report | Ryan and Matt |
| 12/2 - 12/8 | - Finish all parallelization, wrap up and benchmark solutions<br>- Fix any extraneous issues<br>- If time permits, improving parallelization and MPI | Ryan and Matt |
| 12/9 - 12/15 | - Write project report<br>- Make poster and continue benchmarking | Ryan and Matt |

## Work Completed (and Progress Updates)

### Week 1 (11/11 - 11/17)

This week was dedicated to finding and researching a problem, then working out feasibility and complexity. It was right on track, ending with completing the project proposal.

### Week 2 (11/18 - 11/24)

This week was dedicated to working on a sequential algorithm, and subsequently parallelizing with OpenMP. Test cases also had to be written in order to run benchmarks and determine correctness of any algorithms we write. Admittedly, we did not fulfill our goals for this week, due to several factors:

- Thanksgiving break was especially busy, with both partners travelling and not having any time for project work
- There was an underestimating of the amount of codebase setup needed other than parallelization, importing libraries, makefiles, testing functions, graph generation functions, etc. took longer than expected
- There was a frontloading of work on the initial planned schedule, with a majority of work scheduled to be completed before the 4th week and the time after that dedicated to extraneous work. Clearly a shifting of work in the entire timeline is needed

In the time that we did have, we set up a codebase, created necessary files, and implemented + fixed bugs on a rudimentary sequential algorithm

### Week 3 (11/25 - 12/1)

This week was dedicated to completing parallelization with OpenMP and starting CUDA parallelization. It is also dedicated to starting the milestone report. Because of week 2 being behind schedule, the work in week 3 was also behind. For week 3, we worked out bugs from the previous week, wrote python scripts to generate large testing files, created a faster (recursive) and less memory intensive sequential implementation, and worked on parallelization with OpenMP. We also cleaned up the codebase.

### Week 4 (12/2 - 12/8) (IN PROGRESS)

This week is still a work in progress, but the milestone report was done and submitted on 12/4/2023. At this moment in time, we have a small bit of OpenMP parallelization to do, with some benchmarking needed to ensure the speedup hits our targets of num_cores * 1/2. CUDA parallelization should start mid-late week. One roadblock we may face are exams in other courses, which should mostly wrap up around 12/6, leaving us ample time to work on the project for the rest of the time.

## Current Revised Schedule

| Time Period | Ryan's Todo List | Matt's Todo List |
| :---------|:--------| :--------|
| 11/11 - 11/17 | - Research problem and solutions<br>- Work on project proposal | Same as Ryan |
| 11/18 - 11/24 | - Setup codebase, write test cases | - Test parallization techniques |
| 11/25 - 12/1 | - Improving codebase, test file generation scripts | - Start OpenMP parallelization |
| 12/2 - 12/5 | - Benchmark OpenMP results, assist on parallelization | - Finish parallelization with OpenMP |
| 12/6 - 12/8 | - Clean up codebase and ensure robust testing, assistance on CUDA parallelization | - Start CUDA parallelization |
| 12/9 - 12/12 | - Finish parallelization with CUDA<br>-Benchmark results | Same as Ryan |
| 12/13 - 12/14 | - Final checks and benchmarks, writing final project report, prepare poster | Same as Ryan |

## Milestone Report

### Summary of work so far

In week 1, we researched and found an np-complete problem (clique decision) to solve. The week ended with the completion of a project proposal. In the subsequent weeks up until now, we set up a codebase, wrote scripts to generate testing files, wrote the sequential algorithm in multiple ways to find the best one in terms of speed, and started parallelizing using OpenMP. We also introduced some bug fixes to our existing code this week and are wrapping up this milestone report (inception).

### Thoughts on goals and deliverables

The goals and deliverables stated in the proposal still seem very much doable and achievable, but we are currently slightly behind schedule due to the nature of how we initially planned our work. Our original plan was to parallelizing the clique decision algorithm using OpenMP and CUDA, achieving at least ½ * core_count speedup, and we seem to be on track. 

In the original plan, we listed the “nice to haves”/“hope to achieve” as parallelizing the algorithm like above, with at least 0.75 * core_count speedup, the development of a comprehensive testing suite for all edge cases, and a MPI parallelization. We hope to improve the speedup and testing suite, with our current optimization, but do not believe there is time left in the schedule to explore possible parallelization with MPI.

New Goals: The new goals are largely the same as the old ones, except with the removal of parallelization with MPI, because that introduces an entirely new category of project work.

- At least a ½ * core_count speedup with both OpenMP and CUDA, with hopefully a 0.75 * core_count speedup.

- A larger testing suite than currently (currently there are 3 types of graphs with 3 different sizes each to test with)

### Plan with presentation

At the poster session, we plan to show graphs denoting the benchmark results obtained from parallelization with OpenMP and CUDA. If time allows, we may also develop a demo to show, where the audience can give us a clique size they want to search for in our graphs.

### Preliminary results

We currently do not have concrete preliminary results. We do have some initial speedups calculated from our OpenMP parallel implementation but still have to verify correctness.

### Concerns and issues

It seems to be just a matter of coding and doing the work, in the face of a busy schedule. We still are thinking of ways to parallelize our code the best way possible, but that will take some trial and error which takes lots of time.














