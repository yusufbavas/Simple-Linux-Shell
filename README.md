# Simple-Linux-Shell
System Programing Lecture

The objective is to develop your own shell named gtushell. Shell support the following commands:

	• lsf: which will list file type (R for regular file, S for non-regular(special) file), access rights (int the
		form of rwxr-xr-x, just like actual ls), file size(bytes) and file name of all files (not directories) in
		the present working directory. It doesn’t take any argument. Does not list any directory.
	• pwd: which will print the path of present working directory.
	• cd: which will change the present working directory to the location provided as argument.
	• help: which will print the list of supported commands.
	• cat: which will print on standard output the contents of the file provided to it as argument or from
		standard input.
	• wc: which will print on standard output the number of lines in the file provided to it as argument
		or the string coming from standard input until EOF character.
	• bunedu: https://github.com/yusufbavas/Linux-du-command
	• exit: which will exit the Shell

pwd, lsf, cat, wc and du commands will be separate executables (each has its own main function),
we will call them utilities. They will be called with fork+exec method from your shell.

Shell have the following features:

	• When the user types !n , the n-th previous command will run. For example, lets assume the
	commands ran so far are [pwd, cd arg, lsf, pwd], when the user types !2, lsf will run again (cd arg
	for !3 etc.). The command run with the same arguments.
	• The vertical bar character ‘|’ must enable constructing a pipe from [wc, bunedu, lsf, cat or pwd]
	to [wc, cat or bunedu]. This will redirect output of the first command to input of second command
	(example, cat file.txt | bunedu -z, this will call bunedu -z on the path written inside file.txt).
	• support redirecting standard input and output of commands all utilities to files through the
	‘<’ and ‘>’ characters.
	• in case of SIGTERM, shell exits by printing a message on the screen.