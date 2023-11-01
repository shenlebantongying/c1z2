(library (minidraw)
  (export 
   version
   initDrawBoard
   setColor
   lineTo
   moveTo
   stroke
   savePng
   )
  (import (chezscheme) (ffi))
  
  ;; TODO: exactly where the library will be loaded?? Boundary between different libs??
  (define this_name_does_not_matter
    (load-dynamic-lib "../minidraw/libminidraw"))

  (def-cfun version () -> string)
  (def-cfun initDrawBoard (int int) -> void)
  (def-cfun setColor (int int int) -> void)
  (def-cfun lineTo (int int) -> void)
  (def-cfun moveTo (int int) -> void)
  (def-cfun stroke () -> void)
  (def-cfun savePng (string) -> void)
  )
