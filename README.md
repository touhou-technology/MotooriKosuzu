﻿# This is an automatic translation DCbot
I adopted a method of constructing human behavior(
> [!IMPORTANT]
> This is my first time doing this, So there will be some issues*(

This is a Discord listening channel translation bot (channel monitoring is selected according to commands)
I tried adding a command to enable this program to attempt updating itself
But the program needs a daemon to replace the original program with an updated one

-------------
* how use
(I haven't written a cmake yet)
```bash
git clone https://github.com/touhou-technology/MotooriKosuzu
cd MotooriKosuzu/src
g++ Application.cpp BambooSlips.h Bookshelf.hpp MotooriKosuzu.cpp MotooriKosuzu.h WritingBrush.cpp WritingBrush.h -s
td=c++20 -l"dpp" -l"ssl" -l"pthread" -l"jsoncpp" -l"curl"
```
 Or in docker
```bash
sudo docker run -d --name TranslationDC -v /etc/MotooriKosuzu/config/:/etc/MotooriKosuzu/config/ awalwashig/bureau /root/Project.out
```


## Disposition
You need to configure
```bash
mkdir -p /etc/MotooriKosuzu/config
vim /etc/MotooriKosuzu/config/ConfigBook.json
```
Enter the token for Discord and translation
```json
﻿{
  "RobotSlips": {
//Enter your DiscordBot token
    "Token": ""
  },
  "WebPen": {
//Enter your token (deepl token(only free token))
    "Token": ""
  },
  "AutoComplete": {
    "TranslationTypes": [
      {
        "language": "JA",
        "name": "Japanese",
        "supports_formality": true
      },
      {
        "language": "EN-GB",
        "name": "English (British)",
        "supports_formality": false
      },
      {
        "language": "EN-US",
        "name": "English (American)",
        "supports_formality": false
      },
      {
        "language": "ZH",
        "name": "Chinese (simplified)",
        "supports_formality": false
      }
    ]
  }
}
```
-------
## Dependencies
* [dpp](https://github.com/brainboxdotcc/DPP);
* [cpp-httplib](https://github.com/yhirose/cpp-httplib);
* [json](https://github.com/open-source-parsers/jsoncpp);
* [whisper.cpp](https://github.com/ggerganov/whisper.cpp);
* curl-dev
* openssl-dev