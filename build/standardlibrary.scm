(define true #t)
(define false #f)

(define (boolean->integer a)
  (if a 1 0)
)

(define (range first last)
  (if (= first last) () (cons first (range (+ first 1) last)))
)

; List operations
(define number? integer?)

(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cdar x) (cdr (car x)))
(define (cddr x) (cdr (cdr x)))

(define (caaar x) (car (car (car x))))
(define (caadr x) (car (car (cdr x))))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))
(define (cdaar x) (cdr (car (car x))))
(define (cdadr x) (cdr (car (cdr x))))
(define (cddar x) (cdr (cdr (car x))))
(define (cdddr x) (cdr (cdr (cdr x))))

(define (caaaar x) (car (car (car (car x)))))
(define (caaadr x) (car (car (car (cdr x)))))
(define (caadar x) (car (car (cdr (car x)))))
(define (caaddr x) (car (car (cdr (cdr x)))))
(define (cadaar x) (car (cdr (car (car x)))))
(define (cadadr x) (car (cdr (car (cdr x)))))
(define (caddar x) (car (cdr (cdr (car x)))))
(define (cadddr x) (car (cdr (cdr (cdr x)))))

(define (cdaaar x) (cdr (car (car (car x)))))
(define (cdaadr x) (cdr (car (car (cdr x)))))
(define (cdadar x) (cdr (car (cdr (car x)))))
(define (cdaddr x) (cdr (car (cdr (cdr x)))))
(define (cddaar x) (cdr (cdr (car (car x)))))
(define (cddadr x) (cdr (cdr (car (cdr x)))))
(define (cdddar x) (cdr (cdr (cdr (car x)))))
(define (cddddr x) (cdr (cdr (cdr (cdr x)))))

(define (zero? x) (= 0 x))

(define (<= a b) (or (< a b) (= a b)))

(define (list . x) x)

(define (build-list n proc)
  (define (iter m max-value)
      (if (= m max-value)
        '()
        (cons (proc m) (iter (+ m 1) max-value))
      )
  ) (iter 0 n)
)

(define (list-ref n ns)
  (if (= n 0)
    (car ns)
    (list-ref (- n 1) (cdr ns))
  )
)

(define (reverse ns)
  (define (iter in out)
    (if (null? in)
      out
      (iter (cdr in) (cons (car in) out))
    )
  )
  (iter ns ())
)

(define (take n ns)
  (if (or (null? ns) (= n 0))
    ()
    (cons (car ns) (take (- n 1) (cdr ns)))
  )
)

(define (take-while test ns)
  (if (or (not (test (car ns))) (null? ns))
    ()
    (cons (car ns) (take-while test (cdr ns)))
  )
)

;filter
(define (filter test ns)
  (if (null? ns)
    ()
    (if (test (car ns)) 
      (cons (car ns) (filter test (cdr ns))) 
      (filter test (cdr ns))
    )
  )
)

(define (fold-right accum op ns)
  (if (null? ns)
    accum
    (fold-right (op accum (car ns)) op (cdr ns))
  )
)

(define (fold-left accum op ns)
  (if (null? ns)
      accum
      (op (car ns) (fold-left accum op (cdr ns)))
  )
)

(define (list-min ns)
  (fold-right (car ns) (lambda (m val) (if (> m val) m val)) (cdr ns))
)

(define (list-max ns)
  (fold-right (car ns) (lambda (m val) (if (> m val) m val)) (cdr ns))
)

(define (min . lst) (list-min lst))
(define (max . lst) (list-max lst))

(define (append list1 list2)
  (if (null? list1)
    list2
    (cons (car list1) (append (cdr list1) list2))
  )
)

(define (pow x n)
  (define (iter product n)
    (if (= n 0)
      product
      (iter (* product x) (- n 1))
    )
  ) (iter 1 n)
)

;map
(define (unary-map proc ns)
  (if (null? ns) 
    ()
    (cons (proc (car ns)) (unary-map proc (cdr ns)))
  )
)

(define (map fn primary-list . other-lists)
  (if (null? primary-list) '()
    (cons (apply fn (cons (car primary-list) (unary-map car other-lists)))
          (apply map (cons fn (cons (cdr primary-list)
                           (unary-map cdr other-lists)))))))

(define (zip lst1 lst2)
  (if (or (null? lst1) (null? lst2))
    '()
    (cons (cons (car lst1) (car lst2)) (zip (cdr lst1) (cdr lst2)))
  )
)

(define (map* initial proc ns)
  (if (null? ns) 
    initial
    (cons (proc (car ns)) (map proc (cdr ns)))
  )
)

(define (elem? item items)
  (cond 
    ((null? items) #f)
    ((= (car items) item) #t)
    (else (elem? item (cdr items)))
  )
)

(define (slice ns start amount)
  (if (= start 0)
    (take amount ns)
    (slice (cdr ns) (- start 1) amount)
  )
)

;;;;;; String Procedures ;;;;;;;
(define (string . chars) (list->string chars))

(define (string->list str)
  (define (iter count max-count)
    (if (= count max-count)
      '()
      (cons (string-ref str count) (iter (+ count 1) max-count))
    )
  )
  (iter 0 (string-length str))
)
