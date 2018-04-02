#!/usr/bin/env guile
!#
(load "core.scm")

(use-modules (play core))

(define ctx (play-create-context "title"))

(sleep 2)

(play-destroy-context ctx)