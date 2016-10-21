# graph

This is a simple and high performance graph library written in modern C++ i.e C++11 and C++14. **graph** can be suitable for high performance applications that do not need to modify the graph and require very fast serialization/deserialization.

## Features ##

* Use [Column Sparse Row (CSR)](https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_.28CSR.2C_CRS_or_Yale_format.29) format to store the graph information.

* Support very fast serialization/deserialization using [cereal](http://uscilab.github.io/cereal/).

* Provide several basic recursive and non-recursive graph traversal algorithms such as DFS and BFS.

* Output the graph information in DOT, JSON, and XML formats.

