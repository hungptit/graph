# graph

This is a small and fast graph library written using C++11/14. **graph** is suitable for high performance applications that do not need to modify the graph and require very fast serialization/deserialization i.e [sbutils](https://github.com/hungptit/sbutils).

## Features ##

* Use [Column Sparse Row (CSR)](https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_.28CSR.2C_CRS_or_Yale_format.29) format to store the graph information.

* Support very fast serialization/deserialization using [cereal](http://uscilab.github.io/cereal/).

* Provide several basic recursive and non-recursive graph traversal algorithms such as DFS and BFS.

* Output the graph information in Binary, DOT, JSON, and XML formats.
