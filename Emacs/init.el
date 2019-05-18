;; init.el --- Emacs configuration
;; INSTALL PACKAGES
;; --------------------------------------
;; Required Packages:
;; auctex, better-defaults, cmake-ide, company (+ additional), elpy, flycheck, google-c-style,
;; irony, ivy, material-theme, py-autopep8, smartparens, yasnippet, yasnippet-snippets

(add-to-list 'load-path "~/.emacs.d/elisp")
(require 'package)

(add-to-list 'package-archives
	     '("melpa" . "http://melpa.org/packages/") t)

(package-initialize)
(when (not package-archive-contents)
  (package-refresh-contents))

(defvar myPackages
  '(better-defaults
    cmake-ide
    company
    company-irony
    company-irony-c-headers
    elpy
    flycheck
    google-c-style
    irony
    ivy
    material-theme
    py-autopep8
    smartparens
    yasnippet
    yasnippet-snippets))

(mapc #'(lambda (package)
	  (unless (package-installed-p package)
	    (package-install package)))
      myPackages)

(setq inhibit-default-init t)
(require 'xref)

;; BASIC CUSTOMIZATION
;; --------------------------------------
;; disable backup
(setq backup-inhibited t)
;; disable auto save
(setq auto-save-default nil)

;; Theme
(load-theme 'material t) ;; load material theme

(defalias 'yes-or-no-p 'y-or-n-p)
(setq inhibit-startup-message t) ;; hide the startup message

;; global-set-keys
(load "~/.emacs.d/globalkeys.el")

(global-linum-mode t) ;; enable line numbers globally
(setq column-number-mode t);; enable column numbers
(tool-bar-mode -1) ;;disable toolbar


;; flyspell
;; --------------------------
;; switch between german and english
(defun fd-switch-dictionary()
      (interactive)
      (let* ((dic ispell-current-dictionary)
    	 (change (if (string= dic "deutsch8") "english" "deutsch8")))
        (ispell-change-dictionary change)
        (message "Dictionary switched from %s to %s" dic change)
        ))


;; for smart parenthesis 
(require 'smartparens-config)
(show-smartparens-global-mode +1)
(smartparens-global-mode 1)

;;ivy mode(minibuffer)
(ivy-mode 1)

;; yasnippet
;; ------------------------------------
(require 'yasnippet)
(yas-reload-all)
(add-hook 'c++-mode-hook #'yas-minor-mode)
(yas-global-mode 1)


;; COMPANY
;; ----------------------------
(require 'company)
(add-hook 'after-init-hook 'global-company-mode)
(setq company-dabbrev-downcase 0)
(setq company-idle-delay 0)
(setq company-require-match nil)

;; multiple-cursors
;; --------------------------------------
;;(require 'multiple-cursors)

;; PYTHON-ELPY
;; ---------------------------------------
(add-hook 'python-mode-hook
          (lambda ()
            (define-key python-mode-map (kbd "C-;") #'comment-or-uncomment-region)))

(add-hook 'python-mode-hook (lambda() (flyspell-prog-mode))) ;; flyspell for mi baad inglisch
(require 'elpy)
(elpy-enable)
(setq elpy-rpc-python-command "python3") ;;use python3
(setq python-shell-interpreter "python3");;use python3
(setq elpy-shell-echo-input nil)
(setq python-shell-completion-native-enable nil)
(when (require 'flycheck nil t)
  (setq elpy-modules(delq 'elpy-module-flymake elpy-modules))
  (add-hook 'elpy-mode-hook 'flycheck-mode))
(require 'py-autopep8)
(add-hook 'elpy-mode-hook 'py-autopep8-enable-on-save)


;; Python ROOT
(setenv "ROOTSYS" "/usr/local/root")
(setenv "PYTHONDIR" "/usr/local/root:$PYTONDIR")
(setenv "PYTHONPATH" "/usr/local/root/lib::/usr/local/root/bindings/pyroot:$PYTHONPATH")
;; (setenv "PATH" "/usr/local/root/bin")
(setenv "LD_LIBRARY_PATH"
	"/usr/local/root/lib:/usr/local/root/lib:/usr/local/root/bindings/pyroot:$LD_LIBRARY_PATH")


;; IPython and Jupyter
;; -------------------------
;;(require 'ein-connect)

;;;;(setq ein:completion-backend 'ein:use-ac-jedi-backend)
;;(setq ein:use-auto-complete-superpack t)

;; C++
;; --------------------------
(add-hook 'c-mode-common-hook (lambda() (flyspell-prog-mode))) ;; flyspell for mi baad inglisch
(add-to-list 'auto-mode-alist '("\\.h\\'" . c++-mode))
(add-hook 'c-mode-common-hook 'google-set-c-style)
(add-hook 'c-mode-common-hook 'google-make-newline-indent)


(setq c-default-style "linux"
      c-basic-offset 2
	  tab-width 2
	  indent-tabs-mode t)

;;(modern-c++-font-lock-global-mode t)
;; flycheck and google's cpplint checkstyle


(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(flycheck-c/c++googlelint-executable "/usr/local/bin/cpplint.py")
 '(flycheck-googlelint-linelength "80")
 '(flycheck-googlelint-root ".")
 '(flycheck-googlelint-verbose "0")
 '(package-selected-packages
   (quote
    (flymake-python-pyflakes company-reftex auctex company auto-complete auto-yasnippet ivy ein yasnippet irony 0blayout 2048-game flycheck-irony multiple-cursors yasnippet-snippets smartparens rtags py-autopep8 material-theme jedi google-c-style flycheck elpy doom-themes company-math company-irony-c-headers company-irony company-auctex cmake-mode cmake-ide better-defaults))))
(require 'flycheck)
(eval-after-load 'flycheck
  '(progn
     (require 'flycheck-google-cpplint)
     (flycheck-add-next-checker 'c/c++-clang
				'(warning . c/c++-googlelint))))

(add-hook 'c++-mode-hook
	  (lambda () (setq flycheck-clang-language-standard "c++11")))
(add-hook 'c++-mode-hook 'flycheck-mode)
(add-hook 'c-mode-hook 'flycheck-mode)
;; rtags for references and shit
(add-hook 'c-mode-hook 'rtags-start-process-unless-running)
(add-hook 'c++-mode-hook 'rtags-start-process-unless-running)

(defun my-goto-symbol ()
  (interactive)
  (deactivate-mark)
  (ring-insert find-tag-marker-ring (point-marker))
  (or (and (require 'rtags nil t)
	   (rtags-find-symbol-at-point))))

;;(rtags-enable-standard-keybindings)

(define-key global-map (kbd "C-c f") 'rtags-find-file)

;;(define-key c-mode-base-map (kbd "M-.") 'my-goto-symbol)
;;(define-key c-mode-base-map (kbd "M-,") 'pop-tag-mark)


;; cmake-ide
(require 'cmake-ide)
(cmake-ide-setup)
(setq cmake-ide-flags-c++ (append '("std=c++11")))



;; irony for completion
(require 'irony)
(require 'company-irony-c-headers)
(add-hook 'c-mode-hook 'irony-mode)
(add-hook 'c++-mode-hook 'irony-mode)

(defun my-irony-mode-hook ()
  (define-key irony-mode-map
    [remap completion-at-point] 'irony-completion-at-point)
  (define-key irony-mode-map
    [remap complete-symbol] 'irony-completion-at-point))
(add-hook 'irony-mode-hook 'my-irony-mode-hook)
(add-hook 'irony-mode-hook 'irony-cdb-autosetup-compile-options)


(add-hook 'irony-mode-hook 'company-irony-setup-begin-commands)
(eval-after-load 'company '(add-to-list 'company-backends '(company-irony-c-headers
							    company-irony ;; company-yasnippet
							    company-clang)))

;; Add yasnippet support for all company backends
;; https://github.com/syl20bnr/spacemacs/pull/179
(defvar company-mode/enable-yas t
  "Enable yasnippet for all backends.")

(defun company-mode/backend-with-yas (backend)
  (if (or (not company-mode/enable-yas) (and (listp backend) (member 'company-yasnippet backend)))
      backend
    (append (if (consp backend) backend (list backend))
            '(:with company-yasnippet))))

(setq company-backends (mapcar #'company-mode/backend-with-yas company-backends))

;; Java
;;----------------------------------------
(add-to-list 'load-path "/usr/share/emacs/site-lisp/cedet-common/")
(add-to-list 'load-path "/usr/share/emacs/site-lisp/cedet-contrib/")
;;for jde
(require 'cedet)
(add-hook 'java-mode-hook
          (lambda ()
           "Treat Java 1.5 @-style annotations as comments."
            (setq c-comment-start-regexp "(@|/(/|[*][*]?))")
            (modify-syntax-entry ?@ "< b" java-mode-syntax-table)))
(add-hook 'java-mode-hook (lambda ()
                            (setq c-basic-offset 2
                                  tab-width 2
                                  indent-tabs-mode t)))

(add-to-list 'load-path "/usr/share/emacs/site-lisp/elib/")


;; ;; Latex
;; ;; ---------------------------
(add-hook 'LaTeX-mode-hook 'flyspell-mode)
(add-hook 'LaTeX-mode-hook 'flyspell-buffer)
(add-hook 'LateX-mode-hook 'reftex-mode)
(load "auctex.el" nil t t)

(setq TeX-auto-save t)
(setq TeX-parse-self t)
(setq TeX-save-query nil)

(setq TeX-PDF-mode t)

;; Make okular work
(setq TeX-source-correlate-mode t
      TeX-source-correlate-start-server t)
(eval-after-load "tex"
  '(setcar (cdr (assoc 'output-pdf TeX-view-program-selection)) "Okular"))


(company-auctex-init)
(setq company-math-disallow-unicode-symbols-in-face nil)
(append '((company-math-symbols-latex company-math-symbols-unicode
              company-auctex-macros company-auctex-environments))
                      company-backends)

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )
