# Hand-in questions SV21POTR Course A

SV21POTR - Examination Assignment - SV21POTR Course A - Joakim Odermalm

## Agile Software Development

1. What is Scrum? (G)

- A collaborative work model. [1]

2. Name and briefly explain the pillars of Scrum. (G)

- Transparancy: All roles understand process, process tools, project vision. Common _definition of done_ for backlog items.
- Inspection: Regular practice, find opportunities for improvement.
- Adaptation: _Respond to change_ - adapt to optimize project outcome.

[1]

3. Name and briefly explain the roles in a Scrum team. (G)

- Product owner: Manages backlog, accepts or rejects work done by Dev team.
- Development team: Organizes their own work.
- Scrum master: Supports the Scrum work model.

[1]

4. Name and briefly explain the Scrum artifacts. (G)

- Backlog: Prioritized set of TODOs.
- Sprint backlog: Subset of TODOs selected for sprint.
- Product increment: Releasable product (_Working software_) where sprint backlog is done.

[1]

5. Name and briefly explain the Scrum events. (G)

- Sprint: Timeboxed (weeks) work phase, goal: Product increment.
- Sprint planning: Timeboxed (hours) meeting of Team, Master, Owner. goal: Select sprint backlog.
- Daily scrum: Timeboxed (15 min) standup meeting of Team and Master. goals: Inspect progress, optimize collaboration and progression.
- Sprint review: Timeboxed (hour / sprint week) meeting of planning party and stakeholders. Goals: Inspect product increment, adapt backlog, update stakeholders (demo).
- Sprint retrospective: Timeboxed (3/4ths of review) meeting of Team and Master. Goals: Inspect process, plan improvment.

[1]

6. T-shirt Sizing is a Scrum story point estimation technique. Explain it. (VG)

- T-shirt sizes (S, M, L , XL) are used as buckets for relative estimation of work size for story point. Implementation may include using cards marked with sizes and a process to reach agreement. [2]

## Functional Safety

1. What is functional safety?(G)

- No unreasonable risk of harm or damage caused by a malfunctioning system. [3]

2. Briefly explain the V-model used by ISO 26262 to develop products at software level (G)

- Process flow is represented as a V-shape with detail levels of documentation on left, implementation at the point and detail levels of validation on right. Documentation and validation have correspondance on each level.
[3]

3. Explain the following ISO 26262 guidelines in software development level

a. Use of defensive programming. Why? What is defensive programming? (VG)

- Defensive programming is designing software to continue functioning while (and after) experiencing unplanned issues. Defensive programming is of particular use when interacting with hardware and external inputs not controllable by the software. [6]

b. Use of language subsets like MISRA C. Why? What is MISRA C? (G)

- MISRA C is a subset of C for use in critical systems, in particular embedded systems. It aims to facilitate safety and security, and to provide reassurance of high code quality. [5]

c. Software unit testing. Why? What is unit testing? (G)

- Functional tests of atomic parts of the software. Helps find software defects early. Enables test-driven development. [4]

d. Software integration testing. Why? What is integration testing? (G)

- Functional tests of sets of integrated units of the software. Evaluate compliance with functional requirements. [4]

## Automotive software Architecture

1. What is software architecture and why do we need structured software? (VG) 

- A software architecture serves as a blueprint for a system. It allows oversight and collaboration. It may include both functional and non-functional requirements. [7] The term may also refer to a particular set of standards a specific software architecture may conform to, for example AUTOSAR. [3] Structured design facilitates understanding of the software by applying a "divide and conquer"-strategy. [8] This in turn enables or facilitates unit- and integration testing, agile workflows, code reuse and verification of standards compliance.

2. How can the architecture improve maintainability of a software? (VG)

- By providing standard interfaces and abstractions. [3] 
- Simplifying the system by applying a architectural strategy of hierarchical "black boxes". [9]
- By supporting use of known positive maintenance factors such as; code restructuring tools, defect tracking tools, automated change control tools and regression test libraries. [10]

3. What is AUTOSAR? (G)

- An industry standard for automotive software architecture. [3]

4. What is the main idea behind the RTE layer in AUTOSAR? (VG)

- To allow portability between different types of ECU for AUTOSAR software components. [3]

5. Explain the purposes of the following layers in the AUTOSAR basic software architecture

a. Microcontroller Abstraction Layer (G)

- Make higher software layers independent of microcontroller. [3]

b. ECU Abstraction Layer (G)

- Make higher software layers independent of ECU hardware. Provide API for hardware access. [3]

c. Services Layer (G)
  
- Provide operating system-like functionality. Memory, network, diagnostic services, etc. [3]

d. Complex Drivers (G)

- Make possible integration of non-standard, hardware dependent or timing critical devices. [3]

## References

[1] K. William, SV21POTR Course A Guest lecture, Sep. 13, 2021

[2] A. Mistry, Agile Story Point Estimation Techniques - T-Shirt Sizing. Accessed on: Sep. 16, 2021. [Online]. Available: <https://www.c-sharpcorner.com/article/agile-story-point-estimation-techniques-t-shirt-sizing/>

[3] F. Mehri, SV21POTR Course A Lecture series, Sep., 2021

[4] R. Black, D. Graham and E. v. Veenendaal, "Foundations of Software Testing", 3rd ed. Boston, MA: Cengage Learning EMEA, 2012.

[5] D. Ward and A. Banks, MISRA clarifies safe and secure uses of the C language. Accessed on: Sep. 25, 2021. [Online] Available: <https://web.archive.org/web/20200804032713/https://www.misra.org.uk/LinkClick.aspx?fileticket=JMG_JlEytqY%3D&tabid=59>  

[6] T. Hoffman, Defensive Programming - Friend or Foe?. Accessed on: Sep. 25, 2021. [Online]. Available: <https://interrupt.memfault.com/blog/defensive-and-offensive-programming>

[7] Tutorialspoint.com, Software Architecture & Design Introduction. Accessed on Sep. 25, 2021. [Online] Available: <https://www.tutorialspoint.com/software_architecture_design/introduction.htm>

[8] Tutorialspoint.com, Software Design Strategies. Accessed on Sep. 25, 2021. [Online] Available: <https://www.tutorialspoint.com/software_engineering/software_design_strategies.htm>

[9] M. Page-Jones, "The practical guide to structured systems design". New York, NY: Yourdon Press, 1980.

[10] C. Jones, "The Economics of Software Maintenace in the Twenty-first Century". Accessed on Sep 25, 2021. Available: <https://web.archive.org/web/20150319075401/http://www.compaid.com/caiinternet/ezine/capersjones-maintenance.pdf>