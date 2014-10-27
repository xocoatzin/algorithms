"Data Structures and Algorithms"
==========

##University of Bristol
v 1.0 (c) 2014

This work package is a learning platform for students at the University of Bristol, Department of Computer Science, as part of studying for course COMS21103 "Data Structures and Algorithms".

The aim of this learning platform is to enable students to implement the algorithms discussed in the lectures without spending lots of time developing input/output, data structure and validation methods. The platform also enables the students to try these algorithms on different inputs, following the code progress through algorithmic iterations.

The code stubs cover the material taught within the topics:

1. Stable Matching
2. String Matching
3. Max-Flow
4. Linear Programming (the Simplex method)

The code has been written by Alan Torres, University of Bristol;
Material and teaching is delivered by Dr. Dima Damen, University of Bristol.


##How to...

###Clone the repository

1. From the command line `git clone https://github.com/xocoatzin/algorithms.git` -or-
2. If you have "Github for Windows" just click the button at the right -or-
3. Download the code as ZIP with the button at the right.
 
###Create a branch to work with

From the command line `git checkout -b [name_of_your_new_branch]`
 
###Get updates

From the command line: `git pull`. Remember to commit your changes before pulling an update!


###Compile the code

####Visual Studio (2013):

1. Open the project in 'build/VisualStudio/Algorithms/Algorithms.sln'
2. Press [F5] to run.
 
You can get a free copy of VS2013 on http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs#DownloadFamilies_2

####Linux:

1. From the command line (you'll need a c++11 compatible compiler, GCC 4.7 recommended):

```bash
cd build/makefiles/stable-matching/
 make
./stable-matching ../../../input/stable-matching/input.txt
```


##About working with Git

You can access the documentation here: http://git-scm.com/book/en/Getting-Started-Git-Basics
