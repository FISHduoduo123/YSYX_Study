# 在 Shell 中导航
当你打开终端时，会看到一个通常长这样子的提示符：
- ```missing:~$```   
这是 Shell 的主要交互界面。它告诉你：你当前在名为 missing 的机器上，「当前工作目录」（即你所在的位置）是 ~，它是 home 目录的简写，在 Linux 上通常对应 /home/用户名（例如 /home/jon）。
$ 表示你当前不是 root 用户（后面会详细讲）。在提示符后输入命令，Shell 会解释并执行它。最基本的命令就是运行一个程序：

missing:~$ date
Fri 10 Jan 2020 11:49:31 AM EST
missing:~$
这里我们执行了 date 程序，它会（不出所料地）打印当前日期和时间。随后 Shell 会等待我们输入下一条命令。
我们也可以带上参数（argument）来执行命令：

missing:~$ echo hello
hello
在这个例子中，我们让 Shell 执行 echo 程序，并传入参数 hello。echo 的功能很简单：把收到的参数原样打印出来。Shell 解析这条命令时，会先按空白字符（空格、Tab 等）把整行拆分成若干部分，第一个单词就是要执行的程序，后面的每个单词都会作为参数传给它。
如果参数本身包含空格或特殊字符（例如名为「My Photos」的目录），有两种处理方式：

用 ' 或 " 把整个参数括起来，例如 "My Photos"
只对需要的字符进行转义，用反斜杠 \，例如 My\ Photos
对初学者来说最重要的命令大概是 man，即 manual（手册）的缩写。 man 可以帮你查询系统中任意命令的详细说明。比如运行 man date，它会告诉你 date 是什么、可以传入哪些参数来改变行为。对大多数命令来说，加上 --help 也能查看简短的帮助信息。

除了 man 之外，我们也推荐安装 tldr ：它会直接在终端里给出常见的命令使用示例，非常方便。此外，大语言模型通常也很擅长解释命令的工作原理，以及应该如何调用命令来实现你想完成的任务。

学会 man 之后，下一个最重要的是 cd（change directory，切换目录）。它实际上是 Shell 的内建命令，而不是独立程序（所以 which cd 会显示 no cd found）。给它传入一个路径，该路径就会成为你的当前工作目录，提示符也会随之变化。

missing:~$ cd /bin
missing:/bin$ cd /
missing:/$ cd ~
missing:~$
需要注意的是，Shell 通常自带自动补全功能，所以按下 Tab 往往能更快地补全路径。

许多命令在未指定路径时，默认作用于当前工作目录。如果不确定自己在哪个目录，可以运行 pwd（print working directory，打印当前工作目录），或者查看 $PWD 环境变量（如 echo $PWD）。两者都会输出当前路径。

当前工作目录的另一重要作用是让我们能够使用相对路径。到目前为止我们看到的都是绝对路径：以 / 开头，给出了从文件系统根目录到目标位置的完整路径。 实际使用中，相对路径更常用。之所以叫「相对」，是因为它们是相对于当前工作目录来解析的。对于相对路径（即任何不以 / 开头的路径），Shell 会先在当前工作目录中查找路径的第一部分，然后逐级向下查找。例如：

missing:~$ cd /
missing:/$ cd bin
missing:/bin$
每个目录里还都有两个「特殊路径」：. 和 .. 。 其中，. 表示「当前目录」，.. 表示「父目录」。例如：

missing:~$ cd /
missing:/$ cd bin/../bin/../bin/././../bin/..
missing:/$
大多数情况下，绝对路径和相对路径可以互换使用；但使用相对路径时，一定要清楚自己当前在哪个目录！

我们建议安装并使用 zoxide 来加速 cd 操作。它提供的 z 命令会记住你经常访问的路径，让你用更少的输入实现快速跳转。

Shell 中有哪些可用的程序？
但 Shell 怎么知道去哪找 date 或 echo 这样的程序呢？当 Shell 需要执行命令时，它会查询 $PATH 环境变量。这个变量列出一组目录，Shell 会在这些目录中搜索与命令名匹配的程序：

missing:~$ echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
missing:~$ which echo
/bin/echo
missing:~$ /bin/echo $PATH
/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
运行 echo 时，Shell 识别出需要执行名为 echo 的程序，然后在 $PATH 中以冒号（:）分隔的目录列表里逐个搜索同名文件。找到后就运行它（前提是该文件是可执行的，这点稍后详述）。
可以用 which 查看某个命令实际对应哪个文件。也可以完全绕过 $PATH，直接给出要执行文件的完整路径。

这也说明了一个办法：列出 $PATH 中所有目录的内容，就能知道 Shell 中有哪些程序可用。我们可以把目录路径传给 ls 程序来实现（名称取自「list」，用于列出文件）：

missing:~$ ls /bin
我们建议安装并使用 eza，它是一个更加现代友好的工具，用于替代 ls 。

在大多数计算机上，这会列出非常多的程序，但我们只关注其中最重要的几个。先从简单的开始：

cat hello.txt：打印 hello.txt 的全部内容
sort hello.txt：将 hello.txt 的各行按字典序排序后输出
uniq hello.txt：去除 hello.txt 中连续重复的行
head hello.txt 和 tail hello.txt：分别打印 hello.txt 的前几行和后几行
我们建议安装并使用 bat 来替代 cat，它支持语法高亮和分页滚动。

还有 grep pattern hello.txt，它会在指定文本文件中查找所有匹配 pattern 的行。这个命令非常实用，值得多花些时间了解，它的功能比你想象的要丰富得多。

这里的 pattern 实际上是正则表达式（regular expression），可以描述非常复杂的匹配模式——我们会在「代码质量」一讲中 详细讲解 。

除了指定单个文件，你也可以指定一个目录作为搜索范围（或者直接不写，默认就是当前目录 . ），并加上 -r 参数让 grep 递归搜索目录里的所有文本文件，输出匹配的行。

如果想要更快、更好用的体验，可以考虑安装 ripgrep 来替代 grep 。它默认就会递归搜索当前工作目录里的文本文件，使用起来更直观，但可移植性稍弱一些。

还有一些实用工具，使用方式稍微复杂一些。先看 sed——一个可编程的文件编辑器。它有自己的「小语言」，可以自动化地修改文件。最常见的用法是：

missing:~$ sed -i 's/pattern/replacement/g' hello.txt
这条命令会把 hello.txt 中所有的 pattern 替换为 replacement 。具体来说：

-i 参数表示直接修改文件（inline），而不是只在终端输出替换后的内容
s/ 是 sed 语法里表示「替换」的意思
两个 / 用来分隔「匹配模式」和「替换内容」
结尾的 /g 表示在每一行中替换所有匹配项，而不仅仅是第一个
译者注：
sed 是 stream editor（流编辑器） 的缩写，最早设计用来对输入流中的文本进行自动化处理，而不仅仅是单个文件。
s/ 为什么表示替换：在 sed 的命令语法里，s 就是 substitute（替换） 的首字母，表示「把匹配到的内容替换成其他内容」。
/g 为什么表示替换所有匹配项：结尾的 g 是 global（全局） 的意思，表示在每一行中替换所有匹配项，如果没有 g ，sed 只会替换每行的第一个匹配项。

和 grep 一样，这里的 pattern 也是正则表达式，可以描述非常复杂的匹配模式。此外，正则表达式替换还允许 replacement 引用匹配模式中的部分内容，我们稍后会通过例子演示这一点。

接下来是 find，它可以递归地查找满足特定条件的文件。比如：

missing:~$ find ~/Downloads -type f -name "*.zip" -mtime +30
这会在下载（Downloads）目录中查找所有超过 30 天的 ZIP 文件。

missing:~$ find ~ -type f -size +100M -exec ls -lh {} \;
这会在 home 目录中查找所有大于 100M 的文件并列出它们。注意，-exec 接受一条命令，命令以 ; 结尾（因此需要像转义空格那样对它转义）。find 会把每个匹配文件的路径替换到 {} 的位置。

missing:~$ find . -name "*.py" -exec grep -l "TODO" {} \;
这会在当前工作目录下查找所有包含 TODO （这个大写单词）的 .py 文件。

find 的语法可能有点吓人，但希望这些例子能让你感受到它的实用！

我们建议安装并使用 fd 来替代 find，它更加人性化（但可移植性稍弱）。

接下来介绍 awk，它和 sed 一样有自己的小语言。如果说 sed 专门用来编辑文件，那 awk 则专门用来解析文件。 awk 最常见的用途是处理有规则格式的数据文件（比如 CSV），从每条记录（每一行）中提取你需要的部分：

missing:~$ awk '{print $2}' hello.csv
这条命令会打印 hello.csv 中每一行的第二列（默认以空白字符分隔，空格或制表符都算）。如果你的文件是逗号分隔的（CSV 文件常见格式），可以加上 -F, 参数：

missing:~$ awk -F, '{print $2}' hello.csv
这样就会把每一行按逗号分成列，然后打印第二列。

除了提取列，awk 还能做很多事——过滤行、计算统计、求和等。具体可以通过习题动手试试。

将这些工具组合起来，我们可以完成一些很酷的操作，比如：

missing:~$ ssh myserver 'journalctl -u sshd -b-1 | grep "Disconnected from"' \
  | sed -E 's/.*Disconnected from .* user (.*) [^ ]+ port.*/\1/' \
  | sort | uniq -c \
  | sort -nk1,1 | tail -n10 \
  | awk '{print $2}' | paste -sd,
postgres,mysql,oracle,dell,ubuntu,inspur,test,admin,user,root
这条命令从远程服务器抓取 SSH 日志（ssh 会在下一讲详细介绍），搜索断开连接的消息，从中提取用户名，最后输出出现次数最多的前 10 个用户名（逗号分隔）。一切都在一条命令里完成！我们把逐步拆解这条命令留作习题。
