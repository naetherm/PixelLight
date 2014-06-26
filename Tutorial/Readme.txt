
PixelLight Engine Tutorial
==========================

This folder and everything below consist what is considered the interactive part of PixelLight's
documentation. Each one of these projects explains in detail one important concept or feature of
the engine.


I. The structure
----------------

The first "zeroeth" chapter is optional and deals with low-level concepts - it is very
helpful to go through it first but you can as well skip it if you want to go to the cool visual
stuff first. Just make sure to return threre if you hit a concept you don't fully understand.

(NOTE: the following is not yet done)

The following chapters deal with higher level tasks with increasing complexity. The range of
topics covered encompasses hopefully every aspect of modern game development. The climax of
the tutorial is then its final chapter, where a basic game is put together, showing how to
combine the knowledge gained in the previous chapters.


II. Using the tutorial
----------------------

This interactive documentation is intended to be read chapter by chapter, excercise by excersise,
like a book is. A diagram of the important files you'll find below is as follows:

/00.Basics
	|--- 00.Essentials.txt				<--	Short summary of the chapter, read first
	|--- /00.01.HelloWorld	
			|--- /Private
					|--- Main.cpp		<--	The main source file for the excercise - you should
											open this file first and read through it as it contains
											the introduction to the topic being covered and a list
											of important files to go through

This structure is repeated for every excercise. To summarize, you should proceed through the tutorial
in the following way:

- Before starting a chapter, read its introduction file
- For each excercise, go to its Private/Main.cpp file first and then proceed as asked there