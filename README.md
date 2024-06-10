# 「本居小鈴」
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

