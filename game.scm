#!/usr/bin/env guile
!#
(load "core.scm")

(use-modules (play core))

(define width 800)
(define height 600)

(define red '(255 0 0 255))
(define green '(0 255 0 255))
(define blue '(0 0 255 255))
(define black '(0 0 0 255))
(define white '(255 255 255 255))
(define purple '(127 0 127 255))
(define orange '(255 165 0 255))

(define ctx (create-play-context "Example Game" width height))
(define player (load-texture ctx "assets/player.bmp" white))
(define font (open-font "assets/font.ttf" 24))

(define player-x 0)
(define player-y 0)

(define player-vx 0)
(define player-vy 0)

(define player-w (car (texture-size player)))
(define player-h (cdr (texture-size player)))

(define running #t)
(define splash #t)

(define render-splash (lambda ()
    (render-clear ctx purple)
    (render-texture-xy ctx (font-render-text-solid ctx font "Press Enter to begin" orange) '(300 . 120))
    (render-present ctx)))

(define render-game (lambda ()
    (render-clear ctx green)
    (render-texture-xy ctx player `(,player-x . ,player-y))
    (render-present ctx)))

(define render (lambda ()
    (cond (splash (render-splash))
          (else (render-game)))))

(define input-splash (lambda ()
    (when (key-down ctx KEY_RETURN)
          (set! splash #f))))

(define input-game (lambda ()
  (when (key-down ctx KEY_w)
    ;(set! player-vy -4))
    (jump))
  (when (key-down ctx KEY_a)
    (set! player-vx -4))
  (when (key-down ctx KEY_s)
    (set! player-vy 4))))

(define input (lambda ()
  (when (close-requested ctx)
    (set! running #f))
  (player-update)
  (if splash (input-splash)
    (input-game))))

(define gravity (lambda ()
  (set! player-vy (+ player-vy 1))))

(define jump (lambda ()
  (write "jump") (newline)
  (when (= player-vy 0)
    (set! player-vy -40))))

(define player-update (lambda ()
  (gravity)
  (set! player-x (+ player-x player-vx))
  (set! player-y (+ player-y player-vy))
  (when (< player-x 0)
    (set! player-x 0))
  (when (< player-y 0)
    (set! player-y 0))
  (when (> (+ player-x player-w -1) width)
    (set! player-x (- width player-w -1)))
  (when (> (+ player-y player-h -1) height)
    (set! player-y (- height player-h -1)))))

(while running
    (poll-input ctx)
    (render)
    (input))

(destroy-context ctx)