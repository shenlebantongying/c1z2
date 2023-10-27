#!/usr/bin/env racket
#lang racket

(require "minidraw.rkt")

(define width 500)
(define height 500)

(define (draw-line x1 y1 x2 y2)
  (moveTo x1 y1)
  (lineTo x2 y2)
  (stroke))

(initDrawBoard width height)

(setColor 255 0 0)

(for ([x (in-inclusive-range 0 width (/ width 10))])
  (draw-line x 0 x height))
(for ([y (in-inclusive-range 0 height (/ height 10))])
  (draw-line 0 y width y))

(savePng "grid.png")
