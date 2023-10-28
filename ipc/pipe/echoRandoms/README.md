A FIFO pipe and a Racket subprocess.

C side -> keeps writing random numbers into the pipe

Racket -> keeps reading from the pipe.

C side also keeps reading from the Racket side's output.

TODO: Bidirectional communication & reinvent some kind of RPC (popen2?)

TODO: embed racket rather than subprocess

