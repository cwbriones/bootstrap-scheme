(define true #t)
(define false #f)

(define (even? n) (= (modulo n 2) 0))
(define (odd? n) (not (even? n)))
(define (zero? x) (= 0 x))
(define (negative? x) (> 0 x))

(define (boolean->integer a) (if a 1 0))

(define (range first last)
  (if (= first last) () (cons first (range (+ first 1) last))))

(define (vector-range first last) 
  (make-initialized-vector (- last first) (lambda (i) (+ first i))))

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

(define (<= a b) (not (> a b)))
(define (>= a b) (not (< a b)))

(define (list . x) x)

(define (build-list n proc)
  (define (iter m max-value)
      (if (= m max-value) '()
        (cons (proc m) (iter (+ m 1) max-value))))
  (iter 0 n))

(define (list-ref n ns) (if (= n 0) (car ns) (list-ref (- n 1) (cdr ns))))

(define (reverse ns)
  (define (iter in out)
    (if (null? in)
      out
      (iter (cdr in) (cons (car in) out))))
  (iter ns ()))

(define (take n ns)
  (if (or (null? ns) (= n 0))
    ()
    (cons (car ns) (take (- n 1) (cdr ns)))))

(define (take-while test ns)
  (if (or (not (test (car ns))) (null? ns))
    ()
    (cons (car ns) (take-while test (cdr ns)))))

;filter
(define (filter test ns)
  (if (null? ns)
    ()
    (if (test (car ns)) 
      (cons (car ns) (filter test (cdr ns))) 
      (filter test (cdr ns)))))

(define (fold-right accum op ns)
  (if (null? ns)
    accum
    (fold-right (op accum (car ns)) op (cdr ns))))

(define (fold-left accum op ns)
  (if (null? ns)
      accum
      (op (car ns) (fold-left accum op (cdr ns)))))

(define (list-min ns)
  (fold-right (car ns) (lambda (m val) (if (> m val) m val)) (cdr ns))
)

(define (list-max ns)
  (fold-right (car ns) (lambda (m val) (if (> m val) m val)) (cdr ns))
)

(define (min . lst) (list-min lst))
(define (max . lst) (list-max lst))

(define (append2 list1 list2)
  (if (null? list1)
    list2
    (cons (car list1) (append2 (cdr list1) list2))))

(define (append . lsts)
  (if (null? (cdr lsts))
    (car lsts)
    (append2 (car lsts) (apply append (cdr lsts)))))

(define (square n) (* n n))

(define (expt n m)
  (cond 
    ((= m 0) 1)
    ((even? m) (square (expt n (/ m 2))))
    (else (* n (expt n (- m 1))))))

;map
(define (unary-map proc ns)
  (if (null? ns) 
    ()
    (cons (proc (car ns)) (unary-map proc (cdr ns)))))

(define (map fn primary-list . other-lists)
  (if (null? primary-list) '()
    (cons (apply fn (cons (car primary-list) (unary-map car other-lists)))
          (apply map (cons fn (cons (cdr primary-list) (unary-map cdr other-lists))))
    )
  )
)

(define (append-map func . items) (apply append (apply map (cons func items))))

(define (zip . lsts) (apply map list lsts))

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

(define (char<? . chars) (apply < (unary-map char->integer chars)))
(define (char>? . chars) (apply > (unary-map char->integer chars)))
(define (char=? . chars) (apply = (unary-map char->integer chars)))

(define (string-append . strs)
  (list->string (apply append (map string->list strs)))
)

;; Vector operations
(define (list->vector lst)
  (define (iter vec count max-count lst)
    (if (= count max-count)
      vec
      (begin (vector-set! vec count (car lst)) 
             (iter vec (+ count 1) max-count (cdr lst)))
    )
  )
  (iter (make-vector (length lst)) 0 (length lst) lst)
)

(define (make-initialized-vector k proc)
  (define (iter vec count max-count)
    (if (= count max-count)
      vec
      (begin (vector-set! vec count (proc count))
             (iter vec (+ count 1) max-count))))
  (iter (make-vector k) 0 k)
)

(define (vector-grow vec k)
  (define (iter vec new-vec count max-count)
    (if (= count max-count)
      new-vec
      (begin (vector-set! new-vec count (vector-ref vec count))
             (iter vec new-vec (+ count 1) max-count))))
  (iter vec (make-vector k) 0 (vector-length vec)))

(define (vector-copy vec) (vector-grow vec (vector-length vec)))

(define (vector-map proc vec)
  (define (iter new-vec count max-count)
    (if (= count max-count)
      new-vec
      (begin (vector-set! new-vec count (proc (vector-ref vec count)))
             (iter new-vec (+ count 1) max-count))))
  (iter (vector-copy vec) 0 (vector-length vec)))

(define (subvector vec start end)
  (define (iter vec new-vec count max-count)
    (if (= count max-count)
      new-vec
      (begin (vector-set! new-vec count (vector-ref vec count))
             (iter vec new-vec (+ count 1) max-count))))
  (iter vec (make-vector (- end start)) start end))

(define (vector-head vec end) (subvector vec 0 end))
(define (vector-tail vec start) (subvector vec start (vector-length vec)))

(define (make-random-list n k)
  (if (= k 0)
    '()
    (cons (random n) (make-random-list n (- k 1)))))

(define (partition-list test lst)
  (define (iter first second item lst)
    (if (null? lst)
      (list first second)
      (if (test (car lst) item)
        (iter (cons (car lst) first) second item (cdr lst))
        (iter first (cons (car lst) second) item (cdr lst)))))
  (iter '() '() (car lst) (cdr lst)))

(define (qsort test lst)
  (if (null? lst)
    '()
    (let ((partitions (partition-list test lst)))
      (append (qsort test (car partitions)) (cons (car lst) (qsort test (cadr partitions))))
    )))

(define (merge test lst1 lst2)
  (cond ((null? lst1) lst2)
        ((null? lst2) lst1)
        (else
            (if (test (car lst1) (car lst2))
              (cons (car lst1) (merge test (cdr lst1) lst2))
              (cons (car lst2) (merge test lst1 (cdr lst2)))))))

(define (split lst at)
  (define (iter n l i)
    (if (or (null? l) (= i at))
      (cons (reverse n) l)
      (iter (cons (car l) n) (cdr l) (+ i 1))))
  (iter () lst 0))

(define (split-in-half lst) (split lst (quotient (length lst) 2)))

(define (merge-sort test lst)
  (if (< (length lst) 2)
    lst
    (let ((halves (split-in-half lst)))
      (merge test (merge-sort test (car halves)) (merge-sort test (cdr halves))))))

(define (sort test lst) (merge-sort test lst))

(define (read-line input-port)
  (list->string
      (let loop ((char (read-char input-port)))
        (if (or (eof-object? char) (char=? #\newline char))
          '()
          (cons char (loop (read-char input-port)))
        )
      )))

(define (read-lines input-port)
    (let loop ((next-char (peek-char input-port)))
      (if (eof-object? next-char)
        '()
        (cons (read-line input-port) (loop (peek-char input-port))))))

(define (read-string input-port)
  (list->string 
  (let loop ((char (read-char input-port)))
    (if (eof-object? char)
      '()
      (cons char (loop (read-char input-port)))
    ))))
