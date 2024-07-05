# 「本居小鈴」Discord translator bot

This is a Discord listening channel translation bot (channel monitoring is selected according to commands)

1. これはbotです
2. dppを使用した記述 link- https://github.com/brainboxdotcc/DPP
3. よく考えていない~

# 対応するライブラリをインストールしてください
1. cpp-httplib
2. jsoncpp
3. dpp

# コードを調整することに注意
Bookshelf.hppで std::string ConfigSlips::Path_ 独自の構成パスを書いてください
WritingBrush.cpp 76行 "python3.10 API.py" 自分の正しい呼び出しAPIコマンドを書いて、呼び出し可能な場所にAPIがあることを確認してください

srcディレクトリで使用してください g++ Application.cpp BambooSlips.h Bookshelf.hpp MotooriKosuzu.cpp MotooriKosuzu.h start.hpp WritingBrush.cpp WritingBrush.h -std=c++20 -l"dpp" -l"pthread" -l"jsoncpp"

deeplAPIを使用した
deeplの無料APIを持っていることを確認してください（有料の私は試していません）

# use Docker

docker pull awalwashig/bureau

mkdir /etc/MotooriKosuzu;
mkdir /etc/MotooriKosuzu/config;

cd /etc/MotooriKosuzu/config
vim ConfigBook.json
#The format of ConfigBook.json and config directory should be the same


docker run -d --name TranslationDC -v /etc/MotooriKosuzu/config/:/etc/MotooriKosuzu/config/ awalwashig/bureau /root/MotooriKosuzu
