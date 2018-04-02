#!/usr/bin/env guile
!#
(load "core.scm")

(use-modules (play core))

(define ctx (create-play-context "Hello Game" 800 600))

(define face (load-texture ctx "face.bmp"))

(define red '(255 0 0 255))
(define green '(0 255 0 255))
(define blue '(0 0 255 255))
(define black '(0 0 0 255))

(define bg-color black)

(define face-x 0)
(define face-y 0)

(define running #t)
(define mouse (cons 0 0))
(while running
    (poll-input ctx)
    (set! mouse (mouse-pos ctx))
    (render-clear ctx bg-color)
    (render-texture-xy ctx face `(,face-x . ,face-y))
    (render-present ctx)
    ;; (write (car mouse)) (write-char #\space) (write (cdr mouse)) (newline)
    (when (close-requested ctx)
        (set! running #f))
    (when (key-down ctx KEY_1)
        (set! bg-color red))
    (when (key-down ctx KEY_2)
        (set! bg-color green))
    (when (key-down ctx KEY_3)
        (set! bg-color blue))
    (when (key-down ctx KEY_4)
        (set! bg-color black))
    (when (key-down ctx KEY_w)
        (set! face-y (+ face-y -1)))
    (when (key-down ctx KEY_a)
        (set! face-x (+ face-x -1)))
    (when (key-down ctx KEY_s)
        (set! face-y (+ face-y 1)))
    (when (key-down ctx KEY_d)
        (set! face-x (+ face-x 1))))

(destroy-texture face)
(destroy-play-context ctx)