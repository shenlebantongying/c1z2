(library (ffi)
  (export
   load-dynamic-lib
   def-cfun
   )
  (import (chezscheme))

  ;; load-shared-object but with (machine-type)
  (define (load-dynamic-lib libname)
    (load-shared-object
     (string-append
      (case (machine-type)
	[(a6le ta6le) (string-append libname ".so")]
	[(arm64osx tarm64osx) ".dylib"]
	[else (display-string "PUT MORE MACHINETYPE")]))))

  (define-syntax def-cfun
    (syntax-rules ()
      ((_  name args -> ret)
       (define name
         (foreign-procedure (symbol->string 'name) args ret)))))
  )
