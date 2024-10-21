
all:

f:
	@$(MAKE) -C ex00 fclean --no-print-directory
	@$(MAKE) -C ex01 fclean --no-print-directory
	@$(MAKE) -C ex02 fclean --no-print-directory
	@$(MAKE) -C ex03 fclean --no-print-directory
	# rm  ex02/*.txt ex03/*.txt
	
0:
	$(MAKE) -C ex00 --no-print-directory
0r:
	$(MAKE) -C ex00 r --no-print-directory
0re:
	$(MAKE) -C ex00 re --no-print-directory
0f:
	$(MAKE) -C ex00 fclean --no-print-directory

1:
	$(MAKE) -C ex01 --no-print-directory
1r:
	$(MAKE) -C ex01 r --no-print-directory
1re:
	$(MAKE) -C ex01 re --no-print-directory
1f:
	$(MAKE) -C ex01 fclean --no-print-directory
2v:
	$(MAKE) -C ex02 --no-print-directory vgrind
2:
	$(MAKE) -C ex02 --no-print-directory
2r:
	$(MAKE) -C ex02 r --no-print-directory
2re:
	$(MAKE) -C ex02 re --no-print-directory
2f:
	$(MAKE) -C ex02 fclean --no-print-directory
3:
	$(MAKE) -C ex03 --no-print-directory
3v:
	$(MAKE) -C ex03 vgrind --no-print-directory
3r:
	$(MAKE) -C ex03 r --no-print-directory
3re:
	$(MAKE) -C ex03 re --no-print-directory
3f:
	$(MAKE) -C ex03 fclean --no-print-directory