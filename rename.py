import os
import re

def rename_files(directory):
    # Percorre todos os arquivos no diretório e subdiretórios
    for root, dirs, files in os.walk(directory):
        for filename in files:
            if filename.endswith('.png'):
                # Encontra padrões como "Nome (N).png"
                match = re.match(r'(.*?)\s*\((\d+)\)\.png', filename)
                if match:
                    # Extrai o nome base e o número
                    base_name = match.group(1)
                    number = match.group(2)
                    # Cria o novo nome
                    new_name = f"{base_name}{number}.png"
                    
                    # Caminhos completos
                    old_path = os.path.join(root, filename)
                    new_path = os.path.join(root, new_name)
                    
                    # Renomeia o arquivo
                    os.rename(old_path, new_path)
                    print(f"Renomeado: {filename} -> {new_name}")

# Diretório onde estão as imagens
image_dir = "Images"
rename_files(image_dir)