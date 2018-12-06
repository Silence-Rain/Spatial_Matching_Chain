## Setting of variable factors

|   Factor    |               Setting               |
| :---------: | :---------------------------------: |
|    \|O\|    |       1k, 2k, **3k**, 4k, 5k        |
|    \|P\|    |  1.5\*\|O\|, **2\*\|O\|**, 3\*\|O\|  |
|    Exponential param of O    |       500, **1000**, 1500       |
| Scalability | \|O\| = 10k, 100k; \|P\| = 10\*\|O\| |

#### Weight:

对每个点 P，查询其半径为 r 的邻域，获得邻域中数据点个数 k。（ r 设置为整个区域的 5% ）

则 P 的权重满足均值为 10 * k（对 O 则为 5 * k），方差为 1 的 Gaussian 分布。