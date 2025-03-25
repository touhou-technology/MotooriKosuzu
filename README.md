# This is an automatic translation DCbot
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
td=c++20 -l"dpp" -l"pthread" -l"curl"
```
 Or in docker
```bash
sudo docker run -d --name TranslationDC -v /etc/MotooriKosuzu/config/:/etc/MotooriKosuzu/config/ awalwashig/bureau /root/Project.out
```

## Slashcommadn
> [!IMPORTANT]
> Some commands are not perfect, do not call them
> Here's how to use commands

Translations stored using the slash command are all hashed tables, so one-to-many channels cannot be opened

翻訳の開始
Open a one-way translation channel
channel -> ObjChannel

翻訳を双方向に開く
A two-way translation channel is opened, which will be deleted if there is a translation channel
channel <-> ObjChannel

翻訳の停止
Delete from the hash table

双方向翻訳の停止
Only a two-way channel can be used normally

## webhook

The webhook can only send messages, and you don't care what you do after sending it
Use webhooks to translate multiple languages at once

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
  //Your Channel the Webhook and Channel ID
  "webhook": {
    "ChannelWebhook": [
      [ "https://discord.com/api/webhooks/", 1353393450145550449 ],
      [ "https://discord.com/api/webhooks/", 1353393466050089071 ],
      [ "https://discord.com/api/webhooks/", 1353416254752555208 ]
    ],
    "ChannelKey": [
      [ 0, 2, "CH" ],
      [ 1, 2, "CH" ],
      [ 2, 0, "JA" ],
      [ 2, 1, "EN-US" ]
    ]
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
* [whisper.cpp](https://github.com/ggerganov/whisper.cpp);
* curl-dev
* openssl-dev