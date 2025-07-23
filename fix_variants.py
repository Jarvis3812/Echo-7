#!/usr/bin/env python3
import re
import os

# Files to fix
files = [
    "core/CRMModule.cpp",
    "core/SalesModule.cpp", 
    "core/SupportModule.cpp",
    "core/ProjectsModule.cpp",
    "core/FinanceModule.cpp",
    "core/HRModule.cpp",
    "core/InventoryModule.cpp",
    "core/ComplianceModule.cpp"
]

for file_path in files:
    if os.path.exists(file_path):
        print(f"Fixing {file_path}...")
        
        with open(file_path, 'r') as f:
            content = f.read()
        
        # Fix variant assignments
        content = re.sub(r'(\w+\.at\("[\w_]+"\))', r'variantToString(\1)', content)
        content = re.sub(r'(\w+\.count\("[\w_]+"\) \? )variantToString\((\w+\.at\("[\w_]+"\))\)', r'\1variantToString(\2)', content)
        
        # Fix cout statements with variants
        content = re.sub(r'<< (\w+\[[\w"]+\]) <<', r'<< variantToString(\1) <<', content)
        
        with open(file_path, 'w') as f:
            f.write(content)
        
        print(f"Fixed {file_path}")

print("All files fixed!")
