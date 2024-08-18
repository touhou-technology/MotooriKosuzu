# deepl_translate.py
import deepl
import sys

def translate_text(auth_key, text, target_lang):
    translator = deepl.Translator(auth_key)
    result = translator.translate_text(text, target_lang=target_lang)
    return result.text

if __name__ == "__main__":
    auth_key = sys.argv[1]
    text = sys.argv[2]
    target_lang = sys.argv[3]
    print(translate_text(auth_key, text, target_lang))