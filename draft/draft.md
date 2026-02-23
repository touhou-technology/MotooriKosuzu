- [使用C++反射](#orgb7f958f)
- [翻译功能](#org9e3515f)
- [LISP](#org68ba9c5)
- [TUI](#org2c3983e)
- [读取配置](#org4984583)
- [](#orgd28fae1)

我认为使用母语能更准确的表述想法 况且这只是草图() 一般使用AI来进行翻译应该语意准确差不多 如果有歧义或者不清楚的地方可以发发邮件到<awalwashig@gmail.com>(但是由于有GFW存在, 我不能很好的和emacs集成在一起, 所以我检查邮件可能要很久才检查一次)

不过这个项目依旧是想练习我之前学习的内容(~~just for fun~~) 所以可能有些瑕疵 不过我也会从我使用的角度来考虑如何实现功能(或者说从用户的角度来出发)

以下是计划实现的功能


<a id="orgb7f958f"></a>

# 使用C++反射

-   为什么要使用反射的原因是 Remember, one of the most important principles in programming is the same as one of the most important principles in sorcery, all right? That's if you have the name of the spirit, you get control over it. - Hal Abelson ~~知らないわ~~~ そんな魔法！~~ 对于个人来说只是想玩玩反射^^ 也是实验一些想法 尽管目前GCC反射是实验以及不支持LSP, ~~但我总感觉LSP在妨碍我如果不是我英语不好的话~~.


<a id="org9e3515f"></a>

# 翻译功能

-   调用多个翻译API并可以切换 或许某个语种翻译的更好
-   对个人来说可编程 使用LISP, 在后面提到
-   消息映射 我认为必须要, 但可能要满足一些约束 必须对应才行, 对于约束来说, 添加可以debug的功能<LISP>
-   实验性(~~just for fun~~)分布式计算实现ASR->翻译 虽然说可能ASR的准确性不是很高, 但我因为如果想要交流的话, 这不是问题, 只要三段验证一次就可以了(对于双方) discord的bot可以监听在VC中成员的对话, 所以可以使用[Discord Activities](https://docs.discord.com/developers/activities/overview) + [whisper.cpp<sub>wasm</sub>](https://ggml.ai/whisper.cpp/stream.wasm/) 或者说也可以采用积分策略(不过关于这一点等到实现了再提)


<a id="org68ba9c5"></a>

# LISP

-   LISP? Code as Data! 我有点不太习惯对于配置文件来控制代码行为(如果使用配置文件基本只有我知道要如何编写, 后期维护麻烦 ~~主要还是我比较懒~~ ) 我想做到看到这样写就知道怎么做(也就是反射?) 依旧是实验项目
-   Command 对discord注册功能<或者说可以像是dishell> 以及可以提供[智能补全](https://dpp.dev/application-command-autocomplete.html) Command可以用户自行编写
-   Hook 提供触发event后回调函数


<a id="org2c3983e"></a>

# TUI

-   为什么要做TUI 得益于C++ 反射, TUI可以探查到类的实现和成员(或者说能让代码看到代码了<自涉性>) 因此将配置抽象成可以一眼就理解的(虽然我有点想嵌入Plan9的操作, 但是最为UI来说还是对于大部分人都习惯来设计)
-   用户使用 使用[Discord Activities](https://docs.discord.com/developers/activities/overview)来构建TUI或者直接访问静态网站即可食用wasm


<a id="org4984583"></a>

# 读取配置

-   ~~初始化~~ 对于初始化还是需要先读取配置才能使用, 但alpine是运行时不可写的文件体系, 可能就得写死行为后不可变更(或者重启后丢失) 不能读取后保存, 但其实有一个事情被忽略了, 不是还有一个几乎无限的存储配置地方吗 那就是把配置发在discord上, 在第一个启动的配置写下这个消息的链接, 如果想要更改可以自己编辑消息更改(自己发送的消息), 或者bot发送自己的消息配置然后更改
-   顺序
    -   boot 1 **etc/MotooriKosuzu/config** 2 ~/.config/ 3 ./config/
        -   Init.el Discord Message URL

之后lisp可以随意配置<一般情况, 用户不能使用绝大多数功能, 也可以使用自涉性来删除敏感的功能>


<a id="orgd28fae1"></a>

# TODO 

可能未完善, 我认为
