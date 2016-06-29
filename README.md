# graph
This is a simple graph library that is written in modern C++. **graph** uses [Column Sparse Row (CSR)](https://en.wikipedia.org/wiki/Sparse_matrix#Compressed_sparse_row_.28CSR.2C_CRS_or_Yale_format.29) format to store the graph information and it provides the following features:

* Support fast serialization/deserialization using cereal and leveldb.

* Display the graph information in JSON or XML formats.

* Implementation for some basic recursive and non-recursimve graph algorithms traversal algorithms such as DFS, BFS etc.
