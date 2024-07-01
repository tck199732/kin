#!/usr/bin/env python3
import json
import pathlib
import subprocess

header_pth = "/home/s063/svn/2024SAMURAI.s063/sources/Core/include"

def main():

    modules = {}
    for pth in pathlib.Path(header_pth).rglob("TArtDecoder*.hh"):
        if pth.stem in ["TArtDecoder", "TArtDecoderFactory"]:
            continue
        
        mod = subprocess.run(
            f"cat {str(pth)} | grep \"static const int kID\" | awk '{{print $6}}'", 
            shell=True, 
            capture_output=True, 
            text=True, 
        ).stdout.strip()

        mod = mod.split(';')[0]
        cls_name = pth.stem
        print(f"{cls_name:30} {mod:5}")

        modules[mod] = cls_name
    
    with open("module_map.json", "w") as f:
        json.dump(modules, f, indent=4)
    
    with open("module_map.dat", "w") as f:
        for mod, cls_name in modules.items():
            f.write(f"{mod:5} {cls_name}\n")

if __name__ == "__main__":
    main()