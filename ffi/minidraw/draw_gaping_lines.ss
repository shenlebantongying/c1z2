(import
 (minidraw_chez)
 (scheme))

(define width 500)
(define height 50)

(define (draw-line x1 y1 x2 y2)
  (moveTo x1 y1)
  (lineTo x2 y2)
  (stroke))

(define (draw-vertical-line x)
  (draw-line x 0 x height))

(initDrawBoard width 50)
(setColor 255 0 0)

(define (da xt inc)
  (when (< xt width)
    (begin 
      (draw-vertical-line xt)
      (da (+ xt inc) (+ inc 10)))))

(da 0 0)

(savePng "ok.png")
