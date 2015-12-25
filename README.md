Description
=============

This program computes the minimal AABB of a wavefront object file. 
This AABB could be used for implementing view-frustum culling in a game.

Building
==============

The program is built using cmake as follows:

```
mkdir build && cd build && cmake .. && make
```

This should produce an executable named aabb_create

Usage
==============

The program is used as follows:

```bash
./aabb_create /monkey.obj
```

where `monkey.obj` is some wavefront object file. The above command creates a file named `monkey.aabb`. The file contains the following:

```
max 1.36719 0.984375 0.851562
min -1.36719 -0.984375 -0.851562
```

As can be observed, the file specifies the minimum and maximum points of the AABB of the model saved in the file `monkey.obj`
