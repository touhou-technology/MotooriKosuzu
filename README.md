# This is an automatic translation DCbot
I adopted a method of constructing human behavior(
> [!IMPORTANT]
> This is my first time doing this, So there will be some issues*(

This is a Discord listening channel translation bot (channel monitoring is selected according to commands)
I tried adding a command to enable this program to attempt updating itself
But the program needs a daemon to replace the original program with an updated one

-------------
* how use
```bash
git clone https://github.com/touhou-technology/MotooriKosuzu
cd MotooriKosuzu
mkdir build && cd build
cmake ..
```
 Or in docker
```bash
docker pull awalwashig/bureau
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
//Enter your token (default deepl)
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
[deepl-python](https://github.com/DeepLcom/deepl-python)
* [dpp](https://github.com/brainboxdotcc/DPP);
* [cpp-httplib](https://github.com/yhirose/cpp-httplib);
* [json](https://github.com/open-source-parsers/jsoncpp);
