;; This buffer is for notes you don't want to save, and for Lisp evaluation.
;; If you want to create a file, visit that file with C-x C-f,
;; then enter the text in that file's own buffer.

(defun hello (name) (insert "Hello " name))
(hello "sameer")

(progn
  (switch-to-buffer-other-window "*scratch*")
  (erase-buffer)
  (hello "you"))

(defconst fl 23)
(insert fl)
%s
