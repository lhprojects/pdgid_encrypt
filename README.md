# pdgid_encrypt
Encrypt message using Particle Data Group numbering scheme
使用高能物理的知识，进军计算机学界的一个尝试（←_←）。
把26个英文字母翻译成字母对应粒子的PDG ID来进行加密。
尽量使得加密文人类可读，并且引入转义字符`##`空字符`#.`消除密文到明文内容回转的歧义。
密文对于熟悉PDG ID的人来说是直接可读的，如果您看着密文可以直接读出明文，说明您对PDG ID的记忆已经非常好了，也说明您最近特别闲。

编译方法： 直接`make`。如果不幸出错，那么就试一试`g++ -std=c++11 pdgid_encrypt.cpp -o pdgid_encrypt`。如果不行又出错，那就更新`g++`。如果还不行就算了。

测试方法：`./pdgid_encrypt Happy New #Year2018`应当会得到`#025#20113#2212#2212#22 #0221#11#223 ###022#11#20113#553#.2018`。
然后`./pdgid_encrypt -d "#025#20113#2212#2212#22 #0221#11#223 ###022#11#20113#553#.2018"`，应当会得到`Happy New #Year2018`。

也可测试对文件的转换``
```
./pdgid_encrypt -f source.txt > convert-to.txt
./pdgid_encrypt -d -f convert-to.txt > convert-back.txt
diff source.txt convert-back.txt
```
期望没有任何输出，请打开convert-to.txt观看加密内容。



