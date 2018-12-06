
## Spatial Matching Problem - Chain Algorithm

A efficient algorithm for Spatial Matching Problem with linear number of NN-Query

#### About

This is the implementation of a VLDB paper: [On Efficient Spatial Matching](http://www.cse.cuhk.edu.hk/~taoyf/paper/vldb07.pdf)

#### Highlight

- Using KD-Tree as the NN-Query method to improve efficiency

#### Setting of variable factors

|         Factor         |               Setting                |
| :--------------------: | :----------------------------------: |
|         \|O\|          |        1k, 2k, **3k**, 4k, 5k        |
|         \|P\|          |  1.5\*\|O\|, **2\*\|O\|**, 3\*\|O\|  |
| Exponential param of O |         500, **1000**, 1500          |
|      Scalability       | \|O\| = 10k, 100k; \|P\| = 10\*\|O\| |

#### Weight

For each data point **A**, performs a range query with radius **r** from **A** and obtained the number of data points in this range query, say **k**. (**r** is set to 5% of the range of the dimension)

**A.weight** is generated according to a Normal distribution where mean is **10 * k** (for **A** in **P**) or **5 \* k** (for **A** in **O**) and standard deviation is **1**.

