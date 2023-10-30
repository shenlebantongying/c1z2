#lang racket

(require ffi/unsafe
         ffi/unsafe/define)

(define-ffi-definer define-draw (ffi-lib "libminidraw"))
(define-draw initDrawBoard (_fun _int _int -> _void))
(define-draw setColor (_fun _int _int _int -> _void))
(define-draw lineTo (_fun _int _int  -> _void))
(define-draw moveTo (_fun _int _int -> _void))
(define-draw stroke (_fun -> _void))
(define-draw savePng (_fun _string -> _void))

(provide (all-defined-out))
