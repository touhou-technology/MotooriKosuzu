import deepl
import sys

text= sys.argv[1]
#gpus = [int(gpus.split(','))]
To = sys.argv[2]
auth_key = sys.argv[3]  # Replace with your key

translator = deepl.Translator(auth_key)

result = translator.translate_text(text , target_lang=To)
print(result.text) 
