# NN-LSH
-----------------------------------------------------------------
Find Nearest Neighbors using Locality-Sensitive Hashing technique
-----------------------------------------------------------------

Implementation of Locality-Sensitive Hashing in order to find in a dataset P that belongs to a metric space :
- neighbors in range R of object q
- nearest neighbor of object q

  Consider those objects as points. The program supports objects of real coordinate space of d dimensions(using euclidean and     cosine metric), objects of Hamming space and objects where the distance is calculated based on a distance-matrix.


---------------------
DATASETS(File format)
---------------------
- Metric_space vector

  The format of this dataset is :

  @metric_space vector

  @metric {euclidean, manhattan, cosine}

  item_id1  X11 X12 ... X1d

  .

  .

  item_idN XN1  XN2 ... XNd

  where Xij the coordinates(double) of a vector that represents object i of Euclidean space of d dimensions


- Metric_space hamming

  @metric_space hamming

  item_id1    B1
  
  .
  
  .

  item_idN    BN

  where Bi the binary string of length <= 64 bits that represents object i of Hamming space

- Metric_space matrix

  @metric_space matrix

  @items [item_id1, item_id2, ..., item_idN]

  0 d12 d13 . . . d1N

  0  0  d23 . . . d2N

  .
  
  .

  0 0 0 0 0 0 0 0 0 0

  where dij the distance of object i from object j(dij=dji && dii=0)


----------------------
QUERYSETS(File format)
----------------------

- Euclidean Space

  Radius: (double)

  item_idS1 X11 X12 . . . X1d

  .
  
  .
  
  item_idSQ XQ1 XQ2 . . . XQd

- Hamming Space

  Radius: (int)

  item_idS1 B1

  .       .       

  item_idSQ BQ
  
- Metric_Space Matrix

  Radius: (int/double)
  item_idS1 d(S1)1  d(S1)2  . . . d(S1)N
  
  .
  
  .
  
  item_idSQ d(SQ)1  d(SQ)2  . . . d(SQ)N
  
  
  -----------------------------------
  Compiling and Executing the program
  -----------------------------------
  
  A makefile exists so use "make" in order to compile the program. The name of the dataset/queryfile/outputFile, number of hashtables(L)and  number of hash functions used(k) must be given as command line arguments so the command to execute the program is :
  
- ./lsh -d (input file) -q (query file) -k (int) -L (int) -o (outputfile)

  *Flags -k and -L are optional(default values L=5 , k=4)
