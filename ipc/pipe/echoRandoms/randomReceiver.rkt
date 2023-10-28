#lang racket

(define in (open-input-file "my.fifo"))

(define (infinity-reading)
  (let ([s (read-line in)])
    (if (equal? s eof)
        (write "eof")
        (writeln (string-trim s)))
    (flush-output))
  (infinity-reading))

(infinity-reading)
