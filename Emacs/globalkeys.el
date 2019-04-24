;; global-set-keys

(global-set-key (kbd "C-;") 'comment-or-uncomment-region) ;; comment-or-uncomment-region

(global-set-key (kbd "C-x C-b") 'ibuffer)

;; Python-Elpy
(global-set-key (kbd "M-n") 'elpy-nav-forward-block)   ;;move bock down
(global-set-key (kbd "M-p") 'elpy-nav-backward-block)  ;;move bock up
(global-set-key (kbd "M-,") 'pop-tag-mark) ;; go to last place where M-. was used (go-to-definition)

;; multiple-cursors
;;(global-set-key (kbd "C->") 'mc/mark-next-like-this)
;;(global-set-key (kbd "C-<") 'mc/mark-previous-like-this)

;; IPython and Jupyter
;;(global-set-key (kbd "C-c C-n l") 'ein:notebooklist-login)
;;(global-set-key (kbd "C-c C-n o") 'ein:notebooklist-open)

;; cmake-ide
(global-set-key (kbd "C-c m") 'cmake-ide-compile)
