import deepl
import sys

text= sys.argv[1]
#gpus = [int(gpus.split(','))]
To = sys.argv[2]

auth_key = "197bc7ae-d3d4-4625-a43f-b18bd5f0e090:fx"  # Replace with your key
translator = deepl.Translator(auth_key)

result = translator.translate_text(text , target_lang=To)
print(result.text) 
