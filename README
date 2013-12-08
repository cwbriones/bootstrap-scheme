Bootstrap Scheme
================

A scheme interpreter written in C++.

This isn't meant to be used in production, instead it is simply an attempt
to implement the scheme programming language from scratch, to be able to 
bootstrap a scheme compiler.

See http://michaux.ca/articles/scheme-from-scratch-introduction for inspiration

To build and run from bash:
```
$ mkdir build & cd build
$ cmake ..
$ make
$ ./scheme
```

Some things you can do with this already:
```
Welcome to Bootstrap Scheme.
Use ctrl-c to exit.
>>> 1
1
>>> (define x 5)
ok
>>> x
5
>>> (define (factorial n)
        (let loop ((i n)) (if (= i 0) 1 (* i (loop (- i 1))))))
ok
>>> (factorial x)
120
>>> (define nums (range 1 11))
ok
>>> nums
(1 2 3 4 5 6 7 8 9 10)
>>> (fold-left 0 + nums)
55
>>> (make-initialized-vector 6 (lambda (x) (* x x)))
#(0 1 4 9 16 25)
>>> ^C
```

Copying
-------

See the COPYING file for legal information.
