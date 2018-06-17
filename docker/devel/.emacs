;;
;=======================================================================
; Misc
;=======================================================================
(global-font-lock-mode t)
(setq line-number-mode t)
(setq column-number-mode t)
(auto-compression-mode t)
;(global-set-key "\C-z" 'undo)
(setq frame-title-format (concat "%b - emacs@" system-name))
(setq make-backup-files nil)
(setq kill-whole-line t)
(when (boundp 'show-trailing-whitespace) (setq-default show-trailing-whitespace t))
(recentf-mode)
(setq transient-mark-mode t)
(show-paren-mode)
;;
;=======================================================================
; Automatically `chmod +x` when saving a script file
;=======================================================================
(defun make-file-executable ()
  "Make the file of this buffer executable, when it is a script source."
  (save-restriction
    (widen)
    (if (string= "#!"
         (buffer-substring-no-properties 1
                         (min 3 (point-max))))
        (let ((name (buffer-file-name)))
          (or (equal ?. (string-to-char
             (file-name-nondirectory name)))
              (let ((mode (file-modes name)))
                (set-file-modes name (logior mode (logand
                           (/ mode 4) 73)))
                (message (concat "Wrote " name " (+x)"))))))))
(add-hook 'after-save-hook 'make-file-executable)
;;
;=======================================================================
; ~/.emacs.d/auto-install/ にパスを通す
;=======================================================================
(add-to-list 'load-path (expand-file-name "~/.emacs.d/auto-install"))
;;
;=======================================================================
; インデントポリシー
;=======================================================================
(setq-default indent-tabs-mode nil)
;; Set up the keyboard so the delete key on both the regular keyboard
;; and the keypad delete the character under the cursor and to the right
;; under X, instead of the default, backspace behavior.
;(global-set-key [backspace] 'delete-backward-char)
;(global-set-key [delete] 'delete-char)
;(global-set-key "\C-h" 'delete-backward-char)
;(global-set-key "\M-?" 'help-for-help)
(global-set-key "\M-n" 'scroll-up)
(global-set-key "\M-p" 'scroll-down)

(global-set-key "\C-xn" (lambda () (interactive) (other-window 1)))
(global-set-key "\C-xp" (lambda () (interactive) (other-window -1)))

(defun split-window-vertically-n (n)
  (interactive "p")
  (if (= n 2)
      (split-window-vertically)
    (progn
      (split-window-vertically
       (- (window-height) (/ (window-height) n)))
      (split-window-vertically-n (- n 1)))))
(global-set-key "\C-x@" '(lambda ()
                           (interactive)
                           (split-window-vertically-n 3)))

(defun split-window-horizontally-n (n)
  (interactive "p")
  (if (= n 2)
      (split-window-horizontally)
    (progn
      (split-window-horizontally
       (- (window-width) (/ (window-width) n)))
      (split-window-horizontally-n (- n 1)))))
(global-set-key "\C-x#" '(lambda ()
                           (interactive)
                           (split-window-horizontally-n 3)))

(setq compile-command "/work/enek/bin/test --build-dir=/work/enek-build/debug -j auto --debug && /work/enek/bin/test --build-dir=/work/enek-build/release -j auto --release")
(setq compilation-scroll-output 'first-error)
(global-set-key [f1] 'compile)
(global-set-key [f2] 'previous-error)
(global-set-key [f3] 'next-error)

(setq truncate-lines nil)
(setq truncate-partial-width-windows nil)

;(defun toggle-truncate-lines ()
;  (interactive)
;  (if truncate-lines
;      (setq truncate-lines nil)
;    (setq truncate-lines t))
;  (recenter))

(global-set-key "\C-cl" 'toggle-truncate-lines)

;; always end a file with a newline
;(setq require-final-newline t)

(set-background-color "black")
(set-foreground-color "white")
(set-cursor-color "white")

(add-to-list 'load-path "~/.emacs.d/lisp/")

(require 'cc-mode)

(setq c-default-style "bsd")

(add-hook 'c-mode-common-hook
     	  '(lambda ()
             (setq c-basic-offset 2)
             (c-toggle-hungry-state t)
             (setq indent-tabs-mode nil)
             (setq show-trailing-whitespace t)))

(add-hook 'c++-mode-hook
          '(lambda ()
             (c-set-offset 'innamespace 0)))

(setq auto-mode-alist
      (append
       '(("\\.hpp$" . c++-mode)
         ("\\.cpp$" . c++-mode)
         ("\\.ipp$" . c++-mode)
         ) auto-mode-alist))


(add-hook 'sh-mode-hook
          '(lambda ()
             (progn
               (setq sh-basic-offset 2
                     sh-indentation 2
                     sh-indent-for-case-label 0
                     sh-indent-for-case-alt '+))))

(require 'cmake-mode)
(setq auto-mode-alist
      (append
       '(("CMakeList\\.txt\\'", cmake-mode)
         ("\\.cmake\\'", cmake-mode))
       auto-mode-alist))
