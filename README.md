# small-shell
A shell that resembles UNIX shell, supports different set of commands.
This is based on an assignment given on OS course.
The shell supports IO, pipes, and complex commands (that requirs bin/bash files) in addition to these commands:
- chprompt: change prompt
- showpid: print current's process id
- pwd: print current (present) working directory
- cd: change directory
- jobs: print list of jobd running (unfinished and stopped)
- fg <job-id>: bring stopped process to foreground
- bg <job-id>: resume stopped process to background
- quit: exits shell
  quit kill: kills all unfinnished jobs before exiting
- kill <sig-num> <job-id>: send signal to process
- timeout <duration> <command>: sets an alarm for <duration> seconds for the executed <command>

This was made by Timna Smadja (timnas) and myself.
