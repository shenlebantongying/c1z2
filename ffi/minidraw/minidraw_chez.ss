(library (minidraw_chez)
  (export 
   version
   initDrawBoard
   setColor
   lineTo
   moveTo
   stroke
   savePng
   )
  (import (chezscheme))
  
  ;; TODO: exactly where the library will be loaded?? Boundary between different libs??
  (define this_name_does_not_matter
    (load-shared-object (case (machine-type)
			  [(a6le ta6le) "./libminidraw.so"]
			  [(arm64osx tarm64osx) "./libminidraw.dylib"]
			  [else (display-string "PUT MORE MACHINETYPE")])))
  
  (define-syntax def-cfun
    (syntax-rules ()
      ((_  name args -> ret)
       (define name
         (foreign-procedure (symbol->string 'name) args ret)))))

  (def-cfun version () -> string)
  (def-cfun initDrawBoard (int int) -> void)
  (def-cfun setColor (int int int) -> void)
  (def-cfun lineTo (int int) -> void)
  (def-cfun moveTo (int int) -> void)
  (def-cfun stroke () -> void)
  (def-cfun savePng (string) -> void)
  )
