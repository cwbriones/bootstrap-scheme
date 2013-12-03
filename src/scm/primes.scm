(define (range first last)
    (if (> first (- last 1))
        '()
        (cons first (range (+ first 1) last))
    )
)

(define (divides? n m) (= (modulo m n) 0))

(define (remove-multiples n ns)
  (filter (lambda (x) (not (divides? n x))) ns)
)

(define (primes n)
    (define (erat prime-list candidates)
        (if (null? candidates) 
            prime-list
            (erat 
                (cons (car candidates) prime-list) 
                (remove-multiples (car candidates) (cdr candidates))
            )
        )
    )
    (erat '() (range 2 n))
)
