(import
 (ffi)
 (chezscheme))

(load-dynamic-lib "../ftype/libpointer-types")

(define-ftype sab-struct
  (struct
    [a integer-32]
    [b integer-32]))

(define plus_sab
  (foreign-procedure "plus_sab" ((* sab-struct)) int))

(define my_sab
  (make-ftype-pointer sab-struct
		      (foreign-alloc
		       (ftype-sizeof sab-struct))))

(ftype-set! sab-struct (a) my_sab 100)
(ftype-set! sab-struct (b) my_sab -300)

(display (format "Result -> ~D\n" (plus_sab my_sab)))
(display (format "Address -> ~X from Chez"  (ftype-pointer-address my_sab)))

(foreign-free (ftype-pointer-address my_sab))
