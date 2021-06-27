# Build M.A.R.S.

If you want to test the most recent features of M.A.R.S. which haven’t been released yet, if you want to implement your own ideas or if you want to fix bugs of the game, you need to know how to compile M.A.R.S.!

This short tutorial will describe the basic steps needed to successfully build M.A.R.S. on Ubuntu. If you’re using another distro, you will have to adapt some commands, but nevertheless you will learn which libraries M.A.R.S. needs in order to be compiled.

## Preparing

First of all, you will need the basic packages needed for compiling and cmake to generate make files. These packages can be installed via entering the following command in a terminal:

```
sudo apt-get install build-essential cmake
```

When prompted for you password, type it and confirm with return. This will install all relevant packages on your computer.

## Compiling SFML2

SFML2 is the Simple and Fast Multimedia Library M.A.R.S. uses for its basic communications with the current operating system. M.A.R.S. employs the current SVN version of SFML2 because it hasn’t been released yet (in the repositories of Ubuntu is only SFML1.6).

Firstly you should install all dependencies of SFML2:

```
sudo apt-get install libfreetype6-dev libglew1.5-dev libjpeg62-dev libpng12-dev libsndfile1-dev libxrandr-dev libopenal-dev
```

Secondly download (https://github.com/LaurentGomila/SFML/tarball/master) a snapshot of SFML2′s repository and extract it to a directory of your choice. For the following commands I’ll assume that you extracted it to your desktop and renamed the folder from something like “LaurentGomila-SFML-6eac425″ to “sfml2″.

Now, in your terminal, navigate to the extracted folder:

```
cd ~/Desktop/sfml2
```

In order to compile SFML2, you have to create a make file using cmake:

```
cmake .
```

Now compile and install SFML2:

```
make && sudo make install
```

If everything went alright, you should now have a brand new version of SFML2! Congratulations ;)

## Compiling M.A.R.S.

Now on to the interesting part! Well, to be honest, it won’t be much more interesting…in fact it will be nearly exactly the same procedure as above but at least, you will be finished afterwards!

Firstly you should install all additional dependencies of M.A.R.S:

```
sudo apt-get install libfribidi-dev libtag1-dev
```

Secondly download (https://github.com/thelaui/M.A.R.S./archive/refs/heads/master.zip) a snapshot of M.A.R.S’ repository and extract it to a directory of your choice. For the following commands I’ll assume that you extracted it to your desktop.

Now, in your terminal, navigate to the extracted folder:

```
mkdir ~/Desktop/mars-game/build
cd ~/Desktop/mars-game/build/
```

In order to compile M.A.R.S., you have to create a make file using cmake:

```
cmake ..
```

Now compile M.A.R.S.:

```
make && cd ..
```

If everything went alright, you should now have a brand new version of M.A.R.S.! Congratulations :D
You may launch it by typing:

```
./marsshooter
```

in the current directory or by double-clicking the executable “mars” in ~/Desktop/mars-game. Currently “make install” doesn’t work properly so you have to launch M.A.R.S out of this folder.

## Additional thoughts

If you want to update your M.A.R.S. regularly, you may also use git to download the current git version:

```
git clone https://github.com/thelaui/M.A.R.S..git mars-game
```

If you want to work with the code, you can create project files for your favorite IDE using cmake or premake. For example being in ~/Desktop/mars-game/build/

```
cmake -G "CodeBlocks - Unix Makefiles" ..
```

creates project files for Code::Blocks with cmake. Alternatively you may use premake4 (http://industriousone.com/premake) in ~/Desktop/mars-game/

```
premake4 codeblocks
```

in order to create project files. You may download premake4 from its homepage. Drop the executable into a search path of your system (or just in the directory of M.A.R.S.) and execute it with a flag for your desired IDE. Consult the homepage of premake for a full list (http://industriousone.com/what-premake) of supported IDEs.

Have fun with the most recent version of M.A.R.S.! No need to wait for the release!

---
adapted from http://mars-game.sourceforge.net/?page_id=160&vasthtmlaction=viewtopic&t=18.0
