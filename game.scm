#!/usr/bin/env guile
!#
(load "core.scm")

(use-modules (play core))

(define width 800)
(define height 600)

(define ctx (create-play-context "Example Game" width height))

(define face (load-texture ctx "assets/face.bmp"))

(define red '(255 0 0 255))
(define green '(0 255 0 255))
(define blue '(0 0 255 255))
(define black '(0 0 0 255))

(define bg-color black)

(define face-x 0)
(define face-y 0)

(define face-w (car (texture-size face)))
(define face-h (cdr (texture-size face)))

(define running #t)
(define mouse (cons 0 0))

(write "Press 1-4 to change bg color") (newline)
(write "Press WASD to move face") (newline)

(while running
    (poll-input ctx)
    (set! mouse (mouse-pos ctx))
    (render-clear ctx bg-color)
    (render-texture-xy ctx face `(,face-x . ,face-y))
    (render-present ctx)
    (when #f
        (write (car mouse)) (write-char #\space) (write (cdr mouse)) (newline))
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
        (set! face-x (+ face-x 1)))
    (when (< face-x 0)
        (set! face-x 0))
    (when (< face-y 0)
        (set! face-y 0))
    (when (> (+ face-x face-w -1) width)
        (set! face-x (- width face-w -1)))
    (when (> (+ face-y face-h -1) height)
        (set! face-y (- height face-h -1))))
