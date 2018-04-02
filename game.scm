#!/usr/bin/env guile
!#
(load "core.scm")

(use-modules (play core))

(define ctx (create-play-context "Hello Game" 800 600))

(define face (load-texture ctx "face.bmp"))

(define running #t)
(define mouse (cons 0 0))
(while running
    (poll-input ctx)
    (set! mouse (mouse-pos ctx))
    (write (car mouse)) (write-char #\space) (write (cdr mouse)) (newline)
    (when (close-requested ctx)
        (set! running #f))
    (when (key-pressed ctx SDLK_a)
        (write "A pressed")
        (newline))
    (when (key-pressed ctx SDLK_s)
        (write "S pressed")
        (newline))
    (when (key-pressed ctx SDLK_d)
        (write "D pressed")
        (newline))
    (when (key-pressed ctx SDLK_w)
        (write "W pressed")
        (newline)))

(destroy-texture face)
(destroy-play-context ctx)